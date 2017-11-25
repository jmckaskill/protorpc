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
	pb_string_t	suffix;
};
struct CodeGeneratorRequest {
	union pb_msg pb_hdr;
	struct {int len; pb_string_t const *v;}	file_to_generate;
	pb_string_t	parameter;
	struct {int len; struct FileDescriptorProto const **v;}	proto_file;
	struct Version const*	compiler_version;
};
struct CodeGeneratorResponse_File {
	union pb_msg pb_hdr;
	pb_string_t	name;
	pb_string_t	insertion_point;
	pb_string_t	content;
};
struct CodeGeneratorResponse {
	union pb_msg pb_hdr;
	pb_string_t	error;
	struct {int len; struct CodeGeneratorResponse_File const **v;}	file;
};

int pb_get_Version(const char *p, const char *e, pb_buf_t *obj, struct Version *m);
void pb_term_Version(struct Version *m);

int pb_get_CodeGeneratorRequest(const char *p, const char *e, pb_buf_t *obj, struct CodeGeneratorRequest *m);
void pb_term_CodeGeneratorRequest(struct CodeGeneratorRequest *m);

int pb_get_CodeGeneratorResponse(const char *p, const char *e, pb_buf_t *obj, struct CodeGeneratorResponse *m);
void pb_term_CodeGeneratorResponse(struct CodeGeneratorResponse *m);

int pb_get_CodeGeneratorResponse_File(const char *p, const char *e, pb_buf_t *obj, struct CodeGeneratorResponse_File *m);
void pb_term_CodeGeneratorResponse_File(struct CodeGeneratorResponse_File *m);

#ifdef __cplusplus
}
#endif

