#include <protorpc/str.h>
#include <protorpc/flag.h>
#include <stdio.h>
#include <zlib/zlib.h>
#include <stdlib.h>
#include <string.h>
#include "sha1.h"

struct entry {
	str_t http;
	str_t replace;
	int idx;
	int datalen;
};

static void to_csymbol(str_t *o, const char *fn) {
	str_clear(o);

	// strip the extension
	const char *ext = strrchr(fn, '.');
	if (!ext) {
		ext = fn + strlen(fn);
	}

	while (fn < ext) {
		if (('0' <= *fn && *fn <= '9') || ('a' <= *fn && *fn <= 'z') || ('A' <= *fn && *fn <= 'Z')) {
			str_addch(o, *fn);
		} else {
			str_addch(o, '_');
		}
		fn++;
	}
}

static void print_hex(FILE *f, const uint8_t *str, int n) {
	for (int j = 0; j < n; j += 16) {
		fputc('\t', f);
		int i;
		for (i = j; i < n && i < j + 16; i++) {
			fprintf(f, "0x%02X,", str[i]);
		}

		while (i < j + 16) {
			fputs("     ", f);
			i++;
		}

		fprintf(f, " /*");
		for (i = j; i < n && i < j + 16; i++) {
			char ch = (char)str[i];
			if (' ' <= ch && ch < 0x7F && ch != '*') {
				fputc(ch, f);
			} else {
				fputc('.', f);
			}
		}
		fprintf(f, "*/");
		fputc('\n', f);
	}
}

static int do_deflate(str_t *v, z_stream *stream, int flush) {
    str_grow(v, v->len + 1024*1024);
    stream->next_out = (uint8_t*) v->c_str + v->len;
    stream->avail_out = v->cap - v->len;
	int	err = deflate(stream, flush);
	if (err && err != Z_STREAM_END) {
		fprintf(stderr, "zlib error %d\n", err);
		exit(1);
	}

    str_setlen(v, (int) ((char*)stream->next_out - v->c_str));
	return err;
}

static void deflate_file(str_t *vout, uint8_t *hash, z_stream *stream, FILE *in, const struct entry *ev, int ec) {
    static str_t file = STR_INIT;
	static str_t http_url = STR_INIT;
    str_clear(&file);

	str_fread_all(&file, in);

	for (int i = 0; i < ec; i++) {
		const struct entry *e = &ev[i];
		str_clear(&http_url);
		str_addch(&http_url, '"');
		str_addstr(&http_url, e->http);
		str_addch(&http_url, '"');
		if (!str_equals(http_url, e->replace)) {
			str_replace_all(&file, e->replace.c_str, http_url.c_str);
		}
	}

    stream->next_in = (uint8_t*)file.c_str;
    stream->avail_in = file.len;

    while (stream->avail_in) {
        do_deflate(vout, stream, Z_NO_FLUSH);
    }

    while (do_deflate(vout, stream, Z_FINISH) != Z_STREAM_END) {
    }

	SHA1_CTX ctx;
	SHA1Init(&ctx);
	SHA1Update(&ctx, (uint8_t*)file.c_str, file.len);
	SHA1Final(hash, &ctx);
}

#define GZIP_ENCODING 16

static void clean_slashes(char *str) {
    while (*str) {
        if (*str == '\\') {
            *str = '/';
        }
        str++;
    }
}

static int compare_entry(const void *a, const void *b) {
	struct entry *ea = (struct entry*) a;
	struct entry *eb = (struct entry*) b;
	int diff = ea->http.len - eb->http.len;
	if (!diff) {
		diff = memcmp(ea->http.c_str, eb->http.c_str, ea->http.len);
	}
	return diff;
}

static const char *filename(const char *path) {
	const char *slash = strrchr(path, '/');
	return slash ? slash + 1 : path;
}

