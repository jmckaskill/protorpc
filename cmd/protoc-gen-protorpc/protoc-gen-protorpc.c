#include "protoc-gen-protorpc.h"

#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#endif

static void declare_external_fields(str_t *o, const struct FileDescriptorProto *f, const struct DescriptorProto *msg) {
    for (int i = 0; i < msg->field.len; i++) {
        struct type *ft = get_field_type(msg->field.v[i]);
        if (ft->msg && ft->file && ft->file != f && ft->declared != f) {
            ft->declared = f;
            declare_struct(o, ft);
        }
    }
}

static void write_header(str_t *o, const struct FileDescriptorProto *f) {
    str_add(o, "#pragma once" EOL);
    str_add(o, "#include <protobuf.h>" EOL);

    for (int i = 0; i < f->dependency.len; i++) {
		struct pb_string d = f->dependency.v[i];
		if (!pb_cmp(d, "google/protobuf/empty.proto")) {
		} else if (!pb_cmp(d, "google/protobuf/duration.proto") || !pb_cmp(d, "google/protobuf/timestamp.proto")) {
            str_add(o, "#include <protorpc-time.h>" EOL);
		} else {
			str_add(o, "#include \"");
			str_addstr(o, d);
			str_add(o, ".h\"" EOL);
		}
    }

    str_add(o, "#ifdef __cplusplus" EOL);
	str_add(o, "extern \"C\" {" EOL);
    str_add(o, "#endif" EOL);

    for (int i = 0; i < f->enum_type.len; i++) {
        define_enum(o, get_enum_type(f->enum_type.v[i]));
    }

    for (int i = 0; i < f->message_type.len; i++) {
        define_struct(o, get_struct_type(f->message_type.v[i]));
    }

    for (int i = 0; i < f->service.len; i++) {
        const struct type *t = get_service_type(f->service.v[i]);
        do_server(o, t, 0);
    }

    for (int i = 0; i < f->enum_type.len; i++) {
        do_enum_funcs(o, get_enum_type(f->enum_type.v[i]), false);
    }
    for (int i = 0; i < f->message_type.len; i++) {
        do_struct_funcs(o, get_struct_type(f->message_type.v[i]), false);
    }
   
    for (int i = 0; i < f->service.len; i++) {
        const struct type *t = get_service_type(f->service.v[i]);
        do_server(o, t, 1);
    }

    str_add(o, "#ifdef __cplusplus" EOL);
	str_add(o, "}" EOL);
	str_add(o, "#endif" EOL);
}

static void write_source(str_t *o, const struct FileDescriptorProto *f) {
    str_add(o, "#include \"");
    str_addstr(o, f->name);
    str_add(o, ".h\"" EOL);
    
    for (int i = 0; i < f->enum_type.len; i++) {
        const struct type *t = get_enum_type(f->enum_type.v[i]);
        do_enum_funcs(o, t, true);
    }

    for (int i = 0; i < f->message_type.len; i++) {
        const struct type *t = get_struct_type(f->message_type.v[i]);
        do_struct_funcs(o, t, true);
    }

	for (int i = 0; i < f->service.len; i++) {
		const struct type *t = get_service_type(f->service.v[i]);
		do_server(o, t, 2);
	}
}

static const struct FileDescriptorProto *get_file_proto(struct CodeGeneratorRequest *req, struct pb_string name) {
    for (int i = 0; i < req->proto_file.len; i++) {
        if (str_equals(req->proto_file.v[i]->name, name)) {
            return req->proto_file.v[i];
        }
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc >= 3) {
        // we've been called directly
        // call protoc with the arguments which will then call the backend back
        return exec_protoc(argv[0], argv[1], argv[2]);
    }
#ifdef _WIN32
	_setmode(_fileno(stdin), _O_BINARY);
#endif

    str_t in = STR_INIT;
    str_fread_all(&in, stdin);

	size_t bufsz = 1 * 1024 * 1024;
	char *obuf = malloc(bufsz);
	pb_buf_t obj = { obuf, obuf + bufsz };
    struct CodeGeneratorRequest req = {0};
	if (pb_get_CodeGeneratorRequest(in.buf, in.buf + in.len, &obj, &req)) {
		fprintf(stderr, "protoc-gen-protorpc - failed to parse input\n");
		return 2;
	}

    for (int i = 0; i < req.proto_file.len; i++) {
        insert_file_types(req.proto_file.v[i]);
    }

    str_t hdr = STR_INIT;
    str_t src = STR_INIT;
    str_t filename = STR_INIT;

    for (int i = 0; i < req.file_to_generate.len; i++) {
        const struct FileDescriptorProto *f = get_file_proto(&req, req.file_to_generate.v[i]);

		if (f == NULL) {
			fprintf(stderr, "protoc-gen-protorpc - can't find %.*s\n", STRF(req.file_to_generate.v[i]));
			return 2;
		}

        str_clear(&hdr);
        str_clear(&src);
    
        write_header(&hdr, f);
        write_source(&src, f);

        str_clear(&filename);
        str_addstr(&filename, f->name);
        str_add(&filename, ".h");
        FILE *file = fopen(filename.buf, "wb");
		if (!file) {
			fprintf(stderr, "failed to open %s\n", filename.buf);
			return 2;
		}
        fwrite(hdr.buf, 1, hdr.len, file);
        fclose(file);

        str_clear(&filename);
        str_addstr(&filename, f->name);
        str_add(&filename, ".c");
        file = fopen(filename.buf, "wb");
		if (!file) {
			fprintf(stderr, "failed to open %s\n", filename.buf);
			return 2;
		}
        fwrite(src.buf, 1, src.len, file);
        fclose(file);
    }

    return 0;
}
