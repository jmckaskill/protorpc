#include <stdio.h>
#include <zlib/zlib.h>
#include <stdlib.h>
#include <bearssl.h>

#ifdef WIN32
#include <windows.h>
#else
#include <dirent.h>
#endif

static const char *g_argv0;

static void usage() {
	fprintf(stderr, "usage: %s -o <output.cpp> <input files...>\n", g_argv0);
	exit(2);
}

static void add_string(FILE *f, const uint8_t *str, int n) {
	for (const uint8_t *p = str; p < str+n; p++) {
		if (!(uintptr_t(p) & 31)) {
			fprintf(f, "\n\t");
		}
		fprintf(f, "0x%02X,", *p);
	}
}

static int do_deflate(std::vector<uint8_t> *v, z_stream *stream, int flush) {
	static uint8_t out[1024*1024];
	stream->next_out = out;
	stream->avail_out = sizeof(out);
	int	err = deflate(stream, flush);
	if (err && err != Z_STREAM_END) {
		fprintf(stderr, "zlib error %d\n", err);
		exit(1);
	}

	v->insert(v->end(), &out[0], stream->next_out);
	return err;
}
enum FileType {
	BINARY,
	TEXT,
};

static void deflate_file(std::vector<uint8_t> *vout, br_sha1_context *sha, z_stream *stream, FILE *in, FileType type) {
	while (!feof(in)) {
		static uint8_t buf[1024 * 1024];
		int n = fread(buf, 1, sizeof(buf)-1, in);
		if (n < 0) {
			fprintf(stderr, "read error\n");
			exit(1);
		}

		stream->next_in = buf;
		stream->avail_in = n;

		if (type == TEXT) {
			char *p = (char*) &buf[0];
			buf[n] = '\0';
			for (;;) {
				char *nl = strstr(p, "\r\n");
				if (nl) {
					br_sha1_update(sha, p, nl-p);
					p = nl+1;
				} else {
					br_sha1_update(sha, p, strlen(p));
					break;
				}
			}
		} else {
			br_sha1_update(sha, buf, n);
		}

		while (stream->avail_in) {
			do_deflate(vout, stream, Z_NO_FLUSH);
		}
	}

	while (do_deflate(vout, stream, Z_FINISH) != Z_STREAM_END) {
	}
}

static std::string directory(const std::string& path) {
	auto off = path.rfind('/');
	return (off == std::string::npos) ? "" : path.substr(0, off);
}

static std::string extension(const std::string& path) {
	auto off = path.rfind('.');
	return (off == std::string::npos) ? "" : path.substr(off);
}

static std::string filename(const std::string& path) {
	auto off = path.rfind('/');
	return (off == std::string::npos) ? path : path.substr(off+1);
}

#define GZIP_ENCODING 16

static void clean_slashes(std::string& str) {
	for (size_t j = 0; j < str.size(); j++) {
		if (str[j] == '\\') {
			str[j] = '/';
		}
	}
}

