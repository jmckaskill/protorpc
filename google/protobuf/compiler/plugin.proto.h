#pragma once
#include <protobuf.h>
#include "google/protobuf/descriptor.proto.h"
#ifdef __cplusplus
extern "C" {
#endif
struct Version {
	union pb_msg pb_hdr;
	int32_t	major;
	int32_t	minor;
	int32_t	patch;
	struct pb_string	suffix;
};
struct CodeGeneratorRequest {
	union pb_msg pb_hdr;
	struct {int len; struct pb_string const *v;}	file_to_generate;
	struct pb_string	parameter;
	struct {int len; struct FileDescriptorProto const **v;}	proto_file;
	struct Version const*	compiler_version;
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

char *pb_parse_Version(char *p, pb_buf_t *obj, struct Version *m);
int pb_print_Version(pb_buf_t *a, struct Version const *m);
int pb_get_Version(const char *p, const char *e, pb_buf_t *obj, struct Version *m);
char *pb_encode_Version(char *p, struct Version const *m);
int pb_maxsz_Version(struct Version const *m);

char *pb_parse_CodeGeneratorRequest(char *p, pb_buf_t *obj, struct CodeGeneratorRequest *m);
int pb_print_CodeGeneratorRequest(pb_buf_t *a, struct CodeGeneratorRequest const *m);
int pb_get_CodeGeneratorRequest(const char *p, const char *e, pb_buf_t *obj, struct CodeGeneratorRequest *m);
char *pb_encode_CodeGeneratorRequest(char *p, struct CodeGeneratorRequest const *m);
int pb_maxsz_CodeGeneratorRequest(struct CodeGeneratorRequest const *m);

char *pb_parse_CodeGeneratorResponse(char *p, pb_buf_t *obj, struct CodeGeneratorResponse *m);
int pb_print_CodeGeneratorResponse(pb_buf_t *a, struct CodeGeneratorResponse const *m);
int pb_get_CodeGeneratorResponse(const char *p, const char *e, pb_buf_t *obj, struct CodeGeneratorResponse *m);
char *pb_encode_CodeGeneratorResponse(char *p, struct CodeGeneratorResponse const *m);
int pb_maxsz_CodeGeneratorResponse(struct CodeGeneratorResponse const *m);

char *pb_parse_CodeGeneratorResponse_File(char *p, pb_buf_t *obj, struct CodeGeneratorResponse_File *m);
int pb_print_CodeGeneratorResponse_File(pb_buf_t *a, struct CodeGeneratorResponse_File const *m);
int pb_get_CodeGeneratorResponse_File(const char *p, const char *e, pb_buf_t *obj, struct CodeGeneratorResponse_File *m);
char *pb_encode_CodeGeneratorResponse_File(char *p, struct CodeGeneratorResponse_File const *m);
int pb_maxsz_CodeGeneratorResponse_File(struct CodeGeneratorResponse_File const *m);
#ifdef __cplusplus
}
#endif
