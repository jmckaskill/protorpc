#pragma once
#include <protorpc/protorpc.h>
#include "protorpc/cmd/protoc-gen-protorpc/descriptor.proto.h"
#ifdef __cplusplus
extern "C" {
#endif
struct CodeGeneratorRequest {
	union pb_msg pb_hdr;
	struct {int len; struct pb_string const *v;}	file_to_generate;
	struct pb_string	parameter;
	struct {int len; struct FileDescriptorProto const **v;}	proto_file;
};
struct CodeGeneratorResponse_File {
	union pb_msg pb_hdr;
	struct pb_string	name;
	struct pb_string	insertion_point;
	struct pb_string	content;
};
struct CodeGeneratorResponse {
	union pb_msg pb_hdr;
	struct pb_string	error;
	struct {int len; struct CodeGeneratorResponse_File const **v;}	file;
};

char *pb_parse_CodeGeneratorRequest(char *p, pb_alloc_t *a, struct CodeGeneratorRequest *m);
int pb_fparse_CodeGeneratorRequest(const char *fn, str_t *b, pb_alloc_t *a, struct CodeGeneratorRequest *m);
void pb_print_CodeGeneratorRequest(str_t *a, struct CodeGeneratorRequest const *m);
void pb_get_CodeGeneratorRequest(const char *p, const char *e, pb_alloc_t *obj, struct CodeGeneratorRequest *m);
char *pb_encode_CodeGeneratorRequest(char *p, struct CodeGeneratorRequest const *m);
int pb_maxsz_CodeGeneratorRequest(struct CodeGeneratorRequest const *m);

char *pb_parse_CodeGeneratorResponse(char *p, pb_alloc_t *a, struct CodeGeneratorResponse *m);
int pb_fparse_CodeGeneratorResponse(const char *fn, str_t *b, pb_alloc_t *a, struct CodeGeneratorResponse *m);
void pb_print_CodeGeneratorResponse(str_t *a, struct CodeGeneratorResponse const *m);
void pb_get_CodeGeneratorResponse(const char *p, const char *e, pb_alloc_t *obj, struct CodeGeneratorResponse *m);
char *pb_encode_CodeGeneratorResponse(char *p, struct CodeGeneratorResponse const *m);
int pb_maxsz_CodeGeneratorResponse(struct CodeGeneratorResponse const *m);

char *pb_parse_CodeGeneratorResponse_File(char *p, pb_alloc_t *a, struct CodeGeneratorResponse_File *m);
int pb_fparse_CodeGeneratorResponse_File(const char *fn, str_t *b, pb_alloc_t *a, struct CodeGeneratorResponse_File *m);
void pb_print_CodeGeneratorResponse_File(str_t *a, struct CodeGeneratorResponse_File const *m);
void pb_get_CodeGeneratorResponse_File(const char *p, const char *e, pb_alloc_t *obj, struct CodeGeneratorResponse_File *m);
char *pb_encode_CodeGeneratorResponse_File(char *p, struct CodeGeneratorResponse_File const *m);
int pb_maxsz_CodeGeneratorResponse_File(struct CodeGeneratorResponse_File const *m);
#ifdef __cplusplus
}
#endif
