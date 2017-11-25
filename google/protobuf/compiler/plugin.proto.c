#include "google/protobuf/compiler/plugin.proto.h"

int pb_get_Version(const char *p, const char *e, pb_buf_t *obj, struct Version *m) {
	(void) obj;
	if (pb_skipto_1(&p, e, 8)) {
		p = pb_get_u32(p + 1, e, (uint32_t*)&m->major);
	}
	if (pb_skipto_1(&p, e, 16)) {
		p = pb_get_u32(p + 1, e, (uint32_t*)&m->minor);
	}
	if (pb_skipto_1(&p, e, 24)) {
		p = pb_get_u32(p + 1, e, (uint32_t*)&m->patch);
	}
	if (pb_skipto_1(&p, e, 34)) {
		p = pb_get_string(p + 1, e, (pb_string_t*)&m->suffix);
	}
	return 0;
}
void pb_term_Version(struct Version *m) {
	if (m->suffix.buf) {
		((char*)m->suffix.buf)[m->suffix.len] = '\0';
	} else {
		m->suffix.buf = "";
	}
}

int pb_get_CodeGeneratorRequest(const char *p, const char *e, pb_buf_t *obj, struct CodeGeneratorRequest *m) {
	(void) obj;
	if (pb_skipto_1(&p, e, 10)) {
		m->file_to_generate.v = (pb_string_t*) obj->next;
		do {
			if (obj->next + m->file_to_generate.len * sizeof(pb_string_t) > obj->end) {
				return -1;
			}
			p = pb_get_string(p + 1, e, (pb_string_t*)&m->file_to_generate.v[m->file_to_generate.len]);
			m->file_to_generate.len++;
		} while (!pb_cmp_tag_1(p, e, 10));

		obj->next += m->file_to_generate.len * sizeof(pb_string_t);
	}
	if (pb_skipto_1(&p, e, 18)) {
		p = pb_get_string(p + 1, e, (pb_string_t*)&m->parameter);
	}
	if (pb_skipto_1(&p, e, 26)) {
		pb_string_t msg;
		p = pb_get_string(p + 1, e, &msg);
		m->compiler_version = (struct Version*) pb_calloc(obj, sizeof(struct Version));
		if (!m->compiler_version || pb_get_Version(msg.buf, msg.buf + msg.len, obj, (struct Version*) m->compiler_version)) {
			return -1;
		}
	}
	if (pb_skipto_1(&p, e, 122)) {
		struct FileDescriptorProto *prev = NULL;
		do {
			pb_string_t msg;
			p = pb_get_string(p + 1, e, &msg);
			struct FileDescriptorProto *c = (struct FileDescriptorProto*) pb_calloc(obj, sizeof(struct FileDescriptorProto));
			if (!c || pb_get_FileDescriptorProto(msg.buf, msg.buf + msg.len, obj, c)) {
				return -1;
			}
			m->proto_file.len++;
			c->pb_hdr.prev = prev;
			prev = c;
		} while (!pb_cmp_tag_1(p, e, 122));

		m->proto_file.v = (const struct FileDescriptorProto**) pb_calloc(obj, m->proto_file.len * sizeof(struct FileDescriptorProto*));
		if (!m->proto_file.v) {
			return -1;
		}

		for (int i = m->proto_file.len - 1; i >= 0; i--) {
			m->proto_file.v[i] = prev;
			prev = (struct FileDescriptorProto*) prev->pb_hdr.prev;
		}
	}
	return 0;
}
void pb_term_CodeGeneratorRequest(struct CodeGeneratorRequest *m) {
	for (int i = 0; i < m->file_to_generate.len; i++) {
		if (m->file_to_generate.v[i].buf) {
			((char*)m->file_to_generate.v[i].buf)[m->file_to_generate.v[i].len] = '\0';
		} else {
			((pb_string_t*)m->file_to_generate.v)[i].buf = "";
		}
	}
	if (m->parameter.buf) {
		((char*)m->parameter.buf)[m->parameter.len] = '\0';
	} else {
		m->parameter.buf = "";
	}
	if (m->compiler_version) {
		pb_term_Version((struct Version*) m->compiler_version);
	}
	for (int i = 0; i < m->proto_file.len; i++) {
		pb_term_FileDescriptorProto((struct FileDescriptorProto*) m->proto_file.v[i]);
	}
}

int pb_get_CodeGeneratorResponse(const char *p, const char *e, pb_buf_t *obj, struct CodeGeneratorResponse *m) {
	(void) obj;
	if (pb_skipto_1(&p, e, 10)) {
		p = pb_get_string(p + 1, e, (pb_string_t*)&m->error);
	}
	if (pb_skipto_1(&p, e, 122)) {
		struct CodeGeneratorResponse_File *prev = NULL;
		do {
			pb_string_t msg;
			p = pb_get_string(p + 1, e, &msg);
			struct CodeGeneratorResponse_File *c = (struct CodeGeneratorResponse_File*) pb_calloc(obj, sizeof(struct CodeGeneratorResponse_File));
			if (!c || pb_get_CodeGeneratorResponse_File(msg.buf, msg.buf + msg.len, obj, c)) {
				return -1;
			}
			m->file.len++;
			c->pb_hdr.prev = prev;
			prev = c;
		} while (!pb_cmp_tag_1(p, e, 122));

		m->file.v = (const struct CodeGeneratorResponse_File**) pb_calloc(obj, m->file.len * sizeof(struct CodeGeneratorResponse_File*));
		if (!m->file.v) {
			return -1;
		}

		for (int i = m->file.len - 1; i >= 0; i--) {
			m->file.v[i] = prev;
			prev = (struct CodeGeneratorResponse_File*) prev->pb_hdr.prev;
		}
	}
	return 0;
}
void pb_term_CodeGeneratorResponse(struct CodeGeneratorResponse *m) {
	if (m->error.buf) {
		((char*)m->error.buf)[m->error.len] = '\0';
	} else {
		m->error.buf = "";
	}
	for (int i = 0; i < m->file.len; i++) {
		pb_term_CodeGeneratorResponse_File((struct CodeGeneratorResponse_File*) m->file.v[i]);
	}
}

int pb_get_CodeGeneratorResponse_File(const char *p, const char *e, pb_buf_t *obj, struct CodeGeneratorResponse_File *m) {
	(void) obj;
	if (pb_skipto_1(&p, e, 10)) {
		p = pb_get_string(p + 1, e, (pb_string_t*)&m->name);
	}
	if (pb_skipto_1(&p, e, 18)) {
		p = pb_get_string(p + 1, e, (pb_string_t*)&m->insertion_point);
	}
	if (pb_skipto_1(&p, e, 122)) {
		p = pb_get_string(p + 1, e, (pb_string_t*)&m->content);
	}
	return 0;
}
void pb_term_CodeGeneratorResponse_File(struct CodeGeneratorResponse_File *m) {
	if (m->name.buf) {
		((char*)m->name.buf)[m->name.len] = '\0';
	} else {
		m->name.buf = "";
	}
	if (m->insertion_point.buf) {
		((char*)m->insertion_point.buf)[m->insertion_point.len] = '\0';
	} else {
		m->insertion_point.buf = "";
	}
	if (m->content.buf) {
		((char*)m->content.buf)[m->content.len] = '\0';
	} else {
		m->content.buf = "";
	}
}
