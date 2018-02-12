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

enum FileType {
	BINARY,
	TEXT,
};

static void deflate_file(str_t *vout, uint8_t *hash, z_stream *stream, FILE *in, enum FileType type, const struct entry *ev, int ec) {
    static str_t read = STR_INIT;
    static str_t comp = STR_INIT;
	static str_t http_url = STR_INIT;
    str_clear(&read);
    str_clear(&comp);

    if (type == TEXT) {
        str_fread_all(&read, in, STR_TEXT);
        char *p = read.c_str;

        for (;;) {
            size_t left = read.c_str + read.len - p;
            char *nl = (char*) memchr(p, '\n', left);
            if (!nl) {
                str_add2(&comp, p, left);
                break;
            } else if (nl > p && nl[-1] == '\r') {
                str_add2(&comp, p, nl - 1 - p);
				str_addch(&comp, '\n');
            } else {
                str_add2(&comp, p, nl + 1 - p);
            }
			p = nl + 1;
        }

		for (int i = 0; i < ec; i++) {
			const struct entry *e = &ev[i];
			str_clear(&http_url);
			str_addch(&http_url, '"');
			str_addstr(&http_url, e->http);
			str_addch(&http_url, '"');
			str_replace_all(&comp, e->replace.c_str, http_url.c_str);
		}
    } else {
        str_fread_all(&comp, in, STR_BINARY);
    }

    stream->next_in = (uint8_t*) comp.c_str;
    stream->avail_in = comp.len;

    while (stream->avail_in) {
        do_deflate(vout, stream, Z_NO_FLUSH);
    }

    while (do_deflate(vout, stream, Z_FINISH) != Z_STREAM_END) {
    }

	SHA1_CTX ctx;
	SHA1Init(&ctx);
	SHA1Update(&ctx, (uint8_t*) comp.c_str, comp.len);
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

	FILE *cf = stdout;
	if (outfn) {
		clean_slashes((char*) outfn);
		cf = fopen(outfn, "w");
		if (!cf) {
			fprintf(stderr, "failed to open %s\n", outfn);
			return 3;
		}
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
        FILE *in = fopen(fn, "r");
        if (!in) {
            fprintf(stderr, "failed to open %s\n", fn);
            exit(1);
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

        enum FileType type = TEXT;
        const char *meta = NULL;
		int is_index = !strcmp(fn, "index.html");

		if (!strcmp(ext, ".html")) {
            meta = "text/html;charset=utf-8";
        } else if (!strcmp(ext, ".js")) {
            meta = "application/javascript;charset=utf-8";
        } else if (!strcmp(ext, ".css")) {
            meta = "text/css;charset=utf-8";
        } else if (!strcmp(ext, ".svg")) {
            meta = "image/svg+xml;charset=utf-8";
        } else if (!strcmp(ext, ".ttf")) {
            meta = "font/opentype";
            type = BINARY;
        } else {
            fprintf(stderr, "unknown file type for %s\n", fn);
            exit(2);
        }

        str_clear(&vout);
        deflateReset(&stream);

        uint8_t hash[20];
        deflate_file(&vout, hash, &stream, in, type, entries, i);
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
			str_add2(&e->http, fn, ext - fn);
			str_addch(&e->http, '.');
			str_addf(&e->http, "%02X%02X%02X%02X%02X", hash[0], hash[1], hash[2], hash[3], hash[4]);
			str_add(&e->http, ext);
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
			"%s\r\n",
			vout.len,
			meta,
			is_index ? "" : "Cache-Control:max-age=31536000\r\n");

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