int main(int argc, char *argv[]) {
	g_argv0 = argv[0];
	if (argc < 4) {
		usage();
	}

	std::map<std::string, std::pair<std::string, FileType> > exts;
	exts[".html"] = std::make_pair("text/html", TEXT);
	exts[".js"] = std::make_pair("application/json", TEXT);
	exts[".css"] = std::make_pair("text/css", TEXT);
	exts[".svg"] = std::make_pair("image/svg+xml", TEXT);
	exts[".ttf"] = std::make_pair("font/opentype", BINARY);

	// the root folder is the directory of the first file
	bool havedir = false;
	std::string dir;

	int i;
	std::map<std::string, std::string> files;
	for (i = 1; i < argc; i++) {
		std::string fn = argv[i];
		if (fn == "-o") {
			i++;
			break;
		}
		
#ifdef WIN32
		clean_slashes(fn);
#endif
		if (!havedir) {
			dir = directory(fn);
			havedir = true;
		}
		if (fn.size() < dir.size() || fn.substr(0, dir.size()) != dir) {
			fprintf(stderr, "expected %s to have %s as a prefix\n", fn.c_str(), dir.c_str());
			return 3;
		}
		files[fn.substr(dir.size())] = fn;
	}

	std::string cppout, hout;

	for (;i < argc; i++) {
		std::string fn = argv[i];
		std::string ext = extension(fn);
		clean_slashes(fn);
		if (ext == ".h" || ext == ".H") {
			hout = fn;
		} else {
			cppout = fn;
		}
	}

	FILE *cf = fopen(cppout.c_str(), "w");
	FILE *hf = fopen(hout.c_str(), "w");
	if (!cf || !hf) {
		fprintf(stderr, "failed to open %s or %s\n", hout.c_str(), cppout.c_str());
		usage();
	}

	fprintf(cf, "#include \"%s\"\n", hout.c_str());
	fprintf(cf, "#include <protorpc/rpc.hpp>\n");

	std::vector<uint8_t> vout;
	z_stream stream = {};
	deflateInit2(&stream, Z_BEST_COMPRESSION, Z_DEFLATED, 15+GZIP_ENCODING, 9, Z_DEFAULT_STRATEGY);

	fprintf(cf, "static uint8_t g_data[] = {\n");
	size_t off = 0;
	std::vector<std::tuple<std::string, size_t, size_t> > offsets;
	for (auto it = files.begin(); it != files.end(); ++it) {
		std::string ext = extension(it->first);
		auto type = exts.find(ext);
		if (type == exts.end()) {
			continue;
		}

		FILE *in = fopen(it->second.c_str(), "rb");

		if (!in) {
			fprintf(stderr, "open error\n");
			exit(1);
		}

		SHA1_CTX sha;
		SHA1_Init(&sha);

		deflateReset(&stream);
		vout.clear();
		deflate_file(&vout, &sha, &stream, in, type->second.second);

		uint8_t hash[SHA1_DIGEST_SIZE];
		SHA1_Final(hash, &sha);

		size_t extlen = extension(it->first).size();

		char buf[sizeof(hash)*2/2];
		for (i = 0; i < sizeof(hash)/2; i++) {
			const char hex[] = "0123456789abcdef";
			buf[2*i] = hex[hash[i] >> 4];
			buf[2*i+1] = hex[hash[i] & 0xF];
		}
		std::string static_path = it->first;
		static_path.insert(static_path.size() - extlen, ".");
		static_path.insert(static_path.size() - extlen, buf, sizeof(buf));

		const char *cached = "Cache-Control:max-age=31556926\r\n";
		char hdr[256];
		int hlen = sprintf(hdr, 
			"Content-Length:%u\r\n"
			"Content-Type:%s\r\n"
			"Content-Encoding:gzip\r\n"
			"\r\n",
			unsigned(vout.size()),
			type->second.first.c_str());
		add_string(cf, (uint8_t*)cached, strlen(cached));
		add_string(cf, (uint8_t*)hdr, hlen);
		add_string(cf, vout.empty() ? NULL : &vout[0], vout.size());
		
		std::string path = it->first;
		if (filename(it->first) == "index.html") {
			path = path.substr(0, path.size() - strlen("index.html"));
		}
		offsets.push_back(std::make_tuple(static_path, off, strlen(cached) + hlen + vout.size()));
		offsets.push_back(std::make_tuple(path, off + strlen(cached), hlen + vout.size()));

		off += strlen(cached) + hlen + vout.size();
		fclose(in);
	}
	deflateEnd(&stream);

	std::string initfn = filename(cppout);
	initfn = initfn.substr(0, initfn.size() - extension(initfn).size());
	for (i = 0; i < (int) initfn.size(); i++) {
		if (initfn[i] == '-' || initfn[i] == '.') {
			initfn[i] = '_';
		}
	}
	fprintf(hf, "class rpc_server;\n");
	fprintf(hf, "void register_%s(rpc_server *s);\n", initfn.c_str());

	fprintf(cf, "\n};\n");
	fprintf(cf, "void register_%s(rpc_server *s) {\n", initfn.c_str());

	for (auto it = offsets.begin(); it != offsets.end(); ++it) {
		fprintf(cf, "\ts->handle_file(\"%s\", &g_data[%zu], %zu);\n", std::get<0>(*it).c_str(), std::get<1>(*it), std::get<2>(*it));
	}

	fprintf(cf, "}\n");
	fclose(cf);
	fclose(hf);
	return 0;
}