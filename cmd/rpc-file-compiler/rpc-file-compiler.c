#include "../str.h"
#include "../perfect-hash.h"
#include <stdio.h>
#include <zlib/zlib.h>
#include <stdlib.h>
#include <string.h>
#include "sha1.h"

static const char *g_argv0;

static void usage() {
	fprintf(stderr, "usage: %s <input files...> -o <output.c>\n", g_argv0);
	exit(2);
}

static void print_csymbol(FILE *f, const char *s) {
    while (*s) {
        if (('0' <= *s && *s <= '9') || ('a' <= *s && *s <= 'z') || ('A' <= *s && *s <= 'Z')) {
            fputc(*s, f);
        } else {
            fputc('_', f);
        }
        s++;
    }
}

static void print_hex(FILE *f, const uint8_t *str, int n) {
	for (int j = 0; j < n; j += 16) {
		fputc('\t', f);
		for (int i = j; i < n && i < j + 16; i++) {
			fprintf(f, "0x%02X,", str[i]);
		}

		fprintf(f, "/*");
		for (int i = j; i < n && i < j + 16; i++) {
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

static void deflate_file(str_t *vout, uint8_t *hash, z_stream *stream, FILE *in, enum FileType type) {
    static str_t read = STR_INIT;
    static str_t comp = STR_INIT;
    str_clear(&read);
    str_clear(&comp);

    if (type == TEXT) {
        str_fread_all(&read, in);
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
    } else {
        str_fread_all(&comp, in);
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

int main(int argc, char *argv[]) {
	g_argv0 = argv[0];
	if (argc < 4) {
		usage();
	}
	argv++;
	argc--;

	int i, filenum = argc;
	for (i = 0; i < argc; i++) {
		char *fn = argv[i];
		if (!strcmp(fn, "-o")) {
            filenum = i;
			i++;
			break;
		}
		
		clean_slashes(fn);
    }

	char *outfn = argv[i];
	clean_slashes(outfn);

	FILE *cf = fopen(outfn, "w");
	if (!cf) {
		fprintf(stderr, "failed to open %s\n", outfn);
		usage();
	}

	// remove the extension from the output file name as this is used for the symbol name
	char *outext = strrchr(outfn, '.');
	if (outext) {
		*outext = 0;
	}

	str_t dir = STR_INIT;
	char *slash = strrchr(outfn, '/');
	if (slash) {
		str_add2(&dir, outfn, slash - outfn + 1);
	}

	fprintf(cf, "#include <protorpc.h>\n");

	struct hash_entry *entries = (struct hash_entry*) calloc(filenum, sizeof(struct hash_entry));
	str_t *paths = (str_t*) calloc(filenum, sizeof(str_t));

    str_t vout = STR_INIT;
	z_stream stream;
	memset(&stream, 0, sizeof(stream));
	deflateInit2(&stream, Z_BEST_COMPRESSION, Z_DEFLATED, 15+GZIP_ENCODING, 9, Z_DEFAULT_STRATEGY);

    for (i = 0; i < filenum; i++) {
        char *fn = argv[i];
        FILE *in = fopen(fn, "rb");
        if (!in) {
            fprintf(stderr, "failed to open %s\n", fn);
            exit(1);
        }

		str_t tempfn = STR_INIT;
		str_add(&tempfn, fn);
		str_add(&tempfn, ".out.gz");

		if (strlen(fn) < (size_t)dir.len || strncmp(dir.c_str, fn, dir.len)) {
			fprintf(stderr, "different root dir for %s then previous %s\n", fn, dir.c_str);
			return 4;
		}
		fn += dir.len;

        char *ext = strrchr(fn, '.');
        if (!ext) {
            fprintf(stderr, "unknown file type for %s\n", fn);
            exit(2);
        }

		char *fndir = strrchr(fn, '/');

        enum FileType type = TEXT;
        const char *meta = NULL;
		int is_index = fndir ? !strcmp(fndir, "/index.html") : !strcmp(fn, "index.html");

		if (!strcmp(ext, ".html")) {
            meta = "text/html";
        } else if (!strcmp(ext, ".js")) {
            meta = "application/javascript";
        } else if (!strcmp(ext, ".css")) {
            meta = "text/css";
        } else if (!strcmp(ext, ".svg")) {
            meta = "image/svg+xml";
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
        deflate_file(&vout, hash, &stream, in, type);
		fclose(in);

#if 0
		FILE *tf = fopen(tempfn.c_str, "wb");
		if (tf) {
			fwrite(vout.c_str, 1, vout.len, tf);
			fclose(tf);
		}
#endif
		str_destroy(&tempfn);
		
		str_init(&paths[i]);
		if (is_index) {
			str_addch(&paths[i], '/');
			if (fndir) {
				str_add2(&paths[i], fn, fndir - fn);
			}
		} else {
			str_addch(&paths[i], '/');
			str_add2(&paths[i], fn, ext - fn);
			str_addch(&paths[i], '.');
			str_addf(&paths[i], "%02X%02X%02X%02X%02X", hash[0], hash[1], hash[2], hash[3], hash[4]);
			str_add(&paths[i], ext);
		}

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

        fprintf(cf, "static const uint8_t g_");
        print_csymbol(cf, fn);
        fprintf(cf, "[] = {\n");
		print_hex(cf, (uint8_t*) hdr, hlen);
        print_hex(cf, (uint8_t*) vout.c_str, vout.len);
        fprintf(cf, "};\n\n");

		entries[i].str = paths[i].c_str;
    }

	uint32_t hashmul;
	uint32_t hashsz;
	calc_hash_values(entries, filenum, &hashmul, &hashsz);

	fprintf(cf, "static const char g_not_found[] = \"HTTP/1.1 404 Not Found\\r\\nContent-Length:0\\r\\n\\r\\n\";\n\n");

	fprintf(cf, "extern int ");
	print_csymbol(cf, outfn + dir.len);
	fprintf(cf, "(const char *path, pb_buf_t *out);\n\n");

	fprintf(cf, "int ");
	print_csymbol(cf, outfn + dir.len);
	fprintf(cf, "(const char *path, pb_buf_t *out) {\n");
	fprintf(cf, "\tswitch (pr_hash(path, %u) %% %u) {\n", hashmul, hashsz);

	for (i = 0; i < filenum; i++) {
		fprintf(cf, "\tcase %u:\n", entries[i].off);
		fprintf(cf, "\t\tif (strcmp(path, \"%s\")) {\n", entries[i].str);
		fprintf(cf, "\t\t\tgoto unknown;\n");
		fprintf(cf, "\t\t}\n");
		fprintf(cf, "\t\treturn pb_append(out, (char*) g_");
		print_csymbol(cf, argv[i] + dir.len);
		fprintf(cf, ", sizeof(g_");
		print_csymbol(cf, argv[i] + dir.len);
		fprintf(cf, "));\n");
	}

	fprintf(cf, "\tdefault:\n");
	fprintf(cf, "\tunknown:\n");
	fprintf(cf, "\t\treturn pb_append(out, g_not_found, sizeof(g_not_found)-1);\n");
	fprintf(cf, "\t}\n");
	fprintf(cf, "}\n");
	fclose(cf);
	return 0;
}

