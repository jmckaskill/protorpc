#include "plugin.proto.h"

int pb_get_CodeGeneratorRequest(const char *p, const char *e, pb_buf_t *obj, struct CodeGeneratorRequest *m) {
	(void) obj;
	if (pb_skipto_1(&p, e, 10)) {
		m->file_to_generate.v = (struct pb_string*) obj->next;
		do {
			if (obj->next + m->file_to_generate.len * sizeof(struct pb_string) > obj->end) {
				return -1;
			}
			p = pb_get_string(p + 1, e, (struct pb_string*)&m->file_to_generate.v[m->file_to_generate.len]);
			m->file_to_generate.len++;
		} while (!pb_cmp_tag_1(p, e, 10));

		obj->next += m->file_to_generate.len * sizeof(struct pb_string);
	}
	if (pb_skipto_1(&p, e, 18)) {
		p = pb_get_string(p + 1, e, (struct pb_string*)&m->parameter);
	}
	if (pb_skipto_1(&p, e, 122)) {
		struct FileDescriptorProto *prev = NULL;
		do {
			struct pb_string msg;
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

int pb_get_CodeGeneratorResponse(const char *p, const char *e, pb_buf_t *obj, struct CodeGeneratorResponse *m) {
	(void) obj;
	if (pb_skipto_1(&p, e, 10)) {
		p = pb_get_string(p + 1, e, (struct pb_string*)&m->error);
	}
	if (pb_skipto_1(&p, e, 122)) {
		struct CodeGeneratorResponse_File *prev = NULL;
		do {
			struct pb_string msg;
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

int pb_get_CodeGeneratorResponse_File(const char *p, const char *e, pb_buf_t *obj, struct CodeGeneratorResponse_File *m) {
	(void) obj;
	if (pb_skipto_1(&p, e, 10)) {
		p = pb_get_string(p + 1, e, (struct pb_string*)&m->name);
	}
	if (pb_skipto_1(&p, e, 18)) {
		p = pb_get_string(p + 1, e, (struct pb_string*)&m->insertion_point);
	}
	if (pb_skipto_1(&p, e, 122)) {
		p = pb_get_string(p + 1, e, (struct pb_string*)&m->content);
	}
	return 0;
}