int main(int argc, char *argv[]) {
	const char *outfn = NULL;
	flag_string(&outfn, 'o', "output", "Output filename");
	flag_parse(&argc, argv, "rpc-file-compiler <input files...>", 1);
	
	for (int i = 0; i < argc; i++) {
		clean_slashes(argv[i]);
    }

	if (!outfn) {
		return flag_error(4, "no output specified");
	}

	clean_slashes((char*) outfn);
	FILE *cf = fopen(outfn, "wb");
	if (!cf) {
		return flag_error(5, "failed to open %s", outfn);
	}

	str_t outsym = STR_INIT;
	str_set(&outsym, "proto_dir");

	if (outfn) {
		to_csymbol(&outsym, filename(outfn));
	}

	fprintf(cf, "#include <protorpc/protorpc.h>\n");

	struct entry *entries = (struct entry*)calloc(argc, sizeof(struct entry));

    str_t vout = STR_INIT;
	z_stream stream;
	memset(&stream, 0, sizeof(stream));
	deflateInit2(&stream, Z_BEST_COMPRESSION, Z_DEFLATED, 15+GZIP_ENCODING, 9, Z_DEFAULT_STRATEGY);

    for (int i = 0; i < argc; i++) {
        const char *fn = argv[i];
        FILE *in = fopen(fn, "rb");
        if (!in) {
			return flag_error(6, "failed to open %s", fn);
        }

#if 0
		str_t tempfn = STR_INIT;
		str_add(&tempfn, fn);
		str_add(&tempfn, ".out.gz");
#endif

		// strip the directory from the name
		fn = filename(fn);

        char *ext = strrchr(fn, '.');
        if (!ext) {
            fprintf(stderr, "unknown file type for %s\n", fn);
            exit(2);
        }

        const char *meta = NULL;
		int is_index = !strcmp(fn, "index.html");
		bool cache = true;

#ifndef NDEBUG
		// disable caching on debug builds to make debugging in browsers easier
		cache = false;
#endif

		if (!strcmp(ext, ".html")) {
			// Caching is disabled on html. These are root files
			// and should have sensible URLs which can be refreshed.
			cache = false;
            meta = "text/html;charset=utf-8";
        } else if (!strcmp(ext, ".js")) {
            meta = "application/javascript;charset=utf-8";
        } else if (!strcmp(ext, ".css")) {
            meta = "text/css;charset=utf-8";
        } else if (!strcmp(ext, ".svg")) {
            meta = "image/svg+xml;charset=utf-8";
        } else if (!strcmp(ext, ".ttf")) {
            meta = "font/opentype";
        } else {
            fprintf(stderr, "unknown file type for %s\n", fn);
            exit(2);
        }

        str_clear(&vout);
        deflateReset(&stream);

        uint8_t hash[20];
        deflate_file(&vout, hash, &stream, in, entries, i);
		fclose(in);

#if 0
		FILE *tf = fopen(tempfn.c_str, "wb");
		if (tf) {
			fwrite(vout.c_str, 1, vout.len, tf);
			fclose(tf);
		}
		str_destroy(&tempfn);
#endif
		
		struct entry *e = &entries[i];
		e->idx = i;

		str_init(&e->http);
		str_addch(&e->http, '/');
		if (!is_index) {
			if (cache) {
				str_add2(&e->http, fn, ext - fn);
				str_addch(&e->http, '.');
				str_addf(&e->http, "%02X%02X%02X%02X%02X", hash[0], hash[1], hash[2], hash[3], hash[4]);
				str_add(&e->http, ext);
			} else {
				str_add(&e->http, fn);
			}
		}

		str_init(&e->replace);
		str_add(&e->replace, "\"/");
		str_add(&e->replace, fn);
		str_add(&e->replace, "\"");

		char hdr[256];
		int hlen = sprintf(hdr,
			"HTTP/1.1 200 OK\r\n"
			"Content-Length:%u\r\n"
			"Content-Type:%s\r\n"
			"Content-Encoding:gzip\r\n"
			"Cache-Control:%s\r\n"
			"\r\n",
			vout.len,
			meta,
			cache ? "max-age=31536000" : "no-cache");

		fprintf(cf, "\nstatic const uint8_t data_%d[] = {\n", i);
		print_hex(cf, (uint8_t*)hdr, hlen);
		print_hex(cf, (uint8_t*)vout.c_str, vout.len);
		fprintf(cf, "};\n");
		e->datalen = hlen + vout.len;
    }

	for (int i = 0; i < argc; i++) {
		struct entry *e = &entries[i];
		fprintf(cf, "\n// Source URL %s\n", e->replace.c_str);
		fprintf(cf, "static const proto_file file_%d = {\n", i);
		fprintf(cf, "\t{%d, \"%s\"},\n", e->http.len, e->http.c_str);
		fprintf(cf, "\t(const char*) data_%d,\n", i);
		fprintf(cf, "\t%d\n", e->datalen);
		fprintf(cf, "};\n");
	}

	qsort(entries, argc, sizeof(entries[0]), &compare_entry);

	fprintf(cf, "\nstatic const pb_string *by_path[] = {\n");
	for (int i = 0; i < argc; i++) {
		fprintf(cf, "\t&file_%d.path,\n", entries[i].idx);
	}
	fprintf(cf, "};\n");

	fprintf(cf, "\nextern const proto_dir dir_%s;\n", outsym.c_str);
	fprintf(cf, "\nconst proto_dir dir_%s = {%d, by_path};\n", outsym.c_str, argc);

	if (cf != stdout) {
		fclose(cf);
	}

	return 0;
}

