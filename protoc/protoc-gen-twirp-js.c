#include "protorpc/protorpc.h"
#include "exec.h"

#define ALLOC_SIZE (1*1024*1024)

static FileDescriptorProto *find_file(const CodeGeneratorRequest *r, pb_string name) {
	for (int i = 0; i < r->proto_file.len; i++) {
		if (str_equals(r->proto_file.v[i]->name, name)) {
			return r->proto_file.v[i];
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

	str_t in = STR_INIT;
	str_fread_all(&in, stdin, STR_BINARY);

	pb_allocator alloc;
	alloc.next = malloc(ALLOC_SIZE);
	alloc.end = alloc.next + ALLOC_SIZE:

	CodeGeneratorRequest *r = (CodeGeneratorRequest*) pb_decode(&alloc, &type_CodeGeneratorRequest, in.c_str, in.len);

	

	return 0;
}