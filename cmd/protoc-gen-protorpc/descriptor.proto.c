#include "protorpc/cmd/protoc-gen-protorpc/descriptor.proto.h"
#include <os/log.h>
#include <os/pipe.h>

char *pb_parse_FileDescriptorSet(char *p, pb_alloc_t *a, struct FileDescriptorSet *m) {
	(void) a;
	(void) m;
	if (!pb_parse_map(&p)) {
		return p;
	}
	for (;;) {
		struct pb_string key;
		switch (pb_parse_field(&p, &key, 1) % 3) {
		case 0:
			return p;
		case 2:
			if (pb_cmp(key, "file")) {
				goto unknown;
			}
			if (pb_parse_array(&p)) {
				struct FileDescriptorProto *prev = NULL;
				do {
					struct FileDescriptorProto *c = (struct FileDescriptorProto*) pb_calloc(a, 1, sizeof(struct FileDescriptorProto));
					p = pb_parse_FileDescriptorProto(p, a, c);
					m->file.len++;
					c->pb_hdr.prev = prev;
					prev = c;
				} while (pb_more_array(&p));

				m->file.v = (const struct FileDescriptorProto**) pb_calloc(a, m->file.len, sizeof(struct FileDescriptorProto*));

				for (int i = m->file.len - 1; i >= 0; i--) {
					m->file.v[i] = prev;
					prev = (struct FileDescriptorProto*) prev->pb_hdr.prev;
				}
			}
			continue;
		default:
		unknown:
			p = pb_parse_skip(p);
		}
	}
}
int pb_fparse_FileDescriptorSet(const char *fn, str_t *b, pb_alloc_t *a, struct FileDescriptorSet *m) {
	int off = b->len;
	str_read_file(b, fn);
	return pb_parse_FileDescriptorSet(b->buf + off, a, m) == pb_errret ? -1 : 0; 
}
void pb_print_FileDescriptorSet(str_t *a, struct FileDescriptorSet const *m) {
	str_grow(a, a->len + 1);
	char *p = a->buf + a->len;
	*p++ = '{';
	str_setlen(a, (unsigned) (p - a->buf));
	if (m->file.len) {
		str_add(a, "\"file\":");
		str_addch(a, '[');
		for (int i = 0; i < m->file.len; i++) {
			pb_print_FileDescriptorProto(a, m->file.v[i]);
			a->buf[a->len-1] = ','; // replace trailing newline
		}
		pb_print_array_end(a);
	}
	pb_print_map_end(a);
}
void pb_get_FileDescriptorSet(const char *p, const char *e, pb_alloc_t *obj, struct FileDescriptorSet *m) {
	(void) obj;
	if (pb_skipto_1(&p, e, 10)) {
		struct FileDescriptorProto *prev = NULL;
		do {
			struct pb_string msg;
			p = pb_get_string(p + 1, e, &msg);
			struct FileDescriptorProto *c = (struct FileDescriptorProto*) pb_calloc(obj, 1, sizeof(struct FileDescriptorProto));
			pb_get_FileDescriptorProto(msg.p, msg.p + msg.len, obj, c);
			m->file.len++;
			c->pb_hdr.prev = prev;
			prev = c;
		} while (!pb_cmp_tag_1(p, e, 10));

		m->file.v = (const struct FileDescriptorProto**) pb_calloc(obj, m->file.len, sizeof(struct FileDescriptorProto*));

		for (int i = m->file.len - 1; i >= 0; i--) {
			m->file.v[i] = prev;
			prev = (struct FileDescriptorProto*) prev->pb_hdr.prev;
		}
	}
}
char *pb_encode_FileDescriptorSet(char *p, struct FileDescriptorSet const *m) {
	if (m->file.len) {
		for (int i = 0; i < m->file.len; i++) {
			p = pb_put_tag_1(p, 10);
			int szlen = pb_u32_size(m->file.v[i]->pb_hdr.maxsz);
			char *start = p+szlen;
			p = pb_encode_FileDescriptorProto(start, m->file.v[i]);
			pb_put_finish(p, start, szlen);
		}
	}
	return p;
}
int pb_maxsz_FileDescriptorSet(struct FileDescriptorSet const *m) {
	int n = 0;
	if (m->file.len) {
		for (int i = 0; i < m->file.len; i++) {
			n += 1 + pb_maxsz_FileDescriptorProto(m->file.v[i]);
		}
	}
	n += pb_u32_size(n);
	((union pb_msg*) m)->maxsz = n;
	return n;
}

char *pb_parse_FileDescriptorProto(char *p, pb_alloc_t *a, struct FileDescriptorProto *m) {
	(void) a;
	(void) m;
	if (!pb_parse_map(&p)) {
		return p;
	}
	for (;;) {
		struct pb_string key;
		switch (pb_parse_field(&p, &key, 64570081) % 30) {
		case 0:
			return p;
		case 1:
			if (pb_cmp(key, "name")) {
				goto unknown;
			}
			p = pb_parse_string(p, &m->name);
			continue;
		case 26:
			if (pb_cmp(key, "package")) {
				goto unknown;
			}
			p = pb_parse_string(p, &m->package);
			continue;
		case 5:
			if (pb_cmp(key, "dependency")) {
				goto unknown;
			}
			p = pb_parse_array_string(p, a, &m->dependency.v, &m->dependency.len);
			continue;
		case 15:
			if (pb_cmp(key, "public_dependency")) {
				goto unknown;
			}
			p = pb_parse_array_i32(p, a, &m->public_dependency.v, &m->public_dependency.len);
			continue;
		case 4:
			if (pb_cmp(key, "weak_dependency")) {
				goto unknown;
			}
			p = pb_parse_array_i32(p, a, &m->weak_dependency.v, &m->weak_dependency.len);
			continue;
		case 10:
			if (pb_cmp(key, "message_type")) {
				goto unknown;
			}
			if (pb_parse_array(&p)) {
				struct DescriptorProto *prev = NULL;
				do {
					struct DescriptorProto *c = (struct DescriptorProto*) pb_calloc(a, 1, sizeof(struct DescriptorProto));
					p = pb_parse_DescriptorProto(p, a, c);
					m->message_type.len++;
					c->pb_hdr.prev = prev;
					prev = c;
				} while (pb_more_array(&p));

				m->message_type.v = (const struct DescriptorProto**) pb_calloc(a, m->message_type.len, sizeof(struct DescriptorProto*));

				for (int i = m->message_type.len - 1; i >= 0; i--) {
					m->message_type.v[i] = prev;
					prev = (struct DescriptorProto*) prev->pb_hdr.prev;
				}
			}
			continue;
		case 12:
			if (pb_cmp(key, "enum_type")) {
				goto unknown;
			}
			if (pb_parse_array(&p)) {
				struct EnumDescriptorProto *prev = NULL;
				do {
					struct EnumDescriptorProto *c = (struct EnumDescriptorProto*) pb_calloc(a, 1, sizeof(struct EnumDescriptorProto));
					p = pb_parse_EnumDescriptorProto(p, a, c);
					m->enum_type.len++;
					c->pb_hdr.prev = prev;
					prev = c;
				} while (pb_more_array(&p));

				m->enum_type.v = (const struct EnumDescriptorProto**) pb_calloc(a, m->enum_type.len, sizeof(struct EnumDescriptorProto*));

				for (int i = m->enum_type.len - 1; i >= 0; i--) {
					m->enum_type.v[i] = prev;
					prev = (struct EnumDescriptorProto*) prev->pb_hdr.prev;
				}
			}
			continue;
		case 19:
			if (pb_cmp(key, "service")) {
				goto unknown;
			}
			if (pb_parse_array(&p)) {
				struct ServiceDescriptorProto *prev = NULL;
				do {
					struct ServiceDescriptorProto *c = (struct ServiceDescriptorProto*) pb_calloc(a, 1, sizeof(struct ServiceDescriptorProto));
					p = pb_parse_ServiceDescriptorProto(p, a, c);
					m->service.len++;
					c->pb_hdr.prev = prev;
					prev = c;
				} while (pb_more_array(&p));

				m->service.v = (const struct ServiceDescriptorProto**) pb_calloc(a, m->service.len, sizeof(struct ServiceDescriptorProto*));

				for (int i = m->service.len - 1; i >= 0; i--) {
					m->service.v[i] = prev;
					prev = (struct ServiceDescriptorProto*) prev->pb_hdr.prev;
				}
			}
			continue;
		case 23:
			if (pb_cmp(key, "extension")) {
				goto unknown;
			}
			if (pb_parse_array(&p)) {
				struct FieldDescriptorProto *prev = NULL;
				do {
					struct FieldDescriptorProto *c = (struct FieldDescriptorProto*) pb_calloc(a, 1, sizeof(struct FieldDescriptorProto));
					p = pb_parse_FieldDescriptorProto(p, a, c);
					m->extension.len++;
					c->pb_hdr.prev = prev;
					prev = c;
				} while (pb_more_array(&p));

				m->extension.v = (const struct FieldDescriptorProto**) pb_calloc(a, m->extension.len, sizeof(struct FieldDescriptorProto*));

				for (int i = m->extension.len - 1; i >= 0; i--) {
					m->extension.v[i] = prev;
					prev = (struct FieldDescriptorProto*) prev->pb_hdr.prev;
				}
			}
			continue;
		case 2:
			if (pb_cmp(key, "options")) {
				goto unknown;
			}
			m->options = (struct FileOptions*) pb_calloc(a, 1, sizeof(struct FileOptions));
			p = pb_parse_FileOptions(p, a, (struct FileOptions*) m->options);
			continue;
		case 18:
			if (pb_cmp(key, "source_code_info")) {
				goto unknown;
			}
			m->source_code_info = (struct SourceCodeInfo*) pb_calloc(a, 1, sizeof(struct SourceCodeInfo));
			p = pb_parse_SourceCodeInfo(p, a, (struct SourceCodeInfo*) m->source_code_info);
			continue;
		case 11:
			if (pb_cmp(key, "syntax")) {
				goto unknown;
			}
			p = pb_parse_string(p, &m->syntax);
			continue;
		default:
		unknown:
			p = pb_parse_skip(p);
		}
	}
}
int pb_fparse_FileDescriptorProto(const char *fn, str_t *b, pb_alloc_t *a, struct FileDescriptorProto *m) {
	int off = b->len;
	str_read_file(b, fn);
	return pb_parse_FileDescriptorProto(b->buf + off, a, m) == pb_errret ? -1 : 0; 
}
void pb_print_FileDescriptorProto(str_t *a, struct FileDescriptorProto const *m) {
	str_grow(a, a->len + 1);
	char *p = a->buf + a->len;
	*p++ = '{';
	str_setlen(a, (unsigned) (p - a->buf));
	if (m->name.len) {
		str_add(a, "\"name\":");
		pb_print_string(a, m->name); 
		a->buf[a->len-1] = ','; // replace trailing newline
	}
	if (m->package.len) {
		str_add(a, "\"package\":");
		pb_print_string(a, m->package); 
		a->buf[a->len-1] = ','; // replace trailing newline
	}
	if (m->dependency.len) {
		str_add(a, "\"dependency\":");
		str_addch(a, '[');
		for (int i = 0; i < m->dependency.len; i++) {
			pb_print_string(a, m->dependency.v[i]);
			a->buf[a->len-1] = ','; // replace trailing newline
		}
		pb_print_array_end(a);
	}
	if (m->public_dependency.len) {
		str_add(a, "\"public_dependency\":");
		str_grow(a, a->len + 3 /*[],*/ + m->public_dependency.len * 12);
		char *mp = a->buf + a->len;
		*mp++ = '[';
		for (int i = 0; i < m->public_dependency.len; i++) {
			mp = pb_print_i32(mp, m->public_dependency.v[i]);
		}
		mp = pb_print_array_end_i(mp);
		a->len = (int) (mp - a->buf);
	}
	if (m->weak_dependency.len) {
		str_add(a, "\"weak_dependency\":");
		str_grow(a, a->len + 3 /*[],*/ + m->weak_dependency.len * 12);
		char *mp = a->buf + a->len;
		*mp++ = '[';
		for (int i = 0; i < m->weak_dependency.len; i++) {
			mp = pb_print_i32(mp, m->weak_dependency.v[i]);
		}
		mp = pb_print_array_end_i(mp);
		a->len = (int) (mp - a->buf);
	}
	if (m->message_type.len) {
		str_add(a, "\"message_type\":");
		str_addch(a, '[');
		for (int i = 0; i < m->message_type.len; i++) {
			pb_print_DescriptorProto(a, m->message_type.v[i]);
			a->buf[a->len-1] = ','; // replace trailing newline
		}
		pb_print_array_end(a);
	}
	if (m->enum_type.len) {
		str_add(a, "\"enum_type\":");
		str_addch(a, '[');
		for (int i = 0; i < m->enum_type.len; i++) {
			pb_print_EnumDescriptorProto(a, m->enum_type.v[i]);
			a->buf[a->len-1] = ','; // replace trailing newline
		}
		pb_print_array_end(a);
	}
	if (m->service.len) {
		str_add(a, "\"service\":");
		str_addch(a, '[');
		for (int i = 0; i < m->service.len; i++) {
			pb_print_ServiceDescriptorProto(a, m->service.v[i]);
			a->buf[a->len-1] = ','; // replace trailing newline
		}
		pb_print_array_end(a);
	}
	if (m->extension.len) {
		str_add(a, "\"extension\":");
		str_addch(a, '[');
		for (int i = 0; i < m->extension.len; i++) {
			pb_print_FieldDescriptorProto(a, m->extension.v[i]);
			a->buf[a->len-1] = ','; // replace trailing newline
		}
		pb_print_array_end(a);
	}
	if (m->options) {
		str_add(a, "\"options\":");
		pb_print_FileOptions(a, m->options); 
		a->buf[a->len-1] = ','; // replace trailing newline
	}
	if (m->source_code_info) {
		str_add(a, "\"source_code_info\":");
		pb_print_SourceCodeInfo(a, m->source_code_info); 
		a->buf[a->len-1] = ','; // replace trailing newline
	}
	if (m->syntax.len) {
		str_add(a, "\"syntax\":");
		pb_print_string(a, m->syntax); 
		a->buf[a->len-1] = ','; // replace trailing newline
	}
	pb_print_map_end(a);
}
void pb_get_FileDescriptorProto(const char *p, const char *e, pb_alloc_t *obj, struct FileDescriptorProto *m) {
	(void) obj;
	if (pb_skipto_1(&p, e, 10)) {
		p = pb_get_string(p + 1, e, (struct pb_string*)&m->name);
	}
	if (pb_skipto_1(&p, e, 18)) {
		p = pb_get_string(p + 1, e, (struct pb_string*)&m->package);
	}
	if (pb_skipto_1(&p, e, 26)) {
		do {
			m->dependency.v = (struct pb_string*) pb_reserve(obj, m->dependency.len, sizeof(struct pb_string));
			p = pb_get_string(p + 1, e, (struct pb_string*)&m->dependency.v[m->dependency.len]);
			m->dependency.len++;
		} while (!pb_cmp_tag_1(p, e, 26));

		pb_commit(obj, m->dependency.len, sizeof(struct pb_string));
	}
	if (pb_skipto_1(&p, e, 34)) {
		struct DescriptorProto *prev = NULL;
		do {
			struct pb_string msg;
			p = pb_get_string(p + 1, e, &msg);
			struct DescriptorProto *c = (struct DescriptorProto*) pb_calloc(obj, 1, sizeof(struct DescriptorProto));
			pb_get_DescriptorProto(msg.p, msg.p + msg.len, obj, c);
			m->message_type.len++;
			c->pb_hdr.prev = prev;
			prev = c;
		} while (!pb_cmp_tag_1(p, e, 34));

		m->message_type.v = (const struct DescriptorProto**) pb_calloc(obj, m->message_type.len, sizeof(struct DescriptorProto*));

		for (int i = m->message_type.len - 1; i >= 0; i--) {
			m->message_type.v[i] = prev;
			prev = (struct DescriptorProto*) prev->pb_hdr.prev;
		}
	}
	if (pb_skipto_1(&p, e, 42)) {
		struct EnumDescriptorProto *prev = NULL;
		do {
			struct pb_string msg;
			p = pb_get_string(p + 1, e, &msg);
			struct EnumDescriptorProto *c = (struct EnumDescriptorProto*) pb_calloc(obj, 1, sizeof(struct EnumDescriptorProto));
			pb_get_EnumDescriptorProto(msg.p, msg.p + msg.len, obj, c);
			m->enum_type.len++;
			c->pb_hdr.prev = prev;
			prev = c;
		} while (!pb_cmp_tag_1(p, e, 42));

		m->enum_type.v = (const struct EnumDescriptorProto**) pb_calloc(obj, m->enum_type.len, sizeof(struct EnumDescriptorProto*));

		for (int i = m->enum_type.len - 1; i >= 0; i--) {
			m->enum_type.v[i] = prev;
			prev = (struct EnumDescriptorProto*) prev->pb_hdr.prev;
		}
	}
	if (pb_skipto_1(&p, e, 50)) {
		struct ServiceDescriptorProto *prev = NULL;
		do {
			struct pb_string msg;
			p = pb_get_string(p + 1, e, &msg);
			struct ServiceDescriptorProto *c = (struct ServiceDescriptorProto*) pb_calloc(obj, 1, sizeof(struct ServiceDescriptorProto));
			pb_get_ServiceDescriptorProto(msg.p, msg.p + msg.len, obj, c);
			m->service.len++;
			c->pb_hdr.prev = prev;
			prev = c;
		} while (!pb_cmp_tag_1(p, e, 50));

		m->service.v = (const struct ServiceDescriptorProto**) pb_calloc(obj, m->service.len, sizeof(struct ServiceDescriptorProto*));

		for (int i = m->service.len - 1; i >= 0; i--) {
			m->service.v[i] = prev;
			prev = (struct ServiceDescriptorProto*) prev->pb_hdr.prev;
		}
	}
	if (pb_skipto_1(&p, e, 58)) {
		struct FieldDescriptorProto *prev = NULL;
		do {
			struct pb_string msg;
			p = pb_get_string(p + 1, e, &msg);
			struct FieldDescriptorProto *c = (struct FieldDescriptorProto*) pb_calloc(obj, 1, sizeof(struct FieldDescriptorProto));
			pb_get_FieldDescriptorProto(msg.p, msg.p + msg.len, obj, c);
			m->extension.len++;
			c->pb_hdr.prev = prev;
			prev = c;
		} while (!pb_cmp_tag_1(p, e, 58));

		m->extension.v = (const struct FieldDescriptorProto**) pb_calloc(obj, m->extension.len, sizeof(struct FieldDescriptorProto*));

		for (int i = m->extension.len - 1; i >= 0; i--) {
			m->extension.v[i] = prev;
			prev = (struct FieldDescriptorProto*) prev->pb_hdr.prev;
		}
	}
	if (pb_skipto_1(&p, e, 66)) {
		struct pb_string msg;
		p = pb_get_string(p + 1, e, &msg);
		m->options = (struct FileOptions*) pb_calloc(obj, 1, sizeof(struct FileOptions));
		pb_get_FileOptions(msg.p, msg.p + msg.len, obj, (struct FileOptions*) m->options);
	}
	if (pb_skipto_1(&p, e, 74)) {
		struct pb_string msg;
		p = pb_get_string(p + 1, e, &msg);
		m->source_code_info = (struct SourceCodeInfo*) pb_calloc(obj, 1, sizeof(struct SourceCodeInfo));
		pb_get_SourceCodeInfo(msg.p, msg.p + msg.len, obj, (struct SourceCodeInfo*) m->source_code_info);
	}
	if (pb_skipto_1(&p, e, 82)) {
		do {
			m->public_dependency.v = (int32_t*) pb_reserve(obj, m->public_dependency.len, sizeof(int32_t));
			p = pb_get_u32(p + 1, e, (uint32_t*)&m->public_dependency.v[m->public_dependency.len]);
			m->public_dependency.len++;
		} while (!pb_cmp_tag_1(p, e, 82));

		pb_commit(obj, m->public_dependency.len, sizeof(int32_t));
	}
	if (pb_skipto_1(&p, e, 90)) {
		do {
			m->weak_dependency.v = (int32_t*) pb_reserve(obj, m->weak_dependency.len, sizeof(int32_t));
			p = pb_get_u32(p + 1, e, (uint32_t*)&m->weak_dependency.v[m->weak_dependency.len]);
			m->weak_dependency.len++;
		} while (!pb_cmp_tag_1(p, e, 90));

		pb_commit(obj, m->weak_dependency.len, sizeof(int32_t));
	}
	if (pb_skipto_1(&p, e, 98)) {
		p = pb_get_string(p + 1, e, (struct pb_string*)&m->syntax);
	}
}
char *pb_encode_FileDescriptorProto(char *p, struct FileDescriptorProto const *m) {
	if (m->name.len) {
		p = pb_put_tag_1(p, 10);
		p = pb_put_string(p, *(struct pb_string*)& m->name);
	}
	if (m->package.len) {
		p = pb_put_tag_1(p, 18);
		p = pb_put_string(p, *(struct pb_string*)& m->package);
	}
	if (m->dependency.len) {
		for (int i = 0; i < m->dependency.len; i++) {
			p = pb_put_tag_1(p, 26);
			p = pb_put_string(p, *(struct pb_string*)& m->dependency.v[i]);
		}
	}
	if (m->message_type.len) {
		for (int i = 0; i < m->message_type.len; i++) {
			p = pb_put_tag_1(p, 34);
			int szlen = pb_u32_size(m->message_type.v[i]->pb_hdr.maxsz);
			char *start = p+szlen;
			p = pb_encode_DescriptorProto(start, m->message_type.v[i]);
			pb_put_finish(p, start, szlen);
		}
	}
	if (m->enum_type.len) {
		for (int i = 0; i < m->enum_type.len; i++) {
			p = pb_put_tag_1(p, 42);
			int szlen = pb_u32_size(m->enum_type.v[i]->pb_hdr.maxsz);
			char *start = p+szlen;
			p = pb_encode_EnumDescriptorProto(start, m->enum_type.v[i]);
			pb_put_finish(p, start, szlen);
		}
	}
	if (m->service.len) {
		for (int i = 0; i < m->service.len; i++) {
			p = pb_put_tag_1(p, 50);
			int szlen = pb_u32_size(m->service.v[i]->pb_hdr.maxsz);
			char *start = p+szlen;
			p = pb_encode_ServiceDescriptorProto(start, m->service.v[i]);
			pb_put_finish(p, start, szlen);
		}
	}
	if (m->extension.len) {
		for (int i = 0; i < m->extension.len; i++) {
			p = pb_put_tag_1(p, 58);
			int szlen = pb_u32_size(m->extension.v[i]->pb_hdr.maxsz);
			char *start = p+szlen;
			p = pb_encode_FieldDescriptorProto(start, m->extension.v[i]);
			pb_put_finish(p, start, szlen);
		}
	}
	if (m->options) {
		p = pb_put_tag_1(p, 66);
		int szlen = pb_u32_size(m->options->pb_hdr.maxsz);
		char *start = p+szlen;
		p = pb_encode_FileOptions(start, m->options);
		pb_put_finish(p, start, szlen);
	}
	if (m->source_code_info) {
		p = pb_put_tag_1(p, 74);
		int szlen = pb_u32_size(m->source_code_info->pb_hdr.maxsz);
		char *start = p+szlen;
		p = pb_encode_SourceCodeInfo(start, m->source_code_info);
		pb_put_finish(p, start, szlen);
	}
	if (m->public_dependency.len) {
		for (int i = 0; i < m->public_dependency.len; i++) {
			p = pb_put_tag_1(p, 82);
			p = pb_put_u32(p, *(uint32_t*)& m->public_dependency.v[i]);
		}
	}
	if (m->weak_dependency.len) {
		for (int i = 0; i < m->weak_dependency.len; i++) {
			p = pb_put_tag_1(p, 90);
			p = pb_put_u32(p, *(uint32_t*)& m->weak_dependency.v[i]);
		}
	}
	if (m->syntax.len) {
		p = pb_put_tag_1(p, 98);
		p = pb_put_string(p, *(struct pb_string*)& m->syntax);
	}
	return p;
}
int pb_maxsz_FileDescriptorProto(struct FileDescriptorProto const *m) {
	int n = 0;
	if (m->name.len) {
		n += 1 + 5 + m->name.len;
	}
	if (m->package.len) {
		n += 1 + 5 + m->package.len;
	}
	if (m->dependency.len) {
		for (int i = 0; i < m->dependency.len; i++) {
			n += 1 + 5 + m->dependency.v[i].len;
		}
	}
	if (m->message_type.len) {
		for (int i = 0; i < m->message_type.len; i++) {
			n += 1 + pb_maxsz_DescriptorProto(m->message_type.v[i]);
		}
	}
	if (m->enum_type.len) {
		for (int i = 0; i < m->enum_type.len; i++) {
			n += 1 + pb_maxsz_EnumDescriptorProto(m->enum_type.v[i]);
		}
	}
	if (m->service.len) {
		for (int i = 0; i < m->service.len; i++) {
			n += 1 + pb_maxsz_ServiceDescriptorProto(m->service.v[i]);
		}
	}
	if (m->extension.len) {
		for (int i = 0; i < m->extension.len; i++) {
			n += 1 + pb_maxsz_FieldDescriptorProto(m->extension.v[i]);
		}
	}
	if (m->options) {
		n += 1 + pb_maxsz_FileOptions(m->options);
	}
	if (m->source_code_info) {
		n += 1 + pb_maxsz_SourceCodeInfo(m->source_code_info);
	}
	if (m->public_dependency.len) {
		n += (1 + 5) * m->public_dependency.len;
	}
	if (m->weak_dependency.len) {
		n += (1 + 5) * m->weak_dependency.len;
	}
	if (m->syntax.len) {
		n += 1 + 5 + m->syntax.len;
	}
	n += pb_u32_size(n);
	((union pb_msg*) m)->maxsz = n;
	return n;
}

char *pb_parse_DescriptorProto(char *p, pb_alloc_t *a, struct DescriptorProto *m) {
	(void) a;
	(void) m;
	if (!pb_parse_map(&p)) {
		return p;
	}
	for (;;) {
		struct pb_string key;
		switch (pb_parse_field(&p, &key, 3917561844) % 37) {
		case 0:
			return p;
		case 27:
			if (pb_cmp(key, "name")) {
				goto unknown;
			}
			p = pb_parse_string(p, &m->name);
			continue;
		case 15:
			if (pb_cmp(key, "field")) {
				goto unknown;
			}
			if (pb_parse_array(&p)) {
				struct FieldDescriptorProto *prev = NULL;
				do {
					struct FieldDescriptorProto *c = (struct FieldDescriptorProto*) pb_calloc(a, 1, sizeof(struct FieldDescriptorProto));
					p = pb_parse_FieldDescriptorProto(p, a, c);
					m->field.len++;
					c->pb_hdr.prev = prev;
					prev = c;
				} while (pb_more_array(&p));

				m->field.v = (const struct FieldDescriptorProto**) pb_calloc(a, m->field.len, sizeof(struct FieldDescriptorProto*));

				for (int i = m->field.len - 1; i >= 0; i--) {
					m->field.v[i] = prev;
					prev = (struct FieldDescriptorProto*) prev->pb_hdr.prev;
				}
			}
			continue;
		case 16:
			if (pb_cmp(key, "extension")) {
				goto unknown;
			}
			if (pb_parse_array(&p)) {
				struct FieldDescriptorProto *prev = NULL;
				do {
					struct FieldDescriptorProto *c = (struct FieldDescriptorProto*) pb_calloc(a, 1, sizeof(struct FieldDescriptorProto));
					p = pb_parse_FieldDescriptorProto(p, a, c);
					m->extension.len++;
					c->pb_hdr.prev = prev;
					prev = c;
				} while (pb_more_array(&p));

				m->extension.v = (const struct FieldDescriptorProto**) pb_calloc(a, m->extension.len, sizeof(struct FieldDescriptorProto*));

				for (int i = m->extension.len - 1; i >= 0; i--) {
					m->extension.v[i] = prev;
					prev = (struct FieldDescriptorProto*) prev->pb_hdr.prev;
				}
			}
			continue;
		case 31:
			if (pb_cmp(key, "nested_type")) {
				goto unknown;
			}
			if (pb_parse_array(&p)) {
				struct DescriptorProto *prev = NULL;
				do {
					struct DescriptorProto *c = (struct DescriptorProto*) pb_calloc(a, 1, sizeof(struct DescriptorProto));
					p = pb_parse_DescriptorProto(p, a, c);
					m->nested_type.len++;
					c->pb_hdr.prev = prev;
					prev = c;
				} while (pb_more_array(&p));

				m->nested_type.v = (const struct DescriptorProto**) pb_calloc(a, m->nested_type.len, sizeof(struct DescriptorProto*));

				for (int i = m->nested_type.len - 1; i >= 0; i--) {
					m->nested_type.v[i] = prev;
					prev = (struct DescriptorProto*) prev->pb_hdr.prev;
				}
			}
			continue;
		case 2:
			if (pb_cmp(key, "enum_type")) {
				goto unknown;
			}
			if (pb_parse_array(&p)) {
				struct EnumDescriptorProto *prev = NULL;
				do {
					struct EnumDescriptorProto *c = (struct EnumDescriptorProto*) pb_calloc(a, 1, sizeof(struct EnumDescriptorProto));
					p = pb_parse_EnumDescriptorProto(p, a, c);
					m->enum_type.len++;
					c->pb_hdr.prev = prev;
					prev = c;
				} while (pb_more_array(&p));

				m->enum_type.v = (const struct EnumDescriptorProto**) pb_calloc(a, m->enum_type.len, sizeof(struct EnumDescriptorProto*));

				for (int i = m->enum_type.len - 1; i >= 0; i--) {
					m->enum_type.v[i] = prev;
					prev = (struct EnumDescriptorProto*) prev->pb_hdr.prev;
				}
			}
			continue;
		case 22:
			if (pb_cmp(key, "extension_range")) {
				goto unknown;
			}
			if (pb_parse_array(&p)) {
				int n = 0;
				do {
					m->extension_range.v = (struct DescriptorProto_ExtensionRange*) pb_reserve(a, n, sizeof(struct DescriptorProto_ExtensionRange));
					p = pb_parse_DescriptorProto_ExtensionRange(p, (struct DescriptorProto_ExtensionRange*) &m->extension_range.v[n++]); 
				} while (pb_more_array(&p));
				m->extension_range.len = n;
			}
			continue;
		case 34:
			if (pb_cmp(key, "oneof_decl")) {
				goto unknown;
			}
			if (pb_parse_array(&p)) {
				struct OneofDescriptorProto *prev = NULL;
				do {
					struct OneofDescriptorProto *c = (struct OneofDescriptorProto*) pb_calloc(a, 1, sizeof(struct OneofDescriptorProto));
					p = pb_parse_OneofDescriptorProto(p, a, c);
					m->oneof_decl.len++;
					c->pb_hdr.prev = prev;
					prev = c;
				} while (pb_more_array(&p));

				m->oneof_decl.v = (const struct OneofDescriptorProto**) pb_calloc(a, m->oneof_decl.len, sizeof(struct OneofDescriptorProto*));

				for (int i = m->oneof_decl.len - 1; i >= 0; i--) {
					m->oneof_decl.v[i] = prev;
					prev = (struct OneofDescriptorProto*) prev->pb_hdr.prev;
				}
			}
			continue;
		case 18:
			if (pb_cmp(key, "options")) {
				goto unknown;
			}
			m->options = (struct MessageOptions*) pb_calloc(a, 1, sizeof(struct MessageOptions));
			p = pb_parse_MessageOptions(p, a, (struct MessageOptions*) m->options);
			continue;
		case 5:
			if (pb_cmp(key, "reserved_range")) {
				goto unknown;
			}
			if (pb_parse_array(&p)) {
				int n = 0;
				do {
					m->reserved_range.v = (struct DescriptorProto_ReservedRange*) pb_reserve(a, n, sizeof(struct DescriptorProto_ReservedRange));
					p = pb_parse_DescriptorProto_ReservedRange(p, (struct DescriptorProto_ReservedRange*) &m->reserved_range.v[n++]); 
				} while (pb_more_array(&p));
				m->reserved_range.len = n;
			}
			continue;
		case 3:
			if (pb_cmp(key, "reserved_name")) {
				goto unknown;
			}
			p = pb_parse_array_string(p, a, &m->reserved_name.v, &m->reserved_name.len);
			continue;
		default:
		unknown:
			p = pb_parse_skip(p);
		}
	}
}
int pb_fparse_DescriptorProto(const char *fn, str_t *b, pb_alloc_t *a, struct DescriptorProto *m) {
	int off = b->len;
	str_read_file(b, fn);
	return pb_parse_DescriptorProto(b->buf + off, a, m) == pb_errret ? -1 : 0; 
}
void pb_print_DescriptorProto(str_t *a, struct DescriptorProto const *m) {
	str_grow(a, a->len + 1);
	char *p = a->buf + a->len;
	*p++ = '{';
	str_setlen(a, (unsigned) (p - a->buf));
	if (m->name.len) {
		str_add(a, "\"name\":");
		pb_print_string(a, m->name); 
		a->buf[a->len-1] = ','; // replace trailing newline
	}
	if (m->field.len) {
		str_add(a, "\"field\":");
		str_addch(a, '[');
		for (int i = 0; i < m->field.len; i++) {
			pb_print_FieldDescriptorProto(a, m->field.v[i]);
			a->buf[a->len-1] = ','; // replace trailing newline
		}
		pb_print_array_end(a);
	}
	if (m->extension.len) {
		str_add(a, "\"extension\":");
		str_addch(a, '[');
		for (int i = 0; i < m->extension.len; i++) {
			pb_print_FieldDescriptorProto(a, m->extension.v[i]);
			a->buf[a->len-1] = ','; // replace trailing newline
		}
		pb_print_array_end(a);
	}
	if (m->nested_type.len) {
		str_add(a, "\"nested_type\":");
		str_addch(a, '[');
		for (int i = 0; i < m->nested_type.len; i++) {
			pb_print_DescriptorProto(a, m->nested_type.v[i]);
			a->buf[a->len-1] = ','; // replace trailing newline
		}
		pb_print_array_end(a);
	}
	if (m->enum_type.len) {
		str_add(a, "\"enum_type\":");
		str_addch(a, '[');
		for (int i = 0; i < m->enum_type.len; i++) {
			pb_print_EnumDescriptorProto(a, m->enum_type.v[i]);
			a->buf[a->len-1] = ','; // replace trailing newline
		}
		pb_print_array_end(a);
	}
	if (m->extension_range.len) {
		str_add(a, "\"extension_range\":");
		str_addch(a, '[');
		for (int i = 0; i < m->extension_range.len; i++) {
			pb_print_DescriptorProto_ExtensionRange(a, &m->extension_range.v[i]);
			a->buf[a->len-1] = ','; // replace trailing newline
		}
		pb_print_array_end(a);
	}
	if (m->oneof_decl.len) {
		str_add(a, "\"oneof_decl\":");
		str_addch(a, '[');
		for (int i = 0; i < m->oneof_decl.len; i++) {
			pb_print_OneofDescriptorProto(a, m->oneof_decl.v[i]);
			a->buf[a->len-1] = ','; // replace trailing newline
		}
		pb_print_array_end(a);
	}
	if (m->options) {
		str_add(a, "\"options\":");
		pb_print_MessageOptions(a, m->options); 
		a->buf[a->len-1] = ','; // replace trailing newline
	}
	if (m->reserved_range.len) {
		str_add(a, "\"reserved_range\":");
		str_addch(a, '[');
		for (int i = 0; i < m->reserved_range.len; i++) {
			pb_print_DescriptorProto_ReservedRange(a, &m->reserved_range.v[i]);
			a->buf[a->len-1] = ','; // replace trailing newline
		}
		pb_print_array_end(a);
	}
	if (m->reserved_name.len) {
		str_add(a, "\"reserved_name\":");
		str_addch(a, '[');
		for (int i = 0; i < m->reserved_name.len; i++) {
			pb_print_string(a, m->reserved_name.v[i]);
			a->buf[a->len-1] = ','; // replace trailing newline
		}
		pb_print_array_end(a);
	}
	pb_print_map_end(a);
}
void pb_get_DescriptorProto(const char *p, const char *e, pb_alloc_t *obj, struct DescriptorProto *m) {
	(void) obj;
	if (pb_skipto_1(&p, e, 10)) {
		p = pb_get_string(p + 1, e, (struct pb_string*)&m->name);
	}
	if (pb_skipto_1(&p, e, 18)) {
		struct FieldDescriptorProto *prev = NULL;
		do {
			struct pb_string msg;
			p = pb_get_string(p + 1, e, &msg);
			struct FieldDescriptorProto *c = (struct FieldDescriptorProto*) pb_calloc(obj, 1, sizeof(struct FieldDescriptorProto));
			pb_get_FieldDescriptorProto(msg.p, msg.p + msg.len, obj, c);
			m->field.len++;
			c->pb_hdr.prev = prev;
			prev = c;
		} while (!pb_cmp_tag_1(p, e, 18));

		m->field.v = (const struct FieldDescriptorProto**) pb_calloc(obj, m->field.len, sizeof(struct FieldDescriptorProto*));

		for (int i = m->field.len - 1; i >= 0; i--) {
			m->field.v[i] = prev;
			prev = (struct FieldDescriptorProto*) prev->pb_hdr.prev;
		}
	}
	if (pb_skipto_1(&p, e, 26)) {
		struct DescriptorProto *prev = NULL;
		do {
			struct pb_string msg;
			p = pb_get_string(p + 1, e, &msg);
			struct DescriptorProto *c = (struct DescriptorProto*) pb_calloc(obj, 1, sizeof(struct DescriptorProto));
			pb_get_DescriptorProto(msg.p, msg.p + msg.len, obj, c);
			m->nested_type.len++;
			c->pb_hdr.prev = prev;
			prev = c;
		} while (!pb_cmp_tag_1(p, e, 26));

		m->nested_type.v = (const struct DescriptorProto**) pb_calloc(obj, m->nested_type.len, sizeof(struct DescriptorProto*));

		for (int i = m->nested_type.len - 1; i >= 0; i--) {
			m->nested_type.v[i] = prev;
			prev = (struct DescriptorProto*) prev->pb_hdr.prev;
		}
	}
	if (pb_skipto_1(&p, e, 34)) {
		struct EnumDescriptorProto *prev = NULL;
		do {
			struct pb_string msg;
			p = pb_get_string(p + 1, e, &msg);
			struct EnumDescriptorProto *c = (struct EnumDescriptorProto*) pb_calloc(obj, 1, sizeof(struct EnumDescriptorProto));
			pb_get_EnumDescriptorProto(msg.p, msg.p + msg.len, obj, c);
			m->enum_type.len++;
			c->pb_hdr.prev = prev;
			prev = c;
		} while (!pb_cmp_tag_1(p, e, 34));

		m->enum_type.v = (const struct EnumDescriptorProto**) pb_calloc(obj, m->enum_type.len, sizeof(struct EnumDescriptorProto*));

		for (int i = m->enum_type.len - 1; i >= 0; i--) {
			m->enum_type.v[i] = prev;
			prev = (struct EnumDescriptorProto*) prev->pb_hdr.prev;
		}
	}
	if (pb_skipto_1(&p, e, 42)) {
		do {
			m->extension_range.v = (struct DescriptorProto_ExtensionRange*) pb_reserve(obj, m->extension_range.len, sizeof(struct DescriptorProto_ExtensionRange));
			struct pb_string msg;
			p = pb_get_string(p + 1, e, &msg);
			pb_get_DescriptorProto_ExtensionRange(msg.p, msg.p + msg.len, (struct DescriptorProto_ExtensionRange*) &m->extension_range.v[m->extension_range.len]);
			m->extension_range.len++;
		} while (!pb_cmp_tag_1(p, e, 42));

		pb_commit(obj, m->extension_range.len, sizeof(struct DescriptorProto_ExtensionRange));
	}
	if (pb_skipto_1(&p, e, 50)) {
		struct FieldDescriptorProto *prev = NULL;
		do {
			struct pb_string msg;
			p = pb_get_string(p + 1, e, &msg);
			struct FieldDescriptorProto *c = (struct FieldDescriptorProto*) pb_calloc(obj, 1, sizeof(struct FieldDescriptorProto));
			pb_get_FieldDescriptorProto(msg.p, msg.p + msg.len, obj, c);
			m->extension.len++;
			c->pb_hdr.prev = prev;
			prev = c;
		} while (!pb_cmp_tag_1(p, e, 50));

		m->extension.v = (const struct FieldDescriptorProto**) pb_calloc(obj, m->extension.len, sizeof(struct FieldDescriptorProto*));

		for (int i = m->extension.len - 1; i >= 0; i--) {
			m->extension.v[i] = prev;
			prev = (struct FieldDescriptorProto*) prev->pb_hdr.prev;
		}
	}
	if (pb_skipto_1(&p, e, 58)) {
		struct pb_string msg;
		p = pb_get_string(p + 1, e, &msg);
		m->options = (struct MessageOptions*) pb_calloc(obj, 1, sizeof(struct MessageOptions));
		pb_get_MessageOptions(msg.p, msg.p + msg.len, obj, (struct MessageOptions*) m->options);
	}
	if (pb_skipto_1(&p, e, 66)) {
		struct OneofDescriptorProto *prev = NULL;
		do {
			struct pb_string msg;
			p = pb_get_string(p + 1, e, &msg);
			struct OneofDescriptorProto *c = (struct OneofDescriptorProto*) pb_calloc(obj, 1, sizeof(struct OneofDescriptorProto));
			pb_get_OneofDescriptorProto(msg.p, msg.p + msg.len, obj, c);
			m->oneof_decl.len++;
			c->pb_hdr.prev = prev;
			prev = c;
		} while (!pb_cmp_tag_1(p, e, 66));

		m->oneof_decl.v = (const struct OneofDescriptorProto**) pb_calloc(obj, m->oneof_decl.len, sizeof(struct OneofDescriptorProto*));

		for (int i = m->oneof_decl.len - 1; i >= 0; i--) {
			m->oneof_decl.v[i] = prev;
			prev = (struct OneofDescriptorProto*) prev->pb_hdr.prev;
		}
	}
	if (pb_skipto_1(&p, e, 74)) {
		do {
			m->reserved_range.v = (struct DescriptorProto_ReservedRange*) pb_reserve(obj, m->reserved_range.len, sizeof(struct DescriptorProto_ReservedRange));
			struct pb_string msg;
			p = pb_get_string(p + 1, e, &msg);
			pb_get_DescriptorProto_ReservedRange(msg.p, msg.p + msg.len, (struct DescriptorProto_ReservedRange*) &m->reserved_range.v[m->reserved_range.len]);
			m->reserved_range.len++;
		} while (!pb_cmp_tag_1(p, e, 74));

		pb_commit(obj, m->reserved_range.len, sizeof(struct DescriptorProto_ReservedRange));
	}
	if (pb_skipto_1(&p, e, 82)) {
		do {
			m->reserved_name.v = (struct pb_string*) pb_reserve(obj, m->reserved_name.len, sizeof(struct pb_string));
			p = pb_get_string(p + 1, e, (struct pb_string*)&m->reserved_name.v[m->reserved_name.len]);
			m->reserved_name.len++;
		} while (!pb_cmp_tag_1(p, e, 82));

		pb_commit(obj, m->reserved_name.len, sizeof(struct pb_string));
	}
}
char *pb_encode_DescriptorProto(char *p, struct DescriptorProto const *m) {
	if (m->name.len) {
		p = pb_put_tag_1(p, 10);
		p = pb_put_string(p, *(struct pb_string*)& m->name);
	}
	if (m->field.len) {
		for (int i = 0; i < m->field.len; i++) {
			p = pb_put_tag_1(p, 18);
			int szlen = pb_u32_size(m->field.v[i]->pb_hdr.maxsz);
			char *start = p+szlen;
			p = pb_encode_FieldDescriptorProto(start, m->field.v[i]);
			pb_put_finish(p, start, szlen);
		}
	}
	if (m->nested_type.len) {
		for (int i = 0; i < m->nested_type.len; i++) {
			p = pb_put_tag_1(p, 26);
			int szlen = pb_u32_size(m->nested_type.v[i]->pb_hdr.maxsz);
			char *start = p+szlen;
			p = pb_encode_DescriptorProto(start, m->nested_type.v[i]);
			pb_put_finish(p, start, szlen);
		}
	}
	if (m->enum_type.len) {
		for (int i = 0; i < m->enum_type.len; i++) {
			p = pb_put_tag_1(p, 34);
			int szlen = pb_u32_size(m->enum_type.v[i]->pb_hdr.maxsz);
			char *start = p+szlen;
			p = pb_encode_EnumDescriptorProto(start, m->enum_type.v[i]);
			pb_put_finish(p, start, szlen);
		}
	}
	if (m->extension_range.len) {
		for (int i = 0; i < m->extension_range.len; i++) {
			p = pb_put_tag_1(p, 42);
			char *start = p+1;
			p = pb_encode_DescriptorProto_ExtensionRange(start, &m->extension_range.v[i]);
			start[-1] = (char) (p - start);
		}
	}
	if (m->extension.len) {
		for (int i = 0; i < m->extension.len; i++) {
			p = pb_put_tag_1(p, 50);
			int szlen = pb_u32_size(m->extension.v[i]->pb_hdr.maxsz);
			char *start = p+szlen;
			p = pb_encode_FieldDescriptorProto(start, m->extension.v[i]);
			pb_put_finish(p, start, szlen);
		}
	}
	if (m->options) {
		p = pb_put_tag_1(p, 58);
		int szlen = pb_u32_size(m->options->pb_hdr.maxsz);
		char *start = p+szlen;
		p = pb_encode_MessageOptions(start, m->options);
		pb_put_finish(p, start, szlen);
	}
	if (m->oneof_decl.len) {
		for (int i = 0; i < m->oneof_decl.len; i++) {
			p = pb_put_tag_1(p, 66);
			int szlen = pb_u32_size(m->oneof_decl.v[i]->pb_hdr.maxsz);
			char *start = p+szlen;
			p = pb_encode_OneofDescriptorProto(start, m->oneof_decl.v[i]);
			pb_put_finish(p, start, szlen);
		}
	}
	if (m->reserved_range.len) {
		for (int i = 0; i < m->reserved_range.len; i++) {
			p = pb_put_tag_1(p, 74);
			char *start = p+1;
			p = pb_encode_DescriptorProto_ReservedRange(start, &m->reserved_range.v[i]);
			start[-1] = (char) (p - start);
		}
	}
	if (m->reserved_name.len) {
		for (int i = 0; i < m->reserved_name.len; i++) {
			p = pb_put_tag_1(p, 82);
			p = pb_put_string(p, *(struct pb_string*)& m->reserved_name.v[i]);
		}
	}
	return p;
}
int pb_maxsz_DescriptorProto(struct DescriptorProto const *m) {
	int n = 0;
	if (m->name.len) {
		n += 1 + 5 + m->name.len;
	}
	if (m->field.len) {
		for (int i = 0; i < m->field.len; i++) {
			n += 1 + pb_maxsz_FieldDescriptorProto(m->field.v[i]);
		}
	}
	if (m->nested_type.len) {
		for (int i = 0; i < m->nested_type.len; i++) {
			n += 1 + pb_maxsz_DescriptorProto(m->nested_type.v[i]);
		}
	}
	if (m->enum_type.len) {
		for (int i = 0; i < m->enum_type.len; i++) {
			n += 1 + pb_maxsz_EnumDescriptorProto(m->enum_type.v[i]);
		}
	}
	if (m->extension_range.len) {
		n += (1 + 13) * m->extension_range.len;
	}
	if (m->extension.len) {
		for (int i = 0; i < m->extension.len; i++) {
			n += 1 + pb_maxsz_FieldDescriptorProto(m->extension.v[i]);
		}
	}
	if (m->options) {
		n += 1 + pb_maxsz_MessageOptions(m->options);
	}
	if (m->oneof_decl.len) {
		for (int i = 0; i < m->oneof_decl.len; i++) {
			n += 1 + pb_maxsz_OneofDescriptorProto(m->oneof_decl.v[i]);
		}
	}
	if (m->reserved_range.len) {
		n += (1 + 13) * m->reserved_range.len;
	}
	if (m->reserved_name.len) {
		for (int i = 0; i < m->reserved_name.len; i++) {
			n += 1 + 5 + m->reserved_name.v[i].len;
		}
	}
	n += pb_u32_size(n);
	((union pb_msg*) m)->maxsz = n;
	return n;
}

bool pb_nonzero_DescriptorProto_ExtensionRange(struct DescriptorProto_ExtensionRange const *m) {
	return false
	    || m->start
	    || m->end;
}
char *pb_parse_DescriptorProto_ExtensionRange(char *p, struct DescriptorProto_ExtensionRange *m) {
	(void) m;
	if (!pb_parse_map(&p)) {
		return p;
	}
	for (;;) {
		struct pb_string key;
		switch (pb_parse_field(&p, &key, 1) % 4) {
		case 0:
			return p;
		case 2:
			if (pb_cmp(key, "start")) {
				goto unknown;
			}
			p = pb_parse_i32(p, &m->start);
			continue;
		case 3:
			if (pb_cmp(key, "end")) {
				goto unknown;
			}
			p = pb_parse_i32(p, &m->end);
			continue;
		default:
		unknown:
			p = pb_parse_skip(p);
		}
	}
}
int pb_fparse_DescriptorProto_ExtensionRange(const char *fn, str_t *b, struct DescriptorProto_ExtensionRange *m) {
	int off = b->len;
	str_read_file(b, fn);
	return pb_parse_DescriptorProto_ExtensionRange(b->buf + off, m) == pb_errret ? -1 : 0; 
}
void pb_print_DescriptorProto_ExtensionRange(str_t *a, struct DescriptorProto_ExtensionRange const *m) {
	str_grow(a, a->len + 39);
	char *p = a->buf + a->len;
	*p++ = '{';
	if (m->start) {
		memcpy(p, "\"start\":", 8);
		p += 8;
		p = pb_print_i32(p, m->start);
	}
	if (m->end) {
		memcpy(p, "\"end\":", 6);
		p += 6;
		p = pb_print_i32(p, m->end);
	}
	str_setlen(a, (unsigned) (p - a->buf));
	pb_print_map_end(a);
}
void pb_get_DescriptorProto_ExtensionRange(const char *p, const char *e, struct DescriptorProto_ExtensionRange *m) {
	if (pb_skipto_1(&p, e, 8)) {
		p = pb_get_u32(p + 1, e, (uint32_t*)&m->start);
	}
	if (pb_skipto_1(&p, e, 16)) {
		p = pb_get_u32(p + 1, e, (uint32_t*)&m->end);
	}
}
char *pb_encode_DescriptorProto_ExtensionRange(char *p, struct DescriptorProto_ExtensionRange const *m) {
	if (m->start) {
		p = pb_put_tag_1(p, 8);
		p = pb_put_u32(p, *(uint32_t*)& m->start);
	}
	if (m->end) {
		p = pb_put_tag_1(p, 16);
		p = pb_put_u32(p, *(uint32_t*)& m->end);
	}
	return p;
}
int pb_maxsz_DescriptorProto_ExtensionRange(struct DescriptorProto_ExtensionRange const *m) {
	int n = 0;
	if (m->start) {
		n += 1 + 5;
	}
	if (m->end) {
		n += 1 + 5;
	}
	n += pb_u32_size(n);
	((union pb_msg*) m)->maxsz = n;
	return n;
}

bool pb_nonzero_DescriptorProto_ReservedRange(struct DescriptorProto_ReservedRange const *m) {
	return false
	    || m->start
	    || m->end;
}
char *pb_parse_DescriptorProto_ReservedRange(char *p, struct DescriptorProto_ReservedRange *m) {
	(void) m;
	if (!pb_parse_map(&p)) {
		return p;
	}
	for (;;) {
		struct pb_string key;
		switch (pb_parse_field(&p, &key, 1) % 4) {
		case 0:
			return p;
		case 2:
			if (pb_cmp(key, "start")) {
				goto unknown;
			}
			p = pb_parse_i32(p, &m->start);
			continue;
		case 3:
			if (pb_cmp(key, "end")) {
				goto unknown;
			}
			p = pb_parse_i32(p, &m->end);
			continue;
		default:
		unknown:
			p = pb_parse_skip(p);
		}
	}
}
int pb_fparse_DescriptorProto_ReservedRange(const char *fn, str_t *b, struct DescriptorProto_ReservedRange *m) {
	int off = b->len;
	str_read_file(b, fn);
	return pb_parse_DescriptorProto_ReservedRange(b->buf + off, m) == pb_errret ? -1 : 0; 
}
void pb_print_DescriptorProto_ReservedRange(str_t *a, struct DescriptorProto_ReservedRange const *m) {
	str_grow(a, a->len + 39);
	char *p = a->buf + a->len;
	*p++ = '{';
	if (m->start) {
		memcpy(p, "\"start\":", 8);
		p += 8;
		p = pb_print_i32(p, m->start);
	}
	if (m->end) {
		memcpy(p, "\"end\":", 6);
		p += 6;
		p = pb_print_i32(p, m->end);
	}
	str_setlen(a, (unsigned) (p - a->buf));
	pb_print_map_end(a);
}
void pb_get_DescriptorProto_ReservedRange(const char *p, const char *e, struct DescriptorProto_ReservedRange *m) {
	if (pb_skipto_1(&p, e, 8)) {
		p = pb_get_u32(p + 1, e, (uint32_t*)&m->start);
	}
	if (pb_skipto_1(&p, e, 16)) {
		p = pb_get_u32(p + 1, e, (uint32_t*)&m->end);
	}
}
char *pb_encode_DescriptorProto_ReservedRange(char *p, struct DescriptorProto_ReservedRange const *m) {
	if (m->start) {
		p = pb_put_tag_1(p, 8);
		p = pb_put_u32(p, *(uint32_t*)& m->start);
	}
	if (m->end) {
		p = pb_put_tag_1(p, 16);
		p = pb_put_u32(p, *(uint32_t*)& m->end);
	}
	return p;
}
int pb_maxsz_DescriptorProto_ReservedRange(struct DescriptorProto_ReservedRange const *m) {
	int n = 0;
	if (m->start) {
		n += 1 + 5;
	}
	if (m->end) {
		n += 1 + 5;
	}
	n += pb_u32_size(n);
	((union pb_msg*) m)->maxsz = n;
	return n;
}

char *pb_parse_FieldDescriptorProto(char *p, pb_alloc_t *a, struct FieldDescriptorProto *m) {
	(void) a;
	(void) m;
	if (!pb_parse_map(&p)) {
		return p;
	}
	for (;;) {
		struct pb_string key;
		switch (pb_parse_field(&p, &key, 3162750941) % 38) {
		case 0:
			return p;
		case 11:
			if (pb_cmp(key, "name")) {
				goto unknown;
			}
			p = pb_parse_string(p, &m->name);
			continue;
		case 13:
			if (pb_cmp(key, "number")) {
				goto unknown;
			}
			p = pb_parse_i32(p, &m->number);
			continue;
		case 20:
			if (pb_cmp(key, "label")) {
				goto unknown;
			}
			p = pb_parse_FieldDescriptorProto_Label(p, &m->label);
			continue;
		case 32:
			if (pb_cmp(key, "type")) {
				goto unknown;
			}
			p = pb_parse_FieldDescriptorProto_Type(p, &m->type);
			continue;
		case 26:
			if (pb_cmp(key, "type_name")) {
				goto unknown;
			}
			p = pb_parse_string(p, &m->type_name);
			continue;
		case 6:
			if (pb_cmp(key, "extendee")) {
				goto unknown;
			}
			p = pb_parse_string(p, &m->extendee);
			continue;
		case 7:
			if (pb_cmp(key, "default_value")) {
				goto unknown;
			}
			p = pb_parse_string(p, &m->default_value);
			continue;
		case 36:
			if (pb_cmp(key, "oneof_index")) {
				goto unknown;
			}
			p = pb_parse_i32(p, &m->oneof_index);
			continue;
		case 34:
			if (pb_cmp(key, "json_name")) {
				goto unknown;
			}
			p = pb_parse_string(p, &m->json_name);
			continue;
		case 18:
			if (pb_cmp(key, "options")) {
				goto unknown;
			}
			m->options = (struct FieldOptions*) pb_calloc(a, 1, sizeof(struct FieldOptions));
			p = pb_parse_FieldOptions(p, a, (struct FieldOptions*) m->options);
			continue;
		default:
		unknown:
			p = pb_parse_skip(p);
		}
	}
}
int pb_fparse_FieldDescriptorProto(const char *fn, str_t *b, pb_alloc_t *a, struct FieldDescriptorProto *m) {
	int off = b->len;
	str_read_file(b, fn);
	return pb_parse_FieldDescriptorProto(b->buf + off, a, m) == pb_errret ? -1 : 0; 
}
void pb_print_FieldDescriptorProto(str_t *a, struct FieldDescriptorProto const *m) {
	str_grow(a, a->len + 98);
	char *p = a->buf + a->len;
	*p++ = '{';
	if (m->number) {
		memcpy(p, "\"number\":", 9);
		p += 9;
		p = pb_print_i32(p, m->number);
	}
	if (m->label) {
		memcpy(p, "\"label\":", 8);
		p += 8;
		p = pb_print_FieldDescriptorProto_Label(p, m->label);
	}
	if (m->type) {
		memcpy(p, "\"type\":", 7);
		p += 7;
		p = pb_print_FieldDescriptorProto_Type(p, m->type);
	}
	if (m->oneof_index) {
		memcpy(p, "\"oneof_index\":", 14);
		p += 14;
		p = pb_print_i32(p, m->oneof_index);
	}
	str_setlen(a, (unsigned) (p - a->buf));
	if (m->name.len) {
		str_add(a, "\"name\":");
		pb_print_string(a, m->name); 
		a->buf[a->len-1] = ','; // replace trailing newline
	}
	if (m->type_name.len) {
		str_add(a, "\"type_name\":");
		pb_print_string(a, m->type_name); 
		a->buf[a->len-1] = ','; // replace trailing newline
	}
	if (m->extendee.len) {
		str_add(a, "\"extendee\":");
		pb_print_string(a, m->extendee); 
		a->buf[a->len-1] = ','; // replace trailing newline
	}
	if (m->default_value.len) {
		str_add(a, "\"default_value\":");
		pb_print_string(a, m->default_value); 
		a->buf[a->len-1] = ','; // replace trailing newline
	}
	if (m->json_name.len) {
		str_add(a, "\"json_name\":");
		pb_print_string(a, m->json_name); 
		a->buf[a->len-1] = ','; // replace trailing newline
	}
	if (m->options) {
		str_add(a, "\"options\":");
		pb_print_FieldOptions(a, m->options); 
		a->buf[a->len-1] = ','; // replace trailing newline
	}
	pb_print_map_end(a);
}
void pb_get_FieldDescriptorProto(const char *p, const char *e, pb_alloc_t *obj, struct FieldDescriptorProto *m) {
	(void) obj;
	if (pb_skipto_1(&p, e, 10)) {
		p = pb_get_string(p + 1, e, (struct pb_string*)&m->name);
	}
	if (pb_skipto_1(&p, e, 18)) {
		p = pb_get_string(p + 1, e, (struct pb_string*)&m->extendee);
	}
	if (pb_skipto_1(&p, e, 24)) {
		p = pb_get_u32(p + 1, e, (uint32_t*)&m->number);
	}
	if (pb_skipto_1(&p, e, 32)) {
		p = pb_get_u32(p + 1, e, (uint32_t*)&m->label);
	}
	if (pb_skipto_1(&p, e, 40)) {
		p = pb_get_u32(p + 1, e, (uint32_t*)&m->type);
	}
	if (pb_skipto_1(&p, e, 50)) {
		p = pb_get_string(p + 1, e, (struct pb_string*)&m->type_name);
	}
	if (pb_skipto_1(&p, e, 58)) {
		p = pb_get_string(p + 1, e, (struct pb_string*)&m->default_value);
	}
	if (pb_skipto_1(&p, e, 66)) {
		struct pb_string msg;
		p = pb_get_string(p + 1, e, &msg);
		m->options = (struct FieldOptions*) pb_calloc(obj, 1, sizeof(struct FieldOptions));
		pb_get_FieldOptions(msg.p, msg.p + msg.len, obj, (struct FieldOptions*) m->options);
	}
	if (pb_skipto_1(&p, e, 72)) {
		m->oneof_index_set = true;
		p = pb_get_u32(p + 1, e, (uint32_t*)&m->oneof_index);
	}
	if (pb_skipto_1(&p, e, 82)) {
		p = pb_get_string(p + 1, e, (struct pb_string*)&m->json_name);
	}
}
char *pb_encode_FieldDescriptorProto(char *p, struct FieldDescriptorProto const *m) {
	if (m->name.len) {
		p = pb_put_tag_1(p, 10);
		p = pb_put_string(p, *(struct pb_string*)& m->name);
	}
	if (m->extendee.len) {
		p = pb_put_tag_1(p, 18);
		p = pb_put_string(p, *(struct pb_string*)& m->extendee);
	}
	if (m->number) {
		p = pb_put_tag_1(p, 24);
		p = pb_put_u32(p, *(uint32_t*)& m->number);
	}
	if (m->label) {
		p = pb_put_tag_1(p, 32);
		p = pb_put_u32(p, *(uint32_t*)& m->label);
	}
	if (m->type) {
		p = pb_put_tag_1(p, 40);
		p = pb_put_u32(p, *(uint32_t*)& m->type);
	}
	if (m->type_name.len) {
		p = pb_put_tag_1(p, 50);
		p = pb_put_string(p, *(struct pb_string*)& m->type_name);
	}
	if (m->default_value.len) {
		p = pb_put_tag_1(p, 58);
		p = pb_put_string(p, *(struct pb_string*)& m->default_value);
	}
	if (m->options) {
		p = pb_put_tag_1(p, 66);
		int szlen = pb_u32_size(m->options->pb_hdr.maxsz);
		char *start = p+szlen;
		p = pb_encode_FieldOptions(start, m->options);
		pb_put_finish(p, start, szlen);
	}
	if (m->oneof_index) {
		p = pb_put_tag_1(p, 72);
		p = pb_put_u32(p, *(uint32_t*)& m->oneof_index);
	}
	if (m->json_name.len) {
		p = pb_put_tag_1(p, 82);
		p = pb_put_string(p, *(struct pb_string*)& m->json_name);
	}
	return p;
}
int pb_maxsz_FieldDescriptorProto(struct FieldDescriptorProto const *m) {
	int n = 0;
	if (m->name.len) {
		n += 1 + 5 + m->name.len;
	}
	if (m->extendee.len) {
		n += 1 + 5 + m->extendee.len;
	}
	if (m->number) {
		n += 1 + 5;
	}
	if (m->label) {
		n += 1 + 5;
	}
	if (m->type) {
		n += 1 + 5;
	}
	if (m->type_name.len) {
		n += 1 + 5 + m->type_name.len;
	}
	if (m->default_value.len) {
		n += 1 + 5 + m->default_value.len;
	}
	if (m->options) {
		n += 1 + pb_maxsz_FieldOptions(m->options);
	}
	if (m->oneof_index) {
		n += 1 + 5;
	}
	if (m->json_name.len) {
		n += 1 + 5 + m->json_name.len;
	}
	n += pb_u32_size(n);
	((union pb_msg*) m)->maxsz = n;
	return n;
}

char *pb_print_FieldDescriptorProto_Type(char *p, enum FieldDescriptorProto_Type v){
	switch ((int32_t) v) {
	case 1:
		memcpy(p, "\"TYPE_DOUBLE\",", 14);
		return p + 14;
	case 2:
		memcpy(p, "\"TYPE_FLOAT\",", 13);
		return p + 13;
	case 3:
		memcpy(p, "\"TYPE_INT64\",", 13);
		return p + 13;
	case 4:
		memcpy(p, "\"TYPE_UINT64\",", 14);
		return p + 14;
	case 5:
		memcpy(p, "\"TYPE_INT32\",", 13);
		return p + 13;
	case 6:
		memcpy(p, "\"TYPE_FIXED64\",", 15);
		return p + 15;
	case 7:
		memcpy(p, "\"TYPE_FIXED32\",", 15);
		return p + 15;
	case 8:
		memcpy(p, "\"TYPE_BOOL\",", 12);
		return p + 12;
	case 9:
		memcpy(p, "\"TYPE_STRING\",", 14);
		return p + 14;
	case 10:
		memcpy(p, "\"TYPE_GROUP\",", 13);
		return p + 13;
	case 11:
		memcpy(p, "\"TYPE_MESSAGE\",", 15);
		return p + 15;
	case 12:
		memcpy(p, "\"TYPE_BYTES\",", 13);
		return p + 13;
	case 13:
		memcpy(p, "\"TYPE_UINT32\",", 14);
		return p + 14;
	case 14:
		memcpy(p, "\"TYPE_ENUM\",", 12);
		return p + 12;
	case 15:
		memcpy(p, "\"TYPE_SFIXED32\",", 16);
		return p + 16;
	case 16:
		memcpy(p, "\"TYPE_SFIXED64\",", 16);
		return p + 16;
	case 17:
		memcpy(p, "\"TYPE_SINT32\",", 14);
		return p + 14;
	case 18:
		memcpy(p, "\"TYPE_SINT64\",", 14);
		return p + 14;
	default:
		memcpy(p, "\"\",", 3);
		return p + 3;
	}
}
char *pb_parse_FieldDescriptorProto_Type(char *p, enum FieldDescriptorProto_Type *v) {
	struct pb_string val;
	*v = (enum FieldDescriptorProto_Type) 0;
	switch (pb_parse_enum(&p, &val, 3775814768) % 43) {
	case 35:
		if (!pb_cmp(val, "TYPE_DOUBLE")) {
			*v = (enum FieldDescriptorProto_Type) 1;
		}
		break;
	case 13:
		if (!pb_cmp(val, "TYPE_FLOAT")) {
			*v = (enum FieldDescriptorProto_Type) 2;
		}
		break;
	case 42:
		if (!pb_cmp(val, "TYPE_INT64")) {
			*v = (enum FieldDescriptorProto_Type) 3;
		}
		break;
	case 6:
		if (!pb_cmp(val, "TYPE_UINT64")) {
			*v = (enum FieldDescriptorProto_Type) 4;
		}
		break;
	case 32:
		if (!pb_cmp(val, "TYPE_INT32")) {
			*v = (enum FieldDescriptorProto_Type) 5;
		}
		break;
	case 8:
		if (!pb_cmp(val, "TYPE_FIXED64")) {
			*v = (enum FieldDescriptorProto_Type) 6;
		}
		break;
	case 25:
		if (!pb_cmp(val, "TYPE_FIXED32")) {
			*v = (enum FieldDescriptorProto_Type) 7;
		}
		break;
	case 10:
		if (!pb_cmp(val, "TYPE_BOOL")) {
			*v = (enum FieldDescriptorProto_Type) 8;
		}
		break;
	case 29:
		if (!pb_cmp(val, "TYPE_STRING")) {
			*v = (enum FieldDescriptorProto_Type) 9;
		}
		break;
	case 19:
		if (!pb_cmp(val, "TYPE_GROUP")) {
			*v = (enum FieldDescriptorProto_Type) 10;
		}
		break;
	case 24:
		if (!pb_cmp(val, "TYPE_MESSAGE")) {
			*v = (enum FieldDescriptorProto_Type) 11;
		}
		break;
	case 14:
		if (!pb_cmp(val, "TYPE_BYTES")) {
			*v = (enum FieldDescriptorProto_Type) 12;
		}
		break;
	case 39:
		if (!pb_cmp(val, "TYPE_UINT32")) {
			*v = (enum FieldDescriptorProto_Type) 13;
		}
		break;
	case 22:
		if (!pb_cmp(val, "TYPE_ENUM")) {
			*v = (enum FieldDescriptorProto_Type) 14;
		}
		break;
	case 37:
		if (!pb_cmp(val, "TYPE_SFIXED32")) {
			*v = (enum FieldDescriptorProto_Type) 15;
		}
		break;
	case 4:
		if (!pb_cmp(val, "TYPE_SFIXED64")) {
			*v = (enum FieldDescriptorProto_Type) 16;
		}
		break;
	case 17:
		if (!pb_cmp(val, "TYPE_SINT32")) {
			*v = (enum FieldDescriptorProto_Type) 17;
		}
		break;
	case 27:
		if (!pb_cmp(val, "TYPE_SINT64")) {
			*v = (enum FieldDescriptorProto_Type) 18;
		}
		break;
	}
	return p;
}

char *pb_print_FieldDescriptorProto_Label(char *p, enum FieldDescriptorProto_Label v){
	switch ((int32_t) v) {
	case 1:
		memcpy(p, "\"LABEL_OPTIONAL\",", 17);
		return p + 17;
	case 2:
		memcpy(p, "\"LABEL_REQUIRED\",", 17);
		return p + 17;
	case 3:
		memcpy(p, "\"LABEL_REPEATED\",", 17);
		return p + 17;
	default:
		memcpy(p, "\"\",", 3);
		return p + 3;
	}
}
char *pb_parse_FieldDescriptorProto_Label(char *p, enum FieldDescriptorProto_Label *v) {
	struct pb_string val;
	*v = (enum FieldDescriptorProto_Label) 0;
	switch (pb_parse_enum(&p, &val, 121) % 9) {
	case 2:
		if (!pb_cmp(val, "LABEL_OPTIONAL")) {
			*v = (enum FieldDescriptorProto_Label) 1;
		}
		break;
	case 8:
		if (!pb_cmp(val, "LABEL_REQUIRED")) {
			*v = (enum FieldDescriptorProto_Label) 2;
		}
		break;
	case 7:
		if (!pb_cmp(val, "LABEL_REPEATED")) {
			*v = (enum FieldDescriptorProto_Label) 3;
		}
		break;
	}
	return p;
}

char *pb_parse_OneofDescriptorProto(char *p, pb_alloc_t *a, struct OneofDescriptorProto *m) {
	(void) a;
	(void) m;
	if (!pb_parse_map(&p)) {
		return p;
	}
	for (;;) {
		struct pb_string key;
		switch (pb_parse_field(&p, &key, 4) % 5) {
		case 0:
			return p;
		case 4:
			if (pb_cmp(key, "name")) {
				goto unknown;
			}
			p = pb_parse_string(p, &m->name);
			continue;
		case 1:
			if (pb_cmp(key, "options")) {
				goto unknown;
			}
			m->options = (struct OneofOptions*) pb_calloc(a, 1, sizeof(struct OneofOptions));
			p = pb_parse_OneofOptions(p, a, (struct OneofOptions*) m->options);
			continue;
		default:
		unknown:
			p = pb_parse_skip(p);
		}
	}
}
int pb_fparse_OneofDescriptorProto(const char *fn, str_t *b, pb_alloc_t *a, struct OneofDescriptorProto *m) {
	int off = b->len;
	str_read_file(b, fn);
	return pb_parse_OneofDescriptorProto(b->buf + off, a, m) == pb_errret ? -1 : 0; 
}
void pb_print_OneofDescriptorProto(str_t *a, struct OneofDescriptorProto const *m) {
	str_grow(a, a->len + 1);
	char *p = a->buf + a->len;
	*p++ = '{';
	str_setlen(a, (unsigned) (p - a->buf));
	if (m->name.len) {
		str_add(a, "\"name\":");
		pb_print_string(a, m->name); 
		a->buf[a->len-1] = ','; // replace trailing newline
	}
	if (m->options) {
		str_add(a, "\"options\":");
		pb_print_OneofOptions(a, m->options); 
		a->buf[a->len-1] = ','; // replace trailing newline
	}
	pb_print_map_end(a);
}
void pb_get_OneofDescriptorProto(const char *p, const char *e, pb_alloc_t *obj, struct OneofDescriptorProto *m) {
	(void) obj;
	if (pb_skipto_1(&p, e, 10)) {
		p = pb_get_string(p + 1, e, (struct pb_string*)&m->name);
	}
	if (pb_skipto_1(&p, e, 18)) {
		struct pb_string msg;
		p = pb_get_string(p + 1, e, &msg);
		m->options = (struct OneofOptions*) pb_calloc(obj, 1, sizeof(struct OneofOptions));
		pb_get_OneofOptions(msg.p, msg.p + msg.len, obj, (struct OneofOptions*) m->options);
	}
}
char *pb_encode_OneofDescriptorProto(char *p, struct OneofDescriptorProto const *m) {
	if (m->name.len) {
		p = pb_put_tag_1(p, 10);
		p = pb_put_string(p, *(struct pb_string*)& m->name);
	}
	if (m->options) {
		p = pb_put_tag_1(p, 18);
		int szlen = pb_u32_size(m->options->pb_hdr.maxsz);
		char *start = p+szlen;
		p = pb_encode_OneofOptions(start, m->options);
		pb_put_finish(p, start, szlen);
	}
	return p;
}
int pb_maxsz_OneofDescriptorProto(struct OneofDescriptorProto const *m) {
	int n = 0;
	if (m->name.len) {
		n += 1 + 5 + m->name.len;
	}
	if (m->options) {
		n += 1 + pb_maxsz_OneofOptions(m->options);
	}
	n += pb_u32_size(n);
	((union pb_msg*) m)->maxsz = n;
	return n;
}

char *pb_parse_EnumDescriptorProto(char *p, pb_alloc_t *a, struct EnumDescriptorProto *m) {
	(void) a;
	(void) m;
	if (!pb_parse_map(&p)) {
		return p;
	}
	for (;;) {
		struct pb_string key;
		switch (pb_parse_field(&p, &key, 121) % 9) {
		case 0:
			return p;
		case 3:
			if (pb_cmp(key, "name")) {
				goto unknown;
			}
			p = pb_parse_string(p, &m->name);
			continue;
		case 2:
			if (pb_cmp(key, "value")) {
				goto unknown;
			}
			if (pb_parse_array(&p)) {
				struct EnumValueDescriptorProto *prev = NULL;
				do {
					struct EnumValueDescriptorProto *c = (struct EnumValueDescriptorProto*) pb_calloc(a, 1, sizeof(struct EnumValueDescriptorProto));
					p = pb_parse_EnumValueDescriptorProto(p, a, c);
					m->value.len++;
					c->pb_hdr.prev = prev;
					prev = c;
				} while (pb_more_array(&p));

				m->value.v = (const struct EnumValueDescriptorProto**) pb_calloc(a, m->value.len, sizeof(struct EnumValueDescriptorProto*));

				for (int i = m->value.len - 1; i >= 0; i--) {
					m->value.v[i] = prev;
					prev = (struct EnumValueDescriptorProto*) prev->pb_hdr.prev;
				}
			}
			continue;
		case 6:
			if (pb_cmp(key, "options")) {
				goto unknown;
			}
			m->options = (struct EnumOptions*) pb_calloc(a, 1, sizeof(struct EnumOptions));
			p = pb_parse_EnumOptions(p, a, (struct EnumOptions*) m->options);
			continue;
		default:
		unknown:
			p = pb_parse_skip(p);
		}
	}
}
int pb_fparse_EnumDescriptorProto(const char *fn, str_t *b, pb_alloc_t *a, struct EnumDescriptorProto *m) {
	int off = b->len;
	str_read_file(b, fn);
	return pb_parse_EnumDescriptorProto(b->buf + off, a, m) == pb_errret ? -1 : 0; 
}
void pb_print_EnumDescriptorProto(str_t *a, struct EnumDescriptorProto const *m) {
	str_grow(a, a->len + 1);
	char *p = a->buf + a->len;
	*p++ = '{';
	str_setlen(a, (unsigned) (p - a->buf));
	if (m->name.len) {
		str_add(a, "\"name\":");
		pb_print_string(a, m->name); 
		a->buf[a->len-1] = ','; // replace trailing newline
	}
	if (m->value.len) {
		str_add(a, "\"value\":");
		str_addch(a, '[');
		for (int i = 0; i < m->value.len; i++) {
			pb_print_EnumValueDescriptorProto(a, m->value.v[i]);
			a->buf[a->len-1] = ','; // replace trailing newline
		}
		pb_print_array_end(a);
	}
	if (m->options) {
		str_add(a, "\"options\":");
		pb_print_EnumOptions(a, m->options); 
		a->buf[a->len-1] = ','; // replace trailing newline
	}
	pb_print_map_end(a);
}
void pb_get_EnumDescriptorProto(const char *p, const char *e, pb_alloc_t *obj, struct EnumDescriptorProto *m) {
	(void) obj;
	if (pb_skipto_1(&p, e, 10)) {
		p = pb_get_string(p + 1, e, (struct pb_string*)&m->name);
	}
	if (pb_skipto_1(&p, e, 18)) {
		struct EnumValueDescriptorProto *prev = NULL;
		do {
			struct pb_string msg;
			p = pb_get_string(p + 1, e, &msg);
			struct EnumValueDescriptorProto *c = (struct EnumValueDescriptorProto*) pb_calloc(obj, 1, sizeof(struct EnumValueDescriptorProto));
			pb_get_EnumValueDescriptorProto(msg.p, msg.p + msg.len, obj, c);
			m->value.len++;
			c->pb_hdr.prev = prev;
			prev = c;
		} while (!pb_cmp_tag_1(p, e, 18));

		m->value.v = (const struct EnumValueDescriptorProto**) pb_calloc(obj, m->value.len, sizeof(struct EnumValueDescriptorProto*));

		for (int i = m->value.len - 1; i >= 0; i--) {
			m->value.v[i] = prev;
			prev = (struct EnumValueDescriptorProto*) prev->pb_hdr.prev;
		}
	}
	if (pb_skipto_1(&p, e, 26)) {
		struct pb_string msg;
		p = pb_get_string(p + 1, e, &msg);
		m->options = (struct EnumOptions*) pb_calloc(obj, 1, sizeof(struct EnumOptions));
		pb_get_EnumOptions(msg.p, msg.p + msg.len, obj, (struct EnumOptions*) m->options);
	}
}
char *pb_encode_EnumDescriptorProto(char *p, struct EnumDescriptorProto const *m) {
	if (m->name.len) {
		p = pb_put_tag_1(p, 10);
		p = pb_put_string(p, *(struct pb_string*)& m->name);
	}
	if (m->value.len) {
		for (int i = 0; i < m->value.len; i++) {
			p = pb_put_tag_1(p, 18);
			int szlen = pb_u32_size(m->value.v[i]->pb_hdr.maxsz);
			char *start = p+szlen;
			p = pb_encode_EnumValueDescriptorProto(start, m->value.v[i]);
			pb_put_finish(p, start, szlen);
		}
	}
	if (m->options) {
		p = pb_put_tag_1(p, 26);
		int szlen = pb_u32_size(m->options->pb_hdr.maxsz);
		char *start = p+szlen;
		p = pb_encode_EnumOptions(start, m->options);
		pb_put_finish(p, start, szlen);
	}
	return p;
}
int pb_maxsz_EnumDescriptorProto(struct EnumDescriptorProto const *m) {
	int n = 0;
	if (m->name.len) {
		n += 1 + 5 + m->name.len;
	}
	if (m->value.len) {
		for (int i = 0; i < m->value.len; i++) {
			n += 1 + pb_maxsz_EnumValueDescriptorProto(m->value.v[i]);
		}
	}
	if (m->options) {
		n += 1 + pb_maxsz_EnumOptions(m->options);
	}
	n += pb_u32_size(n);
	((union pb_msg*) m)->maxsz = n;
	return n;
}

char *pb_parse_EnumValueDescriptorProto(char *p, pb_alloc_t *a, struct EnumValueDescriptorProto *m) {
	(void) a;
	(void) m;
	if (!pb_parse_map(&p)) {
		return p;
	}
	for (;;) {
		struct pb_string key;
		switch (pb_parse_field(&p, &key, 40) % 8) {
		case 0:
			return p;
		case 5:
			if (pb_cmp(key, "name")) {
				goto unknown;
			}
			p = pb_parse_string(p, &m->name);
			continue;
		case 2:
			if (pb_cmp(key, "number")) {
				goto unknown;
			}
			p = pb_parse_i32(p, &m->number);
			continue;
		case 3:
			if (pb_cmp(key, "options")) {
				goto unknown;
			}
			m->options = (struct EnumValueOptions*) pb_calloc(a, 1, sizeof(struct EnumValueOptions));
			p = pb_parse_EnumValueOptions(p, a, (struct EnumValueOptions*) m->options);
			continue;
		default:
		unknown:
			p = pb_parse_skip(p);
		}
	}
}
int pb_fparse_EnumValueDescriptorProto(const char *fn, str_t *b, pb_alloc_t *a, struct EnumValueDescriptorProto *m) {
	int off = b->len;
	str_read_file(b, fn);
	return pb_parse_EnumValueDescriptorProto(b->buf + off, a, m) == pb_errret ? -1 : 0; 
}
void pb_print_EnumValueDescriptorProto(str_t *a, struct EnumValueDescriptorProto const *m) {
	str_grow(a, a->len + 22);
	char *p = a->buf + a->len;
	*p++ = '{';
	if (m->number) {
		memcpy(p, "\"number\":", 9);
		p += 9;
		p = pb_print_i32(p, m->number);
	}
	str_setlen(a, (unsigned) (p - a->buf));
	if (m->name.len) {
		str_add(a, "\"name\":");
		pb_print_string(a, m->name); 
		a->buf[a->len-1] = ','; // replace trailing newline
	}
	if (m->options) {
		str_add(a, "\"options\":");
		pb_print_EnumValueOptions(a, m->options); 
		a->buf[a->len-1] = ','; // replace trailing newline
	}
	pb_print_map_end(a);
}
void pb_get_EnumValueDescriptorProto(const char *p, const char *e, pb_alloc_t *obj, struct EnumValueDescriptorProto *m) {
	(void) obj;
	if (pb_skipto_1(&p, e, 10)) {
		p = pb_get_string(p + 1, e, (struct pb_string*)&m->name);
	}
	if (pb_skipto_1(&p, e, 16)) {
		p = pb_get_u32(p + 1, e, (uint32_t*)&m->number);
	}
	if (pb_skipto_1(&p, e, 26)) {
		struct pb_string msg;
		p = pb_get_string(p + 1, e, &msg);
		m->options = (struct EnumValueOptions*) pb_calloc(obj, 1, sizeof(struct EnumValueOptions));
		pb_get_EnumValueOptions(msg.p, msg.p + msg.len, obj, (struct EnumValueOptions*) m->options);
	}
}
char *pb_encode_EnumValueDescriptorProto(char *p, struct EnumValueDescriptorProto const *m) {
	if (m->name.len) {
		p = pb_put_tag_1(p, 10);
		p = pb_put_string(p, *(struct pb_string*)& m->name);
	}
	if (m->number) {
		p = pb_put_tag_1(p, 16);
		p = pb_put_u32(p, *(uint32_t*)& m->number);
	}
	if (m->options) {
		p = pb_put_tag_1(p, 26);
		int szlen = pb_u32_size(m->options->pb_hdr.maxsz);
		char *start = p+szlen;
		p = pb_encode_EnumValueOptions(start, m->options);
		pb_put_finish(p, start, szlen);
	}
	return p;
}
int pb_maxsz_EnumValueDescriptorProto(struct EnumValueDescriptorProto const *m) {
	int n = 0;
	if (m->name.len) {
		n += 1 + 5 + m->name.len;
	}
	if (m->number) {
		n += 1 + 5;
	}
	if (m->options) {
		n += 1 + pb_maxsz_EnumValueOptions(m->options);
	}
	n += pb_u32_size(n);
	((union pb_msg*) m)->maxsz = n;
	return n;
}

char *pb_parse_ServiceDescriptorProto(char *p, pb_alloc_t *a, struct ServiceDescriptorProto *m) {
	(void) a;
	(void) m;
	if (!pb_parse_map(&p)) {
		return p;
	}
	for (;;) {
		struct pb_string key;
		switch (pb_parse_field(&p, &key, 40) % 8) {
		case 0:
			return p;
		case 5:
			if (pb_cmp(key, "name")) {
				goto unknown;
			}
			p = pb_parse_string(p, &m->name);
			continue;
		case 4:
			if (pb_cmp(key, "method")) {
				goto unknown;
			}
			if (pb_parse_array(&p)) {
				struct MethodDescriptorProto *prev = NULL;
				do {
					struct MethodDescriptorProto *c = (struct MethodDescriptorProto*) pb_calloc(a, 1, sizeof(struct MethodDescriptorProto));
					p = pb_parse_MethodDescriptorProto(p, a, c);
					m->method.len++;
					c->pb_hdr.prev = prev;
					prev = c;
				} while (pb_more_array(&p));

				m->method.v = (const struct MethodDescriptorProto**) pb_calloc(a, m->method.len, sizeof(struct MethodDescriptorProto*));

				for (int i = m->method.len - 1; i >= 0; i--) {
					m->method.v[i] = prev;
					prev = (struct MethodDescriptorProto*) prev->pb_hdr.prev;
				}
			}
			continue;
		case 3:
			if (pb_cmp(key, "options")) {
				goto unknown;
			}
			m->options = (struct ServiceOptions*) pb_calloc(a, 1, sizeof(struct ServiceOptions));
			p = pb_parse_ServiceOptions(p, a, (struct ServiceOptions*) m->options);
			continue;
		default:
		unknown:
			p = pb_parse_skip(p);
		}
	}
}
int pb_fparse_ServiceDescriptorProto(const char *fn, str_t *b, pb_alloc_t *a, struct ServiceDescriptorProto *m) {
	int off = b->len;
	str_read_file(b, fn);
	return pb_parse_ServiceDescriptorProto(b->buf + off, a, m) == pb_errret ? -1 : 0; 
}
void pb_print_ServiceDescriptorProto(str_t *a, struct ServiceDescriptorProto const *m) {
	str_grow(a, a->len + 1);
	char *p = a->buf + a->len;
	*p++ = '{';
	str_setlen(a, (unsigned) (p - a->buf));
	if (m->name.len) {
		str_add(a, "\"name\":");
		pb_print_string(a, m->name); 
		a->buf[a->len-1] = ','; // replace trailing newline
	}
	if (m->method.len) {
		str_add(a, "\"method\":");
		str_addch(a, '[');
		for (int i = 0; i < m->method.len; i++) {
			pb_print_MethodDescriptorProto(a, m->method.v[i]);
			a->buf[a->len-1] = ','; // replace trailing newline
		}
		pb_print_array_end(a);
	}
	if (m->options) {
		str_add(a, "\"options\":");
		pb_print_ServiceOptions(a, m->options); 
		a->buf[a->len-1] = ','; // replace trailing newline
	}
	pb_print_map_end(a);
}
void pb_get_ServiceDescriptorProto(const char *p, const char *e, pb_alloc_t *obj, struct ServiceDescriptorProto *m) {
	(void) obj;
	if (pb_skipto_1(&p, e, 10)) {
		p = pb_get_string(p + 1, e, (struct pb_string*)&m->name);
	}
	if (pb_skipto_1(&p, e, 18)) {
		struct MethodDescriptorProto *prev = NULL;
		do {
			struct pb_string msg;
			p = pb_get_string(p + 1, e, &msg);
			struct MethodDescriptorProto *c = (struct MethodDescriptorProto*) pb_calloc(obj, 1, sizeof(struct MethodDescriptorProto));
			pb_get_MethodDescriptorProto(msg.p, msg.p + msg.len, obj, c);
			m->method.len++;
			c->pb_hdr.prev = prev;
			prev = c;
		} while (!pb_cmp_tag_1(p, e, 18));

		m->method.v = (const struct MethodDescriptorProto**) pb_calloc(obj, m->method.len, sizeof(struct MethodDescriptorProto*));

		for (int i = m->method.len - 1; i >= 0; i--) {
			m->method.v[i] = prev;
			prev = (struct MethodDescriptorProto*) prev->pb_hdr.prev;
		}
	}
	if (pb_skipto_1(&p, e, 26)) {
		struct pb_string msg;
		p = pb_get_string(p + 1, e, &msg);
		m->options = (struct ServiceOptions*) pb_calloc(obj, 1, sizeof(struct ServiceOptions));
		pb_get_ServiceOptions(msg.p, msg.p + msg.len, obj, (struct ServiceOptions*) m->options);
	}
}
char *pb_encode_ServiceDescriptorProto(char *p, struct ServiceDescriptorProto const *m) {
	if (m->name.len) {
		p = pb_put_tag_1(p, 10);
		p = pb_put_string(p, *(struct pb_string*)& m->name);
	}
	if (m->method.len) {
		for (int i = 0; i < m->method.len; i++) {
			p = pb_put_tag_1(p, 18);
			int szlen = pb_u32_size(m->method.v[i]->pb_hdr.maxsz);
			char *start = p+szlen;
			p = pb_encode_MethodDescriptorProto(start, m->method.v[i]);
			pb_put_finish(p, start, szlen);
		}
	}
	if (m->options) {
		p = pb_put_tag_1(p, 26);
		int szlen = pb_u32_size(m->options->pb_hdr.maxsz);
		char *start = p+szlen;
		p = pb_encode_ServiceOptions(start, m->options);
		pb_put_finish(p, start, szlen);
	}
	return p;
}
int pb_maxsz_ServiceDescriptorProto(struct ServiceDescriptorProto const *m) {
	int n = 0;
	if (m->name.len) {
		n += 1 + 5 + m->name.len;
	}
	if (m->method.len) {
		for (int i = 0; i < m->method.len; i++) {
			n += 1 + pb_maxsz_MethodDescriptorProto(m->method.v[i]);
		}
	}
	if (m->options) {
		n += 1 + pb_maxsz_ServiceOptions(m->options);
	}
	n += pb_u32_size(n);
	((union pb_msg*) m)->maxsz = n;
	return n;
}

char *pb_parse_MethodDescriptorProto(char *p, pb_alloc_t *a, struct MethodDescriptorProto *m) {
	(void) a;
	(void) m;
	if (!pb_parse_map(&p)) {
		return p;
	}
	for (;;) {
		struct pb_string key;
		switch (pb_parse_field(&p, &key, 29524) % 17) {
		case 0:
			return p;
		case 6:
			if (pb_cmp(key, "name")) {
				goto unknown;
			}
			p = pb_parse_string(p, &m->name);
			continue;
		case 11:
			if (pb_cmp(key, "input_type")) {
				goto unknown;
			}
			p = pb_parse_string(p, &m->input_type);
			continue;
		case 5:
			if (pb_cmp(key, "output_type")) {
				goto unknown;
			}
			p = pb_parse_string(p, &m->output_type);
			continue;
		case 3:
			if (pb_cmp(key, "options")) {
				goto unknown;
			}
			m->options = (struct MethodOptions*) pb_calloc(a, 1, sizeof(struct MethodOptions));
			p = pb_parse_MethodOptions(p, a, (struct MethodOptions*) m->options);
			continue;
		case 2:
			if (pb_cmp(key, "client_streaming")) {
				goto unknown;
			}
			p = pb_parse_bool(p, &m->client_streaming);
			continue;
		case 7:
			if (pb_cmp(key, "server_streaming")) {
				goto unknown;
			}
			p = pb_parse_bool(p, &m->server_streaming);
			continue;
		default:
		unknown:
			p = pb_parse_skip(p);
		}
	}
}
int pb_fparse_MethodDescriptorProto(const char *fn, str_t *b, pb_alloc_t *a, struct MethodDescriptorProto *m) {
	int off = b->len;
	str_read_file(b, fn);
	return pb_parse_MethodDescriptorProto(b->buf + off, a, m) == pb_errret ? -1 : 0; 
}
void pb_print_MethodDescriptorProto(str_t *a, struct MethodDescriptorProto const *m) {
	str_grow(a, a->len + 51);
	char *p = a->buf + a->len;
	*p++ = '{';
	if (m->client_streaming) {
		memcpy(p, "\"client_streaming\":", 19);
		p += 19;
		p = pb_print_bool(p, m->client_streaming);
	}
	if (m->server_streaming) {
		memcpy(p, "\"server_streaming\":", 19);
		p += 19;
		p = pb_print_bool(p, m->server_streaming);
	}
	str_setlen(a, (unsigned) (p - a->buf));
	if (m->name.len) {
		str_add(a, "\"name\":");
		pb_print_string(a, m->name); 
		a->buf[a->len-1] = ','; // replace trailing newline
	}
	if (m->input_type.len) {
		str_add(a, "\"input_type\":");
		pb_print_string(a, m->input_type); 
		a->buf[a->len-1] = ','; // replace trailing newline
	}
	if (m->output_type.len) {
		str_add(a, "\"output_type\":");
		pb_print_string(a, m->output_type); 
		a->buf[a->len-1] = ','; // replace trailing newline
	}
	if (m->options) {
		str_add(a, "\"options\":");
		pb_print_MethodOptions(a, m->options); 
		a->buf[a->len-1] = ','; // replace trailing newline
	}
	pb_print_map_end(a);
}
void pb_get_MethodDescriptorProto(const char *p, const char *e, pb_alloc_t *obj, struct MethodDescriptorProto *m) {
	(void) obj;
	if (pb_skipto_1(&p, e, 10)) {
		p = pb_get_string(p + 1, e, (struct pb_string*)&m->name);
	}
	if (pb_skipto_1(&p, e, 18)) {
		p = pb_get_string(p + 1, e, (struct pb_string*)&m->input_type);
	}
	if (pb_skipto_1(&p, e, 26)) {
		p = pb_get_string(p + 1, e, (struct pb_string*)&m->output_type);
	}
	if (pb_skipto_1(&p, e, 34)) {
		struct pb_string msg;
		p = pb_get_string(p + 1, e, &msg);
		m->options = (struct MethodOptions*) pb_calloc(obj, 1, sizeof(struct MethodOptions));
		pb_get_MethodOptions(msg.p, msg.p + msg.len, obj, (struct MethodOptions*) m->options);
	}
	if (pb_skipto_1(&p, e, 40)) {
		p = pb_get_bool(p + 1, e, &m->client_streaming);
	}
	if (pb_skipto_1(&p, e, 48)) {
		p = pb_get_bool(p + 1, e, &m->server_streaming);
	}
}
char *pb_encode_MethodDescriptorProto(char *p, struct MethodDescriptorProto const *m) {
	if (m->name.len) {
		p = pb_put_tag_1(p, 10);
		p = pb_put_string(p, *(struct pb_string*)& m->name);
	}
	if (m->input_type.len) {
		p = pb_put_tag_1(p, 18);
		p = pb_put_string(p, *(struct pb_string*)& m->input_type);
	}
	if (m->output_type.len) {
		p = pb_put_tag_1(p, 26);
		p = pb_put_string(p, *(struct pb_string*)& m->output_type);
	}
	if (m->options) {
		p = pb_put_tag_1(p, 34);
		int szlen = pb_u32_size(m->options->pb_hdr.maxsz);
		char *start = p+szlen;
		p = pb_encode_MethodOptions(start, m->options);
		pb_put_finish(p, start, szlen);
	}
	if (m->client_streaming) {
		p = pb_put_tag_1(p, 40);
		p = pb_put_bool(p,  m->client_streaming);
	}
	if (m->server_streaming) {
		p = pb_put_tag_1(p, 48);
		p = pb_put_bool(p,  m->server_streaming);
	}
	return p;
}
int pb_maxsz_MethodDescriptorProto(struct MethodDescriptorProto const *m) {
	int n = 0;
	if (m->name.len) {
		n += 1 + 5 + m->name.len;
	}
	if (m->input_type.len) {
		n += 1 + 5 + m->input_type.len;
	}
	if (m->output_type.len) {
		n += 1 + 5 + m->output_type.len;
	}
	if (m->options) {
		n += 1 + pb_maxsz_MethodOptions(m->options);
	}
	if (m->client_streaming) {
		n += 1 + 1;
	}
	if (m->server_streaming) {
		n += 1 + 1;
	}
	n += pb_u32_size(n);
	((union pb_msg*) m)->maxsz = n;
	return n;
}

char *pb_parse_FileOptions(char *p, pb_alloc_t *a, struct FileOptions *m) {
	(void) a;
	(void) m;
	if (!pb_parse_map(&p)) {
		return p;
	}
	for (;;) {
		struct pb_string key;
		switch (pb_parse_field(&p, &key, 3102528941) % 67) {
		case 0:
			return p;
		case 49:
			if (pb_cmp(key, "java_package")) {
				goto unknown;
			}
			p = pb_parse_string(p, &m->java_package);
			continue;
		case 4:
			if (pb_cmp(key, "java_outer_classname")) {
				goto unknown;
			}
			p = pb_parse_string(p, &m->java_outer_classname);
			continue;
		case 7:
			if (pb_cmp(key, "java_multiple_files")) {
				goto unknown;
			}
			p = pb_parse_bool(p, &m->java_multiple_files);
			continue;
		case 46:
			if (pb_cmp(key, "java_generate_equals_and_hash")) {
				goto unknown;
			}
			p = pb_parse_bool(p, &m->java_generate_equals_and_hash);
			continue;
		case 59:
			if (pb_cmp(key, "java_string_check_utf8")) {
				goto unknown;
			}
			p = pb_parse_bool(p, &m->java_string_check_utf8);
			continue;
		case 34:
			if (pb_cmp(key, "optimize_for")) {
				goto unknown;
			}
			p = pb_parse_FileOptions_OptimizeMode(p, &m->optimize_for);
			continue;
		case 1:
			if (pb_cmp(key, "go_package")) {
				goto unknown;
			}
			p = pb_parse_string(p, &m->go_package);
			continue;
		case 5:
			if (pb_cmp(key, "cc_generic_services")) {
				goto unknown;
			}
			p = pb_parse_bool(p, &m->cc_generic_services);
			continue;
		case 36:
			if (pb_cmp(key, "java_generic_services")) {
				goto unknown;
			}
			p = pb_parse_bool(p, &m->java_generic_services);
			continue;
		case 56:
			if (pb_cmp(key, "py_generic_services")) {
				goto unknown;
			}
			p = pb_parse_bool(p, &m->py_generic_services);
			continue;
		case 3:
			if (pb_cmp(key, "deprecated")) {
				goto unknown;
			}
			p = pb_parse_bool(p, &m->deprecated);
			continue;
		case 57:
			if (pb_cmp(key, "cc_enable_arenas")) {
				goto unknown;
			}
			p = pb_parse_bool(p, &m->cc_enable_arenas);
			continue;
		case 19:
			if (pb_cmp(key, "objc_class_prefix")) {
				goto unknown;
			}
			p = pb_parse_string(p, &m->objc_class_prefix);
			continue;
		case 16:
			if (pb_cmp(key, "csharp_namespace")) {
				goto unknown;
			}
			p = pb_parse_string(p, &m->csharp_namespace);
			continue;
		case 50:
			if (pb_cmp(key, "uninterpreted_option")) {
				goto unknown;
			}
			if (pb_parse_array(&p)) {
				struct UninterpretedOption *prev = NULL;
				do {
					struct UninterpretedOption *c = (struct UninterpretedOption*) pb_calloc(a, 1, sizeof(struct UninterpretedOption));
					p = pb_parse_UninterpretedOption(p, a, c);
					m->uninterpreted_option.len++;
					c->pb_hdr.prev = prev;
					prev = c;
				} while (pb_more_array(&p));

				m->uninterpreted_option.v = (const struct UninterpretedOption**) pb_calloc(a, m->uninterpreted_option.len, sizeof(struct UninterpretedOption*));

				for (int i = m->uninterpreted_option.len - 1; i >= 0; i--) {
					m->uninterpreted_option.v[i] = prev;
					prev = (struct UninterpretedOption*) prev->pb_hdr.prev;
				}
			}
			continue;
		default:
		unknown:
			p = pb_parse_skip(p);
		}
	}
}
int pb_fparse_FileOptions(const char *fn, str_t *b, pb_alloc_t *a, struct FileOptions *m) {
	int off = b->len;
	str_read_file(b, fn);
	return pb_parse_FileOptions(b->buf + off, a, m) == pb_errret ? -1 : 0; 
}
void pb_print_FileOptions(str_t *a, struct FileOptions const *m) {
	str_grow(a, a->len + 259);
	char *p = a->buf + a->len;
	*p++ = '{';
	if (m->java_multiple_files) {
		memcpy(p, "\"java_multiple_files\":", 22);
		p += 22;
		p = pb_print_bool(p, m->java_multiple_files);
	}
	if (m->java_generate_equals_and_hash) {
		memcpy(p, "\"java_generate_equals_and_hash\":", 32);
		p += 32;
		p = pb_print_bool(p, m->java_generate_equals_and_hash);
	}
	if (m->java_string_check_utf8) {
		memcpy(p, "\"java_string_check_utf8\":", 25);
		p += 25;
		p = pb_print_bool(p, m->java_string_check_utf8);
	}
	if (m->optimize_for) {
		memcpy(p, "\"optimize_for\":", 15);
		p += 15;
		p = pb_print_FileOptions_OptimizeMode(p, m->optimize_for);
	}
	if (m->cc_generic_services) {
		memcpy(p, "\"cc_generic_services\":", 22);
		p += 22;
		p = pb_print_bool(p, m->cc_generic_services);
	}
	if (m->java_generic_services) {
		memcpy(p, "\"java_generic_services\":", 24);
		p += 24;
		p = pb_print_bool(p, m->java_generic_services);
	}
	if (m->py_generic_services) {
		memcpy(p, "\"py_generic_services\":", 22);
		p += 22;
		p = pb_print_bool(p, m->py_generic_services);
	}
	if (m->deprecated) {
		memcpy(p, "\"deprecated\":", 13);
		p += 13;
		p = pb_print_bool(p, m->deprecated);
	}
	if (m->cc_enable_arenas) {
		memcpy(p, "\"cc_enable_arenas\":", 19);
		p += 19;
		p = pb_print_bool(p, m->cc_enable_arenas);
	}
	str_setlen(a, (unsigned) (p - a->buf));
	if (m->java_package.len) {
		str_add(a, "\"java_package\":");
		pb_print_string(a, m->java_package); 
		a->buf[a->len-1] = ','; // replace trailing newline
	}
	if (m->java_outer_classname.len) {
		str_add(a, "\"java_outer_classname\":");
		pb_print_string(a, m->java_outer_classname); 
		a->buf[a->len-1] = ','; // replace trailing newline
	}
	if (m->go_package.len) {
		str_add(a, "\"go_package\":");
		pb_print_string(a, m->go_package); 
		a->buf[a->len-1] = ','; // replace trailing newline
	}
	if (m->objc_class_prefix.len) {
		str_add(a, "\"objc_class_prefix\":");
		pb_print_string(a, m->objc_class_prefix); 
		a->buf[a->len-1] = ','; // replace trailing newline
	}
	if (m->csharp_namespace.len) {
		str_add(a, "\"csharp_namespace\":");
		pb_print_string(a, m->csharp_namespace); 
		a->buf[a->len-1] = ','; // replace trailing newline
	}
	if (m->uninterpreted_option.len) {
		str_add(a, "\"uninterpreted_option\":");
		str_addch(a, '[');
		for (int i = 0; i < m->uninterpreted_option.len; i++) {
			pb_print_UninterpretedOption(a, m->uninterpreted_option.v[i]);
			a->buf[a->len-1] = ','; // replace trailing newline
		}
		pb_print_array_end(a);
	}
	pb_print_map_end(a);
}
void pb_get_FileOptions(const char *p, const char *e, pb_alloc_t *obj, struct FileOptions *m) {
	(void) obj;
	if (pb_skipto_1(&p, e, 10)) {
		p = pb_get_string(p + 1, e, (struct pb_string*)&m->java_package);
	}
	if (pb_skipto_1(&p, e, 66)) {
		p = pb_get_string(p + 1, e, (struct pb_string*)&m->java_outer_classname);
	}
	if (pb_skipto_1(&p, e, 72)) {
		p = pb_get_u32(p + 1, e, (uint32_t*)&m->optimize_for);
	}
	if (pb_skipto_1(&p, e, 80)) {
		p = pb_get_bool(p + 1, e, &m->java_multiple_files);
	}
	if (pb_skipto_1(&p, e, 90)) {
		p = pb_get_string(p + 1, e, (struct pb_string*)&m->go_package);
	}
	p = pb_toend_1(p,e);
	if (pb_skipto_2(&p, e, 384)) {
		p = pb_get_bool(p + 2, e, &m->cc_generic_services);
	}
	if (pb_skipto_2(&p, e, 392)) {
		p = pb_get_bool(p + 2, e, &m->java_generic_services);
	}
	if (pb_skipto_2(&p, e, 400)) {
		p = pb_get_bool(p + 2, e, &m->py_generic_services);
	}
	if (pb_skipto_2(&p, e, 416)) {
		p = pb_get_bool(p + 2, e, &m->java_generate_equals_and_hash);
	}
	if (pb_skipto_2(&p, e, 440)) {
		p = pb_get_bool(p + 2, e, &m->deprecated);
	}
	if (pb_skipto_2(&p, e, 472)) {
		p = pb_get_bool(p + 2, e, &m->java_string_check_utf8);
	}
	if (pb_skipto_2(&p, e, 504)) {
		p = pb_get_bool(p + 2, e, &m->cc_enable_arenas);
	}
	if (pb_skipto_2(&p, e, 674)) {
		p = pb_get_string(p + 2, e, (struct pb_string*)&m->objc_class_prefix);
	}
	if (pb_skipto_2(&p, e, 682)) {
		p = pb_get_string(p + 2, e, (struct pb_string*)&m->csharp_namespace);
	}
	if (pb_skipto_2(&p, e, 16058)) {
		struct UninterpretedOption *prev = NULL;
		do {
			struct pb_string msg;
			p = pb_get_string(p + 2, e, &msg);
			struct UninterpretedOption *c = (struct UninterpretedOption*) pb_calloc(obj, 1, sizeof(struct UninterpretedOption));
			pb_get_UninterpretedOption(msg.p, msg.p + msg.len, obj, c);
			m->uninterpreted_option.len++;
			c->pb_hdr.prev = prev;
			prev = c;
		} while (!pb_cmp_tag_2(p, e, 16058));

		m->uninterpreted_option.v = (const struct UninterpretedOption**) pb_calloc(obj, m->uninterpreted_option.len, sizeof(struct UninterpretedOption*));

		for (int i = m->uninterpreted_option.len - 1; i >= 0; i--) {
			m->uninterpreted_option.v[i] = prev;
			prev = (struct UninterpretedOption*) prev->pb_hdr.prev;
		}
	}
}
char *pb_encode_FileOptions(char *p, struct FileOptions const *m) {
	if (m->java_package.len) {
		p = pb_put_tag_1(p, 10);
		p = pb_put_string(p, *(struct pb_string*)& m->java_package);
	}
	if (m->java_outer_classname.len) {
		p = pb_put_tag_1(p, 66);
		p = pb_put_string(p, *(struct pb_string*)& m->java_outer_classname);
	}
	if (m->optimize_for) {
		p = pb_put_tag_1(p, 72);
		p = pb_put_u32(p, *(uint32_t*)& m->optimize_for);
	}
	if (m->java_multiple_files) {
		p = pb_put_tag_1(p, 80);
		p = pb_put_bool(p,  m->java_multiple_files);
	}
	if (m->go_package.len) {
		p = pb_put_tag_1(p, 90);
		p = pb_put_string(p, *(struct pb_string*)& m->go_package);
	}
	if (m->cc_generic_services) {
		p = pb_put_tag_2(p, 384);
		p = pb_put_bool(p,  m->cc_generic_services);
	}
	if (m->java_generic_services) {
		p = pb_put_tag_2(p, 392);
		p = pb_put_bool(p,  m->java_generic_services);
	}
	if (m->py_generic_services) {
		p = pb_put_tag_2(p, 400);
		p = pb_put_bool(p,  m->py_generic_services);
	}
	if (m->java_generate_equals_and_hash) {
		p = pb_put_tag_2(p, 416);
		p = pb_put_bool(p,  m->java_generate_equals_and_hash);
	}
	if (m->deprecated) {
		p = pb_put_tag_2(p, 440);
		p = pb_put_bool(p,  m->deprecated);
	}
	if (m->java_string_check_utf8) {
		p = pb_put_tag_2(p, 472);
		p = pb_put_bool(p,  m->java_string_check_utf8);
	}
	if (m->cc_enable_arenas) {
		p = pb_put_tag_2(p, 504);
		p = pb_put_bool(p,  m->cc_enable_arenas);
	}
	if (m->objc_class_prefix.len) {
		p = pb_put_tag_2(p, 674);
		p = pb_put_string(p, *(struct pb_string*)& m->objc_class_prefix);
	}
	if (m->csharp_namespace.len) {
		p = pb_put_tag_2(p, 682);
		p = pb_put_string(p, *(struct pb_string*)& m->csharp_namespace);
	}
	if (m->uninterpreted_option.len) {
		for (int i = 0; i < m->uninterpreted_option.len; i++) {
			p = pb_put_tag_2(p, 16058);
			int szlen = pb_u32_size(m->uninterpreted_option.v[i]->pb_hdr.maxsz);
			char *start = p+szlen;
			p = pb_encode_UninterpretedOption(start, m->uninterpreted_option.v[i]);
			pb_put_finish(p, start, szlen);
		}
	}
	return p;
}
int pb_maxsz_FileOptions(struct FileOptions const *m) {
	int n = 0;
	if (m->java_package.len) {
		n += 1 + 5 + m->java_package.len;
	}
	if (m->java_outer_classname.len) {
		n += 1 + 5 + m->java_outer_classname.len;
	}
	if (m->optimize_for) {
		n += 1 + 5;
	}
	if (m->java_multiple_files) {
		n += 1 + 1;
	}
	if (m->go_package.len) {
		n += 1 + 5 + m->go_package.len;
	}
	if (m->cc_generic_services) {
		n += 2 + 1;
	}
	if (m->java_generic_services) {
		n += 2 + 1;
	}
	if (m->py_generic_services) {
		n += 2 + 1;
	}
	if (m->java_generate_equals_and_hash) {
		n += 2 + 1;
	}
	if (m->deprecated) {
		n += 2 + 1;
	}
	if (m->java_string_check_utf8) {
		n += 2 + 1;
	}
	if (m->cc_enable_arenas) {
		n += 2 + 1;
	}
	if (m->objc_class_prefix.len) {
		n += 2 + 5 + m->objc_class_prefix.len;
	}
	if (m->csharp_namespace.len) {
		n += 2 + 5 + m->csharp_namespace.len;
	}
	if (m->uninterpreted_option.len) {
		for (int i = 0; i < m->uninterpreted_option.len; i++) {
			n += 2 + pb_maxsz_UninterpretedOption(m->uninterpreted_option.v[i]);
		}
	}
	n += pb_u32_size(n);
	((union pb_msg*) m)->maxsz = n;
	return n;
}

char *pb_print_FileOptions_OptimizeMode(char *p, enum FileOptions_OptimizeMode v){
	switch ((int32_t) v) {
	case 1:
		memcpy(p, "\"SPEED\",", 8);
		return p + 8;
	case 2:
		memcpy(p, "\"CODE_SIZE\",", 12);
		return p + 12;
	case 3:
		memcpy(p, "\"LITE_RUNTIME\",", 15);
		return p + 15;
	default:
		memcpy(p, "\"\",", 3);
		return p + 3;
	}
}
char *pb_parse_FileOptions_OptimizeMode(char *p, enum FileOptions_OptimizeMode *v) {
	struct pb_string val;
	*v = (enum FileOptions_OptimizeMode) 0;
	switch (pb_parse_enum(&p, &val, 121) % 9) {
	case 8:
		if (!pb_cmp(val, "SPEED")) {
			*v = (enum FileOptions_OptimizeMode) 1;
		}
		break;
	case 1:
		if (!pb_cmp(val, "CODE_SIZE")) {
			*v = (enum FileOptions_OptimizeMode) 2;
		}
		break;
	case 3:
		if (!pb_cmp(val, "LITE_RUNTIME")) {
			*v = (enum FileOptions_OptimizeMode) 3;
		}
		break;
	}
	return p;
}

char *pb_parse_MessageOptions(char *p, pb_alloc_t *a, struct MessageOptions *m) {
	(void) a;
	(void) m;
	if (!pb_parse_map(&p)) {
		return p;
	}
	for (;;) {
		struct pb_string key;
		switch (pb_parse_field(&p, &key, 64570081) % 23) {
		case 0:
			return p;
		case 18:
			if (pb_cmp(key, "message_set_wire_format")) {
				goto unknown;
			}
			p = pb_parse_bool(p, &m->message_set_wire_format);
			continue;
		case 4:
			if (pb_cmp(key, "no_standard_descriptor_accessor")) {
				goto unknown;
			}
			p = pb_parse_bool(p, &m->no_standard_descriptor_accessor);
			continue;
		case 7:
			if (pb_cmp(key, "deprecated")) {
				goto unknown;
			}
			p = pb_parse_bool(p, &m->deprecated);
			continue;
		case 20:
			if (pb_cmp(key, "map_entry")) {
				goto unknown;
			}
			p = pb_parse_bool(p, &m->map_entry);
			continue;
		case 9:
			if (pb_cmp(key, "uninterpreted_option")) {
				goto unknown;
			}
			if (pb_parse_array(&p)) {
				struct UninterpretedOption *prev = NULL;
				do {
					struct UninterpretedOption *c = (struct UninterpretedOption*) pb_calloc(a, 1, sizeof(struct UninterpretedOption));
					p = pb_parse_UninterpretedOption(p, a, c);
					m->uninterpreted_option.len++;
					c->pb_hdr.prev = prev;
					prev = c;
				} while (pb_more_array(&p));

				m->uninterpreted_option.v = (const struct UninterpretedOption**) pb_calloc(a, m->uninterpreted_option.len, sizeof(struct UninterpretedOption*));

				for (int i = m->uninterpreted_option.len - 1; i >= 0; i--) {
					m->uninterpreted_option.v[i] = prev;
					prev = (struct UninterpretedOption*) prev->pb_hdr.prev;
				}
			}
			continue;
		default:
		unknown:
			p = pb_parse_skip(p);
		}
	}
}
int pb_fparse_MessageOptions(const char *fn, str_t *b, pb_alloc_t *a, struct MessageOptions *m) {
	int off = b->len;
	str_read_file(b, fn);
	return pb_parse_MessageOptions(b->buf + off, a, m) == pb_errret ? -1 : 0; 
}
void pb_print_MessageOptions(str_t *a, struct MessageOptions const *m) {
	str_grow(a, a->len + 110);
	char *p = a->buf + a->len;
	*p++ = '{';
	if (m->message_set_wire_format) {
		memcpy(p, "\"message_set_wire_format\":", 26);
		p += 26;
		p = pb_print_bool(p, m->message_set_wire_format);
	}
	if (m->no_standard_descriptor_accessor) {
		memcpy(p, "\"no_standard_descriptor_accessor\":", 34);
		p += 34;
		p = pb_print_bool(p, m->no_standard_descriptor_accessor);
	}
	if (m->deprecated) {
		memcpy(p, "\"deprecated\":", 13);
		p += 13;
		p = pb_print_bool(p, m->deprecated);
	}
	if (m->map_entry) {
		memcpy(p, "\"map_entry\":", 12);
		p += 12;
		p = pb_print_bool(p, m->map_entry);
	}
	str_setlen(a, (unsigned) (p - a->buf));
	if (m->uninterpreted_option.len) {
		str_add(a, "\"uninterpreted_option\":");
		str_addch(a, '[');
		for (int i = 0; i < m->uninterpreted_option.len; i++) {
			pb_print_UninterpretedOption(a, m->uninterpreted_option.v[i]);
			a->buf[a->len-1] = ','; // replace trailing newline
		}
		pb_print_array_end(a);
	}
	pb_print_map_end(a);
}
void pb_get_MessageOptions(const char *p, const char *e, pb_alloc_t *obj, struct MessageOptions *m) {
	(void) obj;
	if (pb_skipto_1(&p, e, 8)) {
		p = pb_get_bool(p + 1, e, &m->message_set_wire_format);
	}
	if (pb_skipto_1(&p, e, 16)) {
		p = pb_get_bool(p + 1, e, &m->no_standard_descriptor_accessor);
	}
	if (pb_skipto_1(&p, e, 24)) {
		p = pb_get_bool(p + 1, e, &m->deprecated);
	}
	if (pb_skipto_1(&p, e, 56)) {
		p = pb_get_bool(p + 1, e, &m->map_entry);
	}
	p = pb_toend_1(p,e);
	if (pb_skipto_2(&p, e, 16058)) {
		struct UninterpretedOption *prev = NULL;
		do {
			struct pb_string msg;
			p = pb_get_string(p + 2, e, &msg);
			struct UninterpretedOption *c = (struct UninterpretedOption*) pb_calloc(obj, 1, sizeof(struct UninterpretedOption));
			pb_get_UninterpretedOption(msg.p, msg.p + msg.len, obj, c);
			m->uninterpreted_option.len++;
			c->pb_hdr.prev = prev;
			prev = c;
		} while (!pb_cmp_tag_2(p, e, 16058));

		m->uninterpreted_option.v = (const struct UninterpretedOption**) pb_calloc(obj, m->uninterpreted_option.len, sizeof(struct UninterpretedOption*));

		for (int i = m->uninterpreted_option.len - 1; i >= 0; i--) {
			m->uninterpreted_option.v[i] = prev;
			prev = (struct UninterpretedOption*) prev->pb_hdr.prev;
		}
	}
}
char *pb_encode_MessageOptions(char *p, struct MessageOptions const *m) {
	if (m->message_set_wire_format) {
		p = pb_put_tag_1(p, 8);
		p = pb_put_bool(p,  m->message_set_wire_format);
	}
	if (m->no_standard_descriptor_accessor) {
		p = pb_put_tag_1(p, 16);
		p = pb_put_bool(p,  m->no_standard_descriptor_accessor);
	}
	if (m->deprecated) {
		p = pb_put_tag_1(p, 24);
		p = pb_put_bool(p,  m->deprecated);
	}
	if (m->map_entry) {
		p = pb_put_tag_1(p, 56);
		p = pb_put_bool(p,  m->map_entry);
	}
	if (m->uninterpreted_option.len) {
		for (int i = 0; i < m->uninterpreted_option.len; i++) {
			p = pb_put_tag_2(p, 16058);
			int szlen = pb_u32_size(m->uninterpreted_option.v[i]->pb_hdr.maxsz);
			char *start = p+szlen;
			p = pb_encode_UninterpretedOption(start, m->uninterpreted_option.v[i]);
			pb_put_finish(p, start, szlen);
		}
	}
	return p;
}
int pb_maxsz_MessageOptions(struct MessageOptions const *m) {
	int n = 0;
	if (m->message_set_wire_format) {
		n += 1 + 1;
	}
	if (m->no_standard_descriptor_accessor) {
		n += 1 + 1;
	}
	if (m->deprecated) {
		n += 1 + 1;
	}
	if (m->map_entry) {
		n += 1 + 1;
	}
	if (m->uninterpreted_option.len) {
		for (int i = 0; i < m->uninterpreted_option.len; i++) {
			n += 2 + pb_maxsz_UninterpretedOption(m->uninterpreted_option.v[i]);
		}
	}
	n += pb_u32_size(n);
	((union pb_msg*) m)->maxsz = n;
	return n;
}

char *pb_parse_FieldOptions(char *p, pb_alloc_t *a, struct FieldOptions *m) {
	(void) a;
	(void) m;
	if (!pb_parse_map(&p)) {
		return p;
	}
	for (;;) {
		struct pb_string key;
		switch (pb_parse_field(&p, &key, 3280) % 17) {
		case 0:
			return p;
		case 12:
			if (pb_cmp(key, "ctype")) {
				goto unknown;
			}
			p = pb_parse_FieldOptions_CType(p, &m->ctype);
			continue;
		case 15:
			if (pb_cmp(key, "packed")) {
				goto unknown;
			}
			p = pb_parse_bool(p, &m->packed);
			continue;
		case 3:
			if (pb_cmp(key, "jstype")) {
				goto unknown;
			}
			p = pb_parse_FieldOptions_JSType(p, &m->jstype);
			continue;
		case 2:
			if (pb_cmp(key, "lazy")) {
				goto unknown;
			}
			p = pb_parse_bool(p, &m->lazy);
			continue;
		case 10:
			if (pb_cmp(key, "deprecated")) {
				goto unknown;
			}
			p = pb_parse_bool(p, &m->deprecated);
			continue;
		case 1:
			if (pb_cmp(key, "weak")) {
				goto unknown;
			}
			p = pb_parse_bool(p, &m->weak);
			continue;
		case 11:
			if (pb_cmp(key, "uninterpreted_option")) {
				goto unknown;
			}
			if (pb_parse_array(&p)) {
				struct UninterpretedOption *prev = NULL;
				do {
					struct UninterpretedOption *c = (struct UninterpretedOption*) pb_calloc(a, 1, sizeof(struct UninterpretedOption));
					p = pb_parse_UninterpretedOption(p, a, c);
					m->uninterpreted_option.len++;
					c->pb_hdr.prev = prev;
					prev = c;
				} while (pb_more_array(&p));

				m->uninterpreted_option.v = (const struct UninterpretedOption**) pb_calloc(a, m->uninterpreted_option.len, sizeof(struct UninterpretedOption*));

				for (int i = m->uninterpreted_option.len - 1; i >= 0; i--) {
					m->uninterpreted_option.v[i] = prev;
					prev = (struct UninterpretedOption*) prev->pb_hdr.prev;
				}
			}
			continue;
		case 13:
			if (pb_cmp(key, "array")) {
				goto unknown;
			}
			p = pb_parse_u32(p, &m->array);
			continue;
		default:
		unknown:
			p = pb_parse_skip(p);
		}
	}
}
int pb_fparse_FieldOptions(const char *fn, str_t *b, pb_alloc_t *a, struct FieldOptions *m) {
	int off = b->len;
	str_read_file(b, fn);
	return pb_parse_FieldOptions(b->buf + off, a, m) == pb_errret ? -1 : 0; 
}
void pb_print_FieldOptions(str_t *a, struct FieldOptions const *m) {
	str_grow(a, a->len + 127);
	char *p = a->buf + a->len;
	*p++ = '{';
	if (m->ctype) {
		memcpy(p, "\"ctype\":", 8);
		p += 8;
		p = pb_print_FieldOptions_CType(p, m->ctype);
	}
	if (m->packed) {
		memcpy(p, "\"packed\":", 9);
		p += 9;
		p = pb_print_bool(p, m->packed);
	}
	if (m->jstype) {
		memcpy(p, "\"jstype\":", 9);
		p += 9;
		p = pb_print_FieldOptions_JSType(p, m->jstype);
	}
	if (m->lazy) {
		memcpy(p, "\"lazy\":", 7);
		p += 7;
		p = pb_print_bool(p, m->lazy);
	}
	if (m->deprecated) {
		memcpy(p, "\"deprecated\":", 13);
		p += 13;
		p = pb_print_bool(p, m->deprecated);
	}
	if (m->weak) {
		memcpy(p, "\"weak\":", 7);
		p += 7;
		p = pb_print_bool(p, m->weak);
	}
	if (m->array) {
		memcpy(p, "\"array\":", 8);
		p += 8;
		p = pb_print_u32(p, m->array);
	}
	str_setlen(a, (unsigned) (p - a->buf));
	if (m->uninterpreted_option.len) {
		str_add(a, "\"uninterpreted_option\":");
		str_addch(a, '[');
		for (int i = 0; i < m->uninterpreted_option.len; i++) {
			pb_print_UninterpretedOption(a, m->uninterpreted_option.v[i]);
			a->buf[a->len-1] = ','; // replace trailing newline
		}
		pb_print_array_end(a);
	}
	pb_print_map_end(a);
}
void pb_get_FieldOptions(const char *p, const char *e, pb_alloc_t *obj, struct FieldOptions *m) {
	(void) obj;
	if (pb_skipto_1(&p, e, 8)) {
		p = pb_get_u32(p + 1, e, (uint32_t*)&m->ctype);
	}
	if (pb_skipto_1(&p, e, 16)) {
		m->packed_set = true;
		p = pb_get_bool(p + 1, e, &m->packed);
	}
	if (pb_skipto_1(&p, e, 24)) {
		p = pb_get_bool(p + 1, e, &m->deprecated);
	}
	if (pb_skipto_1(&p, e, 40)) {
		p = pb_get_bool(p + 1, e, &m->lazy);
	}
	if (pb_skipto_1(&p, e, 48)) {
		p = pb_get_u32(p + 1, e, (uint32_t*)&m->jstype);
	}
	if (pb_skipto_1(&p, e, 80)) {
		p = pb_get_bool(p + 1, e, &m->weak);
	}
	p = pb_toend_1(p,e);
	if (pb_skipto_2(&p, e, 16058)) {
		struct UninterpretedOption *prev = NULL;
		do {
			struct pb_string msg;
			p = pb_get_string(p + 2, e, &msg);
			struct UninterpretedOption *c = (struct UninterpretedOption*) pb_calloc(obj, 1, sizeof(struct UninterpretedOption));
			pb_get_UninterpretedOption(msg.p, msg.p + msg.len, obj, c);
			m->uninterpreted_option.len++;
			c->pb_hdr.prev = prev;
			prev = c;
		} while (!pb_cmp_tag_2(p, e, 16058));

		m->uninterpreted_option.v = (const struct UninterpretedOption**) pb_calloc(obj, m->uninterpreted_option.len, sizeof(struct UninterpretedOption*));

		for (int i = m->uninterpreted_option.len - 1; i >= 0; i--) {
			m->uninterpreted_option.v[i] = prev;
			prev = (struct UninterpretedOption*) prev->pb_hdr.prev;
		}
	}
	p = pb_toend_2(p,e);
	if (pb_skipto_3(&p, e, 1619328)) {
		p = pb_get_u32(p + 3, e, &m->array);
	}
}
char *pb_encode_FieldOptions(char *p, struct FieldOptions const *m) {
	if (m->ctype) {
		p = pb_put_tag_1(p, 8);
		p = pb_put_u32(p, *(uint32_t*)& m->ctype);
	}
	if (m->packed) {
		p = pb_put_tag_1(p, 16);
		p = pb_put_bool(p,  m->packed);
	}
	if (m->deprecated) {
		p = pb_put_tag_1(p, 24);
		p = pb_put_bool(p,  m->deprecated);
	}
	if (m->lazy) {
		p = pb_put_tag_1(p, 40);
		p = pb_put_bool(p,  m->lazy);
	}
	if (m->jstype) {
		p = pb_put_tag_1(p, 48);
		p = pb_put_u32(p, *(uint32_t*)& m->jstype);
	}
	if (m->weak) {
		p = pb_put_tag_1(p, 80);
		p = pb_put_bool(p,  m->weak);
	}
	if (m->uninterpreted_option.len) {
		for (int i = 0; i < m->uninterpreted_option.len; i++) {
			p = pb_put_tag_2(p, 16058);
			int szlen = pb_u32_size(m->uninterpreted_option.v[i]->pb_hdr.maxsz);
			char *start = p+szlen;
			p = pb_encode_UninterpretedOption(start, m->uninterpreted_option.v[i]);
			pb_put_finish(p, start, szlen);
		}
	}
	if (m->array) {
		p = pb_put_tag_3(p, 1619328);
		p = pb_put_u32(p,  m->array);
	}
	return p;
}
int pb_maxsz_FieldOptions(struct FieldOptions const *m) {
	int n = 0;
	if (m->ctype) {
		n += 1 + 5;
	}
	if (m->packed) {
		n += 1 + 1;
	}
	if (m->deprecated) {
		n += 1 + 1;
	}
	if (m->lazy) {
		n += 1 + 1;
	}
	if (m->jstype) {
		n += 1 + 5;
	}
	if (m->weak) {
		n += 1 + 1;
	}
	if (m->uninterpreted_option.len) {
		for (int i = 0; i < m->uninterpreted_option.len; i++) {
			n += 2 + pb_maxsz_UninterpretedOption(m->uninterpreted_option.v[i]);
		}
	}
	if (m->array) {
		n += 3 + 5;
	}
	n += pb_u32_size(n);
	((union pb_msg*) m)->maxsz = n;
	return n;
}

char *pb_print_FieldOptions_CType(char *p, enum FieldOptions_CType v){
	switch ((int32_t) v) {
	case 0:
		memcpy(p, "\"STRING\",", 9);
		return p + 9;
	case 1:
		memcpy(p, "\"CORD\",", 7);
		return p + 7;
	case 2:
		memcpy(p, "\"STRING_PIECE\",", 15);
		return p + 15;
	default:
		memcpy(p, "\"\",", 3);
		return p + 3;
	}
}
char *pb_parse_FieldOptions_CType(char *p, enum FieldOptions_CType *v) {
	struct pb_string val;
	*v = (enum FieldOptions_CType) 0;
	switch (pb_parse_enum(&p, &val, 40) % 8) {
	case 7:
		if (!pb_cmp(val, "STRING")) {
			*v = (enum FieldOptions_CType) 0;
		}
		break;
	case 4:
		if (!pb_cmp(val, "CORD")) {
			*v = (enum FieldOptions_CType) 1;
		}
		break;
	case 5:
		if (!pb_cmp(val, "STRING_PIECE")) {
			*v = (enum FieldOptions_CType) 2;
		}
		break;
	}
	return p;
}

char *pb_print_FieldOptions_JSType(char *p, enum FieldOptions_JSType v){
	switch ((int32_t) v) {
	case 0:
		memcpy(p, "\"JS_NORMAL\",", 12);
		return p + 12;
	case 1:
		memcpy(p, "\"JS_STRING\",", 12);
		return p + 12;
	case 2:
		memcpy(p, "\"JS_NUMBER\",", 12);
		return p + 12;
	default:
		memcpy(p, "\"\",", 3);
		return p + 3;
	}
}
char *pb_parse_FieldOptions_JSType(char *p, enum FieldOptions_JSType *v) {
	struct pb_string val;
	*v = (enum FieldOptions_JSType) 0;
	switch (pb_parse_enum(&p, &val, 40) % 8) {
	case 4:
		if (!pb_cmp(val, "JS_NORMAL")) {
			*v = (enum FieldOptions_JSType) 0;
		}
		break;
	case 7:
		if (!pb_cmp(val, "JS_STRING")) {
			*v = (enum FieldOptions_JSType) 1;
		}
		break;
	case 2:
		if (!pb_cmp(val, "JS_NUMBER")) {
			*v = (enum FieldOptions_JSType) 2;
		}
		break;
	}
	return p;
}

char *pb_parse_OneofOptions(char *p, pb_alloc_t *a, struct OneofOptions *m) {
	(void) a;
	(void) m;
	if (!pb_parse_map(&p)) {
		return p;
	}
	for (;;) {
		struct pb_string key;
		switch (pb_parse_field(&p, &key, 1) % 3) {
		case 0:
			return p;
		case 2:
			if (pb_cmp(key, "uninterpreted_option")) {
				goto unknown;
			}
			if (pb_parse_array(&p)) {
				struct UninterpretedOption *prev = NULL;
				do {
					struct UninterpretedOption *c = (struct UninterpretedOption*) pb_calloc(a, 1, sizeof(struct UninterpretedOption));
					p = pb_parse_UninterpretedOption(p, a, c);
					m->uninterpreted_option.len++;
					c->pb_hdr.prev = prev;
					prev = c;
				} while (pb_more_array(&p));

				m->uninterpreted_option.v = (const struct UninterpretedOption**) pb_calloc(a, m->uninterpreted_option.len, sizeof(struct UninterpretedOption*));

				for (int i = m->uninterpreted_option.len - 1; i >= 0; i--) {
					m->uninterpreted_option.v[i] = prev;
					prev = (struct UninterpretedOption*) prev->pb_hdr.prev;
				}
			}
			continue;
		default:
		unknown:
			p = pb_parse_skip(p);
		}
	}
}
int pb_fparse_OneofOptions(const char *fn, str_t *b, pb_alloc_t *a, struct OneofOptions *m) {
	int off = b->len;
	str_read_file(b, fn);
	return pb_parse_OneofOptions(b->buf + off, a, m) == pb_errret ? -1 : 0; 
}
void pb_print_OneofOptions(str_t *a, struct OneofOptions const *m) {
	str_grow(a, a->len + 1);
	char *p = a->buf + a->len;
	*p++ = '{';
	str_setlen(a, (unsigned) (p - a->buf));
	if (m->uninterpreted_option.len) {
		str_add(a, "\"uninterpreted_option\":");
		str_addch(a, '[');
		for (int i = 0; i < m->uninterpreted_option.len; i++) {
			pb_print_UninterpretedOption(a, m->uninterpreted_option.v[i]);
			a->buf[a->len-1] = ','; // replace trailing newline
		}
		pb_print_array_end(a);
	}
	pb_print_map_end(a);
}
void pb_get_OneofOptions(const char *p, const char *e, pb_alloc_t *obj, struct OneofOptions *m) {
	(void) obj;
	p = pb_toend_1(p,e);
	if (pb_skipto_2(&p, e, 16058)) {
		struct UninterpretedOption *prev = NULL;
		do {
			struct pb_string msg;
			p = pb_get_string(p + 2, e, &msg);
			struct UninterpretedOption *c = (struct UninterpretedOption*) pb_calloc(obj, 1, sizeof(struct UninterpretedOption));
			pb_get_UninterpretedOption(msg.p, msg.p + msg.len, obj, c);
			m->uninterpreted_option.len++;
			c->pb_hdr.prev = prev;
			prev = c;
		} while (!pb_cmp_tag_2(p, e, 16058));

		m->uninterpreted_option.v = (const struct UninterpretedOption**) pb_calloc(obj, m->uninterpreted_option.len, sizeof(struct UninterpretedOption*));

		for (int i = m->uninterpreted_option.len - 1; i >= 0; i--) {
			m->uninterpreted_option.v[i] = prev;
			prev = (struct UninterpretedOption*) prev->pb_hdr.prev;
		}
	}
}
char *pb_encode_OneofOptions(char *p, struct OneofOptions const *m) {
	if (m->uninterpreted_option.len) {
		for (int i = 0; i < m->uninterpreted_option.len; i++) {
			p = pb_put_tag_2(p, 16058);
			int szlen = pb_u32_size(m->uninterpreted_option.v[i]->pb_hdr.maxsz);
			char *start = p+szlen;
			p = pb_encode_UninterpretedOption(start, m->uninterpreted_option.v[i]);
			pb_put_finish(p, start, szlen);
		}
	}
	return p;
}
int pb_maxsz_OneofOptions(struct OneofOptions const *m) {
	int n = 0;
	if (m->uninterpreted_option.len) {
		for (int i = 0; i < m->uninterpreted_option.len; i++) {
			n += 2 + pb_maxsz_UninterpretedOption(m->uninterpreted_option.v[i]);
		}
	}
	n += pb_u32_size(n);
	((union pb_msg*) m)->maxsz = n;
	return n;
}

char *pb_parse_EnumOptions(char *p, pb_alloc_t *a, struct EnumOptions *m) {
	(void) a;
	(void) m;
	if (!pb_parse_map(&p)) {
		return p;
	}
	for (;;) {
		struct pb_string key;
		switch (pb_parse_field(&p, &key, 40) % 8) {
		case 0:
			return p;
		case 3:
			if (pb_cmp(key, "allow_alias")) {
				goto unknown;
			}
			p = pb_parse_bool(p, &m->allow_alias);
			continue;
		case 4:
			if (pb_cmp(key, "deprecated")) {
				goto unknown;
			}
			p = pb_parse_bool(p, &m->deprecated);
			continue;
		case 6:
			if (pb_cmp(key, "uninterpreted_option")) {
				goto unknown;
			}
			if (pb_parse_array(&p)) {
				struct UninterpretedOption *prev = NULL;
				do {
					struct UninterpretedOption *c = (struct UninterpretedOption*) pb_calloc(a, 1, sizeof(struct UninterpretedOption));
					p = pb_parse_UninterpretedOption(p, a, c);
					m->uninterpreted_option.len++;
					c->pb_hdr.prev = prev;
					prev = c;
				} while (pb_more_array(&p));

				m->uninterpreted_option.v = (const struct UninterpretedOption**) pb_calloc(a, m->uninterpreted_option.len, sizeof(struct UninterpretedOption*));

				for (int i = m->uninterpreted_option.len - 1; i >= 0; i--) {
					m->uninterpreted_option.v[i] = prev;
					prev = (struct UninterpretedOption*) prev->pb_hdr.prev;
				}
			}
			continue;
		default:
		unknown:
			p = pb_parse_skip(p);
		}
	}
}
int pb_fparse_EnumOptions(const char *fn, str_t *b, pb_alloc_t *a, struct EnumOptions *m) {
	int off = b->len;
	str_read_file(b, fn);
	return pb_parse_EnumOptions(b->buf + off, a, m) == pb_errret ? -1 : 0; 
}
void pb_print_EnumOptions(str_t *a, struct EnumOptions const *m) {
	str_grow(a, a->len + 40);
	char *p = a->buf + a->len;
	*p++ = '{';
	if (m->allow_alias) {
		memcpy(p, "\"allow_alias\":", 14);
		p += 14;
		p = pb_print_bool(p, m->allow_alias);
	}
	if (m->deprecated) {
		memcpy(p, "\"deprecated\":", 13);
		p += 13;
		p = pb_print_bool(p, m->deprecated);
	}
	str_setlen(a, (unsigned) (p - a->buf));
	if (m->uninterpreted_option.len) {
		str_add(a, "\"uninterpreted_option\":");
		str_addch(a, '[');
		for (int i = 0; i < m->uninterpreted_option.len; i++) {
			pb_print_UninterpretedOption(a, m->uninterpreted_option.v[i]);
			a->buf[a->len-1] = ','; // replace trailing newline
		}
		pb_print_array_end(a);
	}
	pb_print_map_end(a);
}
void pb_get_EnumOptions(const char *p, const char *e, pb_alloc_t *obj, struct EnumOptions *m) {
	(void) obj;
	if (pb_skipto_1(&p, e, 16)) {
		p = pb_get_bool(p + 1, e, &m->allow_alias);
	}
	if (pb_skipto_1(&p, e, 24)) {
		p = pb_get_bool(p + 1, e, &m->deprecated);
	}
	p = pb_toend_1(p,e);
	if (pb_skipto_2(&p, e, 16058)) {
		struct UninterpretedOption *prev = NULL;
		do {
			struct pb_string msg;
			p = pb_get_string(p + 2, e, &msg);
			struct UninterpretedOption *c = (struct UninterpretedOption*) pb_calloc(obj, 1, sizeof(struct UninterpretedOption));
			pb_get_UninterpretedOption(msg.p, msg.p + msg.len, obj, c);
			m->uninterpreted_option.len++;
			c->pb_hdr.prev = prev;
			prev = c;
		} while (!pb_cmp_tag_2(p, e, 16058));

		m->uninterpreted_option.v = (const struct UninterpretedOption**) pb_calloc(obj, m->uninterpreted_option.len, sizeof(struct UninterpretedOption*));

		for (int i = m->uninterpreted_option.len - 1; i >= 0; i--) {
			m->uninterpreted_option.v[i] = prev;
			prev = (struct UninterpretedOption*) prev->pb_hdr.prev;
		}
	}
}
char *pb_encode_EnumOptions(char *p, struct EnumOptions const *m) {
	if (m->allow_alias) {
		p = pb_put_tag_1(p, 16);
		p = pb_put_bool(p,  m->allow_alias);
	}
	if (m->deprecated) {
		p = pb_put_tag_1(p, 24);
		p = pb_put_bool(p,  m->deprecated);
	}
	if (m->uninterpreted_option.len) {
		for (int i = 0; i < m->uninterpreted_option.len; i++) {
			p = pb_put_tag_2(p, 16058);
			int szlen = pb_u32_size(m->uninterpreted_option.v[i]->pb_hdr.maxsz);
			char *start = p+szlen;
			p = pb_encode_UninterpretedOption(start, m->uninterpreted_option.v[i]);
			pb_put_finish(p, start, szlen);
		}
	}
	return p;
}
int pb_maxsz_EnumOptions(struct EnumOptions const *m) {
	int n = 0;
	if (m->allow_alias) {
		n += 1 + 1;
	}
	if (m->deprecated) {
		n += 1 + 1;
	}
	if (m->uninterpreted_option.len) {
		for (int i = 0; i < m->uninterpreted_option.len; i++) {
			n += 2 + pb_maxsz_UninterpretedOption(m->uninterpreted_option.v[i]);
		}
	}
	n += pb_u32_size(n);
	((union pb_msg*) m)->maxsz = n;
	return n;
}

char *pb_parse_EnumValueOptions(char *p, pb_alloc_t *a, struct EnumValueOptions *m) {
	(void) a;
	(void) m;
	if (!pb_parse_map(&p)) {
		return p;
	}
	for (;;) {
		struct pb_string key;
		switch (pb_parse_field(&p, &key, 4) % 5) {
		case 0:
			return p;
		case 4:
			if (pb_cmp(key, "deprecated")) {
				goto unknown;
			}
			p = pb_parse_bool(p, &m->deprecated);
			continue;
		case 2:
			if (pb_cmp(key, "uninterpreted_option")) {
				goto unknown;
			}
			if (pb_parse_array(&p)) {
				struct UninterpretedOption *prev = NULL;
				do {
					struct UninterpretedOption *c = (struct UninterpretedOption*) pb_calloc(a, 1, sizeof(struct UninterpretedOption));
					p = pb_parse_UninterpretedOption(p, a, c);
					m->uninterpreted_option.len++;
					c->pb_hdr.prev = prev;
					prev = c;
				} while (pb_more_array(&p));

				m->uninterpreted_option.v = (const struct UninterpretedOption**) pb_calloc(a, m->uninterpreted_option.len, sizeof(struct UninterpretedOption*));

				for (int i = m->uninterpreted_option.len - 1; i >= 0; i--) {
					m->uninterpreted_option.v[i] = prev;
					prev = (struct UninterpretedOption*) prev->pb_hdr.prev;
				}
			}
			continue;
		default:
		unknown:
			p = pb_parse_skip(p);
		}
	}
}
int pb_fparse_EnumValueOptions(const char *fn, str_t *b, pb_alloc_t *a, struct EnumValueOptions *m) {
	int off = b->len;
	str_read_file(b, fn);
	return pb_parse_EnumValueOptions(b->buf + off, a, m) == pb_errret ? -1 : 0; 
}
void pb_print_EnumValueOptions(str_t *a, struct EnumValueOptions const *m) {
	str_grow(a, a->len + 20);
	char *p = a->buf + a->len;
	*p++ = '{';
	if (m->deprecated) {
		memcpy(p, "\"deprecated\":", 13);
		p += 13;
		p = pb_print_bool(p, m->deprecated);
	}
	str_setlen(a, (unsigned) (p - a->buf));
	if (m->uninterpreted_option.len) {
		str_add(a, "\"uninterpreted_option\":");
		str_addch(a, '[');
		for (int i = 0; i < m->uninterpreted_option.len; i++) {
			pb_print_UninterpretedOption(a, m->uninterpreted_option.v[i]);
			a->buf[a->len-1] = ','; // replace trailing newline
		}
		pb_print_array_end(a);
	}
	pb_print_map_end(a);
}
void pb_get_EnumValueOptions(const char *p, const char *e, pb_alloc_t *obj, struct EnumValueOptions *m) {
	(void) obj;
	if (pb_skipto_1(&p, e, 8)) {
		p = pb_get_bool(p + 1, e, &m->deprecated);
	}
	p = pb_toend_1(p,e);
	if (pb_skipto_2(&p, e, 16058)) {
		struct UninterpretedOption *prev = NULL;
		do {
			struct pb_string msg;
			p = pb_get_string(p + 2, e, &msg);
			struct UninterpretedOption *c = (struct UninterpretedOption*) pb_calloc(obj, 1, sizeof(struct UninterpretedOption));
			pb_get_UninterpretedOption(msg.p, msg.p + msg.len, obj, c);
			m->uninterpreted_option.len++;
			c->pb_hdr.prev = prev;
			prev = c;
		} while (!pb_cmp_tag_2(p, e, 16058));

		m->uninterpreted_option.v = (const struct UninterpretedOption**) pb_calloc(obj, m->uninterpreted_option.len, sizeof(struct UninterpretedOption*));

		for (int i = m->uninterpreted_option.len - 1; i >= 0; i--) {
			m->uninterpreted_option.v[i] = prev;
			prev = (struct UninterpretedOption*) prev->pb_hdr.prev;
		}
	}
}
char *pb_encode_EnumValueOptions(char *p, struct EnumValueOptions const *m) {
	if (m->deprecated) {
		p = pb_put_tag_1(p, 8);
		p = pb_put_bool(p,  m->deprecated);
	}
	if (m->uninterpreted_option.len) {
		for (int i = 0; i < m->uninterpreted_option.len; i++) {
			p = pb_put_tag_2(p, 16058);
			int szlen = pb_u32_size(m->uninterpreted_option.v[i]->pb_hdr.maxsz);
			char *start = p+szlen;
			p = pb_encode_UninterpretedOption(start, m->uninterpreted_option.v[i]);
			pb_put_finish(p, start, szlen);
		}
	}
	return p;
}
int pb_maxsz_EnumValueOptions(struct EnumValueOptions const *m) {
	int n = 0;
	if (m->deprecated) {
		n += 1 + 1;
	}
	if (m->uninterpreted_option.len) {
		for (int i = 0; i < m->uninterpreted_option.len; i++) {
			n += 2 + pb_maxsz_UninterpretedOption(m->uninterpreted_option.v[i]);
		}
	}
	n += pb_u32_size(n);
	((union pb_msg*) m)->maxsz = n;
	return n;
}

char *pb_parse_ServiceOptions(char *p, pb_alloc_t *a, struct ServiceOptions *m) {
	(void) a;
	(void) m;
	if (!pb_parse_map(&p)) {
		return p;
	}
	for (;;) {
		struct pb_string key;
		switch (pb_parse_field(&p, &key, 4) % 5) {
		case 0:
			return p;
		case 4:
			if (pb_cmp(key, "deprecated")) {
				goto unknown;
			}
			p = pb_parse_bool(p, &m->deprecated);
			continue;
		case 2:
			if (pb_cmp(key, "uninterpreted_option")) {
				goto unknown;
			}
			if (pb_parse_array(&p)) {
				struct UninterpretedOption *prev = NULL;
				do {
					struct UninterpretedOption *c = (struct UninterpretedOption*) pb_calloc(a, 1, sizeof(struct UninterpretedOption));
					p = pb_parse_UninterpretedOption(p, a, c);
					m->uninterpreted_option.len++;
					c->pb_hdr.prev = prev;
					prev = c;
				} while (pb_more_array(&p));

				m->uninterpreted_option.v = (const struct UninterpretedOption**) pb_calloc(a, m->uninterpreted_option.len, sizeof(struct UninterpretedOption*));

				for (int i = m->uninterpreted_option.len - 1; i >= 0; i--) {
					m->uninterpreted_option.v[i] = prev;
					prev = (struct UninterpretedOption*) prev->pb_hdr.prev;
				}
			}
			continue;
		default:
		unknown:
			p = pb_parse_skip(p);
		}
	}
}
int pb_fparse_ServiceOptions(const char *fn, str_t *b, pb_alloc_t *a, struct ServiceOptions *m) {
	int off = b->len;
	str_read_file(b, fn);
	return pb_parse_ServiceOptions(b->buf + off, a, m) == pb_errret ? -1 : 0; 
}
void pb_print_ServiceOptions(str_t *a, struct ServiceOptions const *m) {
	str_grow(a, a->len + 20);
	char *p = a->buf + a->len;
	*p++ = '{';
	if (m->deprecated) {
		memcpy(p, "\"deprecated\":", 13);
		p += 13;
		p = pb_print_bool(p, m->deprecated);
	}
	str_setlen(a, (unsigned) (p - a->buf));
	if (m->uninterpreted_option.len) {
		str_add(a, "\"uninterpreted_option\":");
		str_addch(a, '[');
		for (int i = 0; i < m->uninterpreted_option.len; i++) {
			pb_print_UninterpretedOption(a, m->uninterpreted_option.v[i]);
			a->buf[a->len-1] = ','; // replace trailing newline
		}
		pb_print_array_end(a);
	}
	pb_print_map_end(a);
}
void pb_get_ServiceOptions(const char *p, const char *e, pb_alloc_t *obj, struct ServiceOptions *m) {
	(void) obj;
	p = pb_toend_1(p,e);
	if (pb_skipto_2(&p, e, 648)) {
		p = pb_get_bool(p + 2, e, &m->deprecated);
	}
	if (pb_skipto_2(&p, e, 16058)) {
		struct UninterpretedOption *prev = NULL;
		do {
			struct pb_string msg;
			p = pb_get_string(p + 2, e, &msg);
			struct UninterpretedOption *c = (struct UninterpretedOption*) pb_calloc(obj, 1, sizeof(struct UninterpretedOption));
			pb_get_UninterpretedOption(msg.p, msg.p + msg.len, obj, c);
			m->uninterpreted_option.len++;
			c->pb_hdr.prev = prev;
			prev = c;
		} while (!pb_cmp_tag_2(p, e, 16058));

		m->uninterpreted_option.v = (const struct UninterpretedOption**) pb_calloc(obj, m->uninterpreted_option.len, sizeof(struct UninterpretedOption*));

		for (int i = m->uninterpreted_option.len - 1; i >= 0; i--) {
			m->uninterpreted_option.v[i] = prev;
			prev = (struct UninterpretedOption*) prev->pb_hdr.prev;
		}
	}
}
char *pb_encode_ServiceOptions(char *p, struct ServiceOptions const *m) {
	if (m->deprecated) {
		p = pb_put_tag_2(p, 648);
		p = pb_put_bool(p,  m->deprecated);
	}
	if (m->uninterpreted_option.len) {
		for (int i = 0; i < m->uninterpreted_option.len; i++) {
			p = pb_put_tag_2(p, 16058);
			int szlen = pb_u32_size(m->uninterpreted_option.v[i]->pb_hdr.maxsz);
			char *start = p+szlen;
			p = pb_encode_UninterpretedOption(start, m->uninterpreted_option.v[i]);
			pb_put_finish(p, start, szlen);
		}
	}
	return p;
}
int pb_maxsz_ServiceOptions(struct ServiceOptions const *m) {
	int n = 0;
	if (m->deprecated) {
		n += 2 + 1;
	}
	if (m->uninterpreted_option.len) {
		for (int i = 0; i < m->uninterpreted_option.len; i++) {
			n += 2 + pb_maxsz_UninterpretedOption(m->uninterpreted_option.v[i]);
		}
	}
	n += pb_u32_size(n);
	((union pb_msg*) m)->maxsz = n;
	return n;
}

char *pb_parse_MethodOptions(char *p, pb_alloc_t *a, struct MethodOptions *m) {
	(void) a;
	(void) m;
	if (!pb_parse_map(&p)) {
		return p;
	}
	for (;;) {
		struct pb_string key;
		switch (pb_parse_field(&p, &key, 40) % 8) {
		case 0:
			return p;
		case 4:
			if (pb_cmp(key, "deprecated")) {
				goto unknown;
			}
			p = pb_parse_bool(p, &m->deprecated);
			continue;
		case 6:
			if (pb_cmp(key, "uninterpreted_option")) {
				goto unknown;
			}
			if (pb_parse_array(&p)) {
				struct UninterpretedOption *prev = NULL;
				do {
					struct UninterpretedOption *c = (struct UninterpretedOption*) pb_calloc(a, 1, sizeof(struct UninterpretedOption));
					p = pb_parse_UninterpretedOption(p, a, c);
					m->uninterpreted_option.len++;
					c->pb_hdr.prev = prev;
					prev = c;
				} while (pb_more_array(&p));

				m->uninterpreted_option.v = (const struct UninterpretedOption**) pb_calloc(a, m->uninterpreted_option.len, sizeof(struct UninterpretedOption*));

				for (int i = m->uninterpreted_option.len - 1; i >= 0; i--) {
					m->uninterpreted_option.v[i] = prev;
					prev = (struct UninterpretedOption*) prev->pb_hdr.prev;
				}
			}
			continue;
		case 1:
			if (pb_cmp(key, "binary")) {
				goto unknown;
			}
			p = pb_parse_bool(p, &m->binary);
			continue;
		default:
		unknown:
			p = pb_parse_skip(p);
		}
	}
}
int pb_fparse_MethodOptions(const char *fn, str_t *b, pb_alloc_t *a, struct MethodOptions *m) {
	int off = b->len;
	str_read_file(b, fn);
	return pb_parse_MethodOptions(b->buf + off, a, m) == pb_errret ? -1 : 0; 
}
void pb_print_MethodOptions(str_t *a, struct MethodOptions const *m) {
	str_grow(a, a->len + 35);
	char *p = a->buf + a->len;
	*p++ = '{';
	if (m->deprecated) {
		memcpy(p, "\"deprecated\":", 13);
		p += 13;
		p = pb_print_bool(p, m->deprecated);
	}
	if (m->binary) {
		memcpy(p, "\"binary\":", 9);
		p += 9;
		p = pb_print_bool(p, m->binary);
	}
	str_setlen(a, (unsigned) (p - a->buf));
	if (m->uninterpreted_option.len) {
		str_add(a, "\"uninterpreted_option\":");
		str_addch(a, '[');
		for (int i = 0; i < m->uninterpreted_option.len; i++) {
			pb_print_UninterpretedOption(a, m->uninterpreted_option.v[i]);
			a->buf[a->len-1] = ','; // replace trailing newline
		}
		pb_print_array_end(a);
	}
	pb_print_map_end(a);
}
void pb_get_MethodOptions(const char *p, const char *e, pb_alloc_t *obj, struct MethodOptions *m) {
	(void) obj;
	p = pb_toend_1(p,e);
	if (pb_skipto_2(&p, e, 648)) {
		p = pb_get_bool(p + 2, e, &m->deprecated);
	}
	if (pb_skipto_2(&p, e, 16058)) {
		struct UninterpretedOption *prev = NULL;
		do {
			struct pb_string msg;
			p = pb_get_string(p + 2, e, &msg);
			struct UninterpretedOption *c = (struct UninterpretedOption*) pb_calloc(obj, 1, sizeof(struct UninterpretedOption));
			pb_get_UninterpretedOption(msg.p, msg.p + msg.len, obj, c);
			m->uninterpreted_option.len++;
			c->pb_hdr.prev = prev;
			prev = c;
		} while (!pb_cmp_tag_2(p, e, 16058));

		m->uninterpreted_option.v = (const struct UninterpretedOption**) pb_calloc(obj, m->uninterpreted_option.len, sizeof(struct UninterpretedOption*));

		for (int i = m->uninterpreted_option.len - 1; i >= 0; i--) {
			m->uninterpreted_option.v[i] = prev;
			prev = (struct UninterpretedOption*) prev->pb_hdr.prev;
		}
	}
	if (pb_skipto_2(&p, e, 16064)) {
		p = pb_get_bool(p + 2, e, &m->binary);
	}
}
char *pb_encode_MethodOptions(char *p, struct MethodOptions const *m) {
	if (m->deprecated) {
		p = pb_put_tag_2(p, 648);
		p = pb_put_bool(p,  m->deprecated);
	}
	if (m->uninterpreted_option.len) {
		for (int i = 0; i < m->uninterpreted_option.len; i++) {
			p = pb_put_tag_2(p, 16058);
			int szlen = pb_u32_size(m->uninterpreted_option.v[i]->pb_hdr.maxsz);
			char *start = p+szlen;
			p = pb_encode_UninterpretedOption(start, m->uninterpreted_option.v[i]);
			pb_put_finish(p, start, szlen);
		}
	}
	if (m->binary) {
		p = pb_put_tag_2(p, 16064);
		p = pb_put_bool(p,  m->binary);
	}
	return p;
}
int pb_maxsz_MethodOptions(struct MethodOptions const *m) {
	int n = 0;
	if (m->deprecated) {
		n += 2 + 1;
	}
	if (m->uninterpreted_option.len) {
		for (int i = 0; i < m->uninterpreted_option.len; i++) {
			n += 2 + pb_maxsz_UninterpretedOption(m->uninterpreted_option.v[i]);
		}
	}
	if (m->binary) {
		n += 2 + 1;
	}
	n += pb_u32_size(n);
	((union pb_msg*) m)->maxsz = n;
	return n;
}

char *pb_parse_UninterpretedOption(char *p, pb_alloc_t *a, struct UninterpretedOption *m) {
	(void) a;
	(void) m;
	if (!pb_parse_map(&p)) {
		return p;
	}
	for (;;) {
		struct pb_string key;
		switch (pb_parse_field(&p, &key, 7174453) % 23) {
		case 0:
			return p;
		case 9:
			if (pb_cmp(key, "name")) {
				goto unknown;
			}
			if (pb_parse_array(&p)) {
				struct UninterpretedOption_NamePart *prev = NULL;
				do {
					struct UninterpretedOption_NamePart *c = (struct UninterpretedOption_NamePart*) pb_calloc(a, 1, sizeof(struct UninterpretedOption_NamePart));
					p = pb_parse_UninterpretedOption_NamePart(p, a, c);
					m->name.len++;
					c->pb_hdr.prev = prev;
					prev = c;
				} while (pb_more_array(&p));

				m->name.v = (const struct UninterpretedOption_NamePart**) pb_calloc(a, m->name.len, sizeof(struct UninterpretedOption_NamePart*));

				for (int i = m->name.len - 1; i >= 0; i--) {
					m->name.v[i] = prev;
					prev = (struct UninterpretedOption_NamePart*) prev->pb_hdr.prev;
				}
			}
			continue;
		case 6:
			if (pb_cmp(key, "identifier_value")) {
				goto unknown;
			}
			p = pb_parse_string(p, &m->identifier_value);
			continue;
		case 18:
			if (pb_cmp(key, "positive_int_value")) {
				goto unknown;
			}
			p = pb_parse_u64(p, &m->positive_int_value);
			continue;
		case 1:
			if (pb_cmp(key, "negative_int_value")) {
				goto unknown;
			}
			p = pb_parse_i64(p, &m->negative_int_value);
			continue;
		case 13:
			if (pb_cmp(key, "double_value")) {
				goto unknown;
			}
			p = pb_parse_double(p, &m->double_value);
			continue;
		case 21:
			if (pb_cmp(key, "string_value")) {
				goto unknown;
			}
			p = pb_parse_bytes(p, &m->string_value);
			continue;
		case 22:
			if (pb_cmp(key, "aggregate_value")) {
				goto unknown;
			}
			p = pb_parse_string(p, &m->aggregate_value);
			continue;
		default:
		unknown:
			p = pb_parse_skip(p);
		}
	}
}
int pb_fparse_UninterpretedOption(const char *fn, str_t *b, pb_alloc_t *a, struct UninterpretedOption *m) {
	int off = b->len;
	str_read_file(b, fn);
	return pb_parse_UninterpretedOption(b->buf + off, a, m) == pb_errret ? -1 : 0; 
}
void pb_print_UninterpretedOption(str_t *a, struct UninterpretedOption const *m) {
	str_grow(a, a->len + 128);
	char *p = a->buf + a->len;
	*p++ = '{';
	if (m->positive_int_value) {
		memcpy(p, "\"positive_int_value\":", 21);
		p += 21;
		p = pb_print_u64(p, m->positive_int_value);
	}
	if (m->negative_int_value) {
		memcpy(p, "\"negative_int_value\":", 21);
		p += 21;
		p = pb_print_i64(p, m->negative_int_value);
	}
	if (m->double_value) {
		memcpy(p, "\"double_value\":", 15);
		p += 15;
		p = pb_print_double(p, m->double_value);
	}
	str_setlen(a, (unsigned) (p - a->buf));
	if (m->name.len) {
		str_add(a, "\"name\":");
		str_addch(a, '[');
		for (int i = 0; i < m->name.len; i++) {
			pb_print_UninterpretedOption_NamePart(a, m->name.v[i]);
			a->buf[a->len-1] = ','; // replace trailing newline
		}
		pb_print_array_end(a);
	}
	if (m->identifier_value.len) {
		str_add(a, "\"identifier_value\":");
		pb_print_string(a, m->identifier_value); 
		a->buf[a->len-1] = ','; // replace trailing newline
	}
	if (m->string_value.len) {
		str_add(a, "\"string_value\":");
		pb_print_bytes(a, m->string_value); 
		a->buf[a->len-1] = ','; // replace trailing newline
	}
	if (m->aggregate_value.len) {
		str_add(a, "\"aggregate_value\":");
		pb_print_string(a, m->aggregate_value); 
		a->buf[a->len-1] = ','; // replace trailing newline
	}
	pb_print_map_end(a);
}
void pb_get_UninterpretedOption(const char *p, const char *e, pb_alloc_t *obj, struct UninterpretedOption *m) {
	(void) obj;
	if (pb_skipto_1(&p, e, 18)) {
		struct UninterpretedOption_NamePart *prev = NULL;
		do {
			struct pb_string msg;
			p = pb_get_string(p + 1, e, &msg);
			struct UninterpretedOption_NamePart *c = (struct UninterpretedOption_NamePart*) pb_calloc(obj, 1, sizeof(struct UninterpretedOption_NamePart));
			pb_get_UninterpretedOption_NamePart(msg.p, msg.p + msg.len, obj, c);
			m->name.len++;
			c->pb_hdr.prev = prev;
			prev = c;
		} while (!pb_cmp_tag_1(p, e, 18));

		m->name.v = (const struct UninterpretedOption_NamePart**) pb_calloc(obj, m->name.len, sizeof(struct UninterpretedOption_NamePart*));

		for (int i = m->name.len - 1; i >= 0; i--) {
			m->name.v[i] = prev;
			prev = (struct UninterpretedOption_NamePart*) prev->pb_hdr.prev;
		}
	}
	if (pb_skipto_1(&p, e, 26)) {
		p = pb_get_string(p + 1, e, (struct pb_string*)&m->identifier_value);
	}
	if (pb_skipto_1(&p, e, 32)) {
		p = pb_get_u64(p + 1, e, &m->positive_int_value);
	}
	if (pb_skipto_1(&p, e, 40)) {
		p = pb_get_u64(p + 1, e, (uint64_t*)&m->negative_int_value);
	}
	if (pb_skipto_1(&p, e, 53)) {
		p = pb_get_f64(p + 1, e, (union pb_f64*)&m->double_value);
	}
	if (pb_skipto_1(&p, e, 58)) {
		p = pb_get_bytes(p + 1, e, (struct pb_bytes*)&m->string_value);
	}
	if (pb_skipto_1(&p, e, 66)) {
		p = pb_get_string(p + 1, e, (struct pb_string*)&m->aggregate_value);
	}
}
char *pb_encode_UninterpretedOption(char *p, struct UninterpretedOption const *m) {
	if (m->name.len) {
		for (int i = 0; i < m->name.len; i++) {
			p = pb_put_tag_1(p, 18);
			int szlen = pb_u32_size(m->name.v[i]->pb_hdr.maxsz);
			char *start = p+szlen;
			p = pb_encode_UninterpretedOption_NamePart(start, m->name.v[i]);
			pb_put_finish(p, start, szlen);
		}
	}
	if (m->identifier_value.len) {
		p = pb_put_tag_1(p, 26);
		p = pb_put_string(p, *(struct pb_string*)& m->identifier_value);
	}
	if (m->positive_int_value) {
		p = pb_put_tag_1(p, 32);
		p = pb_put_u64(p,  m->positive_int_value);
	}
	if (m->negative_int_value) {
		p = pb_put_tag_1(p, 40);
		p = pb_put_u64(p, *(uint64_t*)& m->negative_int_value);
	}
	if (m->double_value) {
		p = pb_put_tag_1(p, 53);
		p = pb_put_f64(p, *(union pb_f64*)& m->double_value);
	}
	if (m->string_value.len) {
		p = pb_put_tag_1(p, 58);
		p = pb_put_bytes(p, *(struct pb_bytes*)& m->string_value);
	}
	if (m->aggregate_value.len) {
		p = pb_put_tag_1(p, 66);
		p = pb_put_string(p, *(struct pb_string*)& m->aggregate_value);
	}
	return p;
}
int pb_maxsz_UninterpretedOption(struct UninterpretedOption const *m) {
	int n = 0;
	if (m->name.len) {
		for (int i = 0; i < m->name.len; i++) {
			n += 1 + pb_maxsz_UninterpretedOption_NamePart(m->name.v[i]);
		}
	}
	if (m->identifier_value.len) {
		n += 1 + 5 + m->identifier_value.len;
	}
	if (m->positive_int_value) {
		n += 1 + 10;
	}
	if (m->negative_int_value) {
		n += 1 + 10;
	}
	if (m->double_value) {
		n += 1 + 8;
	}
	if (m->string_value.len) {
		n += 1 + 5 + m->string_value.len;
	}
	if (m->aggregate_value.len) {
		n += 1 + 5 + m->aggregate_value.len;
	}
	n += pb_u32_size(n);
	((union pb_msg*) m)->maxsz = n;
	return n;
}

char *pb_parse_UninterpretedOption_NamePart(char *p, pb_alloc_t *a, struct UninterpretedOption_NamePart *m) {
	(void) a;
	(void) m;
	if (!pb_parse_map(&p)) {
		return p;
	}
	for (;;) {
		struct pb_string key;
		switch (pb_parse_field(&p, &key, 13) % 6) {
		case 0:
			return p;
		case 5:
			if (pb_cmp(key, "name_part")) {
				goto unknown;
			}
			p = pb_parse_string(p, &m->name_part);
			continue;
		case 2:
			if (pb_cmp(key, "is_extension")) {
				goto unknown;
			}
			p = pb_parse_bool(p, &m->is_extension);
			continue;
		default:
		unknown:
			p = pb_parse_skip(p);
		}
	}
}
int pb_fparse_UninterpretedOption_NamePart(const char *fn, str_t *b, pb_alloc_t *a, struct UninterpretedOption_NamePart *m) {
	int off = b->len;
	str_read_file(b, fn);
	return pb_parse_UninterpretedOption_NamePart(b->buf + off, a, m) == pb_errret ? -1 : 0; 
}
void pb_print_UninterpretedOption_NamePart(str_t *a, struct UninterpretedOption_NamePart const *m) {
	str_grow(a, a->len + 22);
	char *p = a->buf + a->len;
	*p++ = '{';
	if (m->is_extension) {
		memcpy(p, "\"is_extension\":", 15);
		p += 15;
		p = pb_print_bool(p, m->is_extension);
	}
	str_setlen(a, (unsigned) (p - a->buf));
	if (m->name_part.len) {
		str_add(a, "\"name_part\":");
		pb_print_string(a, m->name_part); 
		a->buf[a->len-1] = ','; // replace trailing newline
	}
	pb_print_map_end(a);
}
void pb_get_UninterpretedOption_NamePart(const char *p, const char *e, pb_alloc_t *obj, struct UninterpretedOption_NamePart *m) {
	(void) obj;
	if (pb_skipto_1(&p, e, 10)) {
		p = pb_get_string(p + 1, e, (struct pb_string*)&m->name_part);
	}
	if (pb_skipto_1(&p, e, 16)) {
		p = pb_get_bool(p + 1, e, &m->is_extension);
	}
}
char *pb_encode_UninterpretedOption_NamePart(char *p, struct UninterpretedOption_NamePart const *m) {
	if (m->name_part.len) {
		p = pb_put_tag_1(p, 10);
		p = pb_put_string(p, *(struct pb_string*)& m->name_part);
	}
	if (m->is_extension) {
		p = pb_put_tag_1(p, 16);
		p = pb_put_bool(p,  m->is_extension);
	}
	return p;
}
int pb_maxsz_UninterpretedOption_NamePart(struct UninterpretedOption_NamePart const *m) {
	int n = 0;
	if (m->name_part.len) {
		n += 1 + 5 + m->name_part.len;
	}
	if (m->is_extension) {
		n += 1 + 1;
	}
	n += pb_u32_size(n);
	((union pb_msg*) m)->maxsz = n;
	return n;
}

char *pb_parse_SourceCodeInfo(char *p, pb_alloc_t *a, struct SourceCodeInfo *m) {
	(void) a;
	(void) m;
	if (!pb_parse_map(&p)) {
		return p;
	}
	for (;;) {
		struct pb_string key;
		switch (pb_parse_field(&p, &key, 1) % 3) {
		case 0:
			return p;
		case 2:
			if (pb_cmp(key, "location")) {
				goto unknown;
			}
			if (pb_parse_array(&p)) {
				struct SourceCodeInfo_Location *prev = NULL;
				do {
					struct SourceCodeInfo_Location *c = (struct SourceCodeInfo_Location*) pb_calloc(a, 1, sizeof(struct SourceCodeInfo_Location));
					p = pb_parse_SourceCodeInfo_Location(p, a, c);
					m->location.len++;
					c->pb_hdr.prev = prev;
					prev = c;
				} while (pb_more_array(&p));

				m->location.v = (const struct SourceCodeInfo_Location**) pb_calloc(a, m->location.len, sizeof(struct SourceCodeInfo_Location*));

				for (int i = m->location.len - 1; i >= 0; i--) {
					m->location.v[i] = prev;
					prev = (struct SourceCodeInfo_Location*) prev->pb_hdr.prev;
				}
			}
			continue;
		default:
		unknown:
			p = pb_parse_skip(p);
		}
	}
}
int pb_fparse_SourceCodeInfo(const char *fn, str_t *b, pb_alloc_t *a, struct SourceCodeInfo *m) {
	int off = b->len;
	str_read_file(b, fn);
	return pb_parse_SourceCodeInfo(b->buf + off, a, m) == pb_errret ? -1 : 0; 
}
void pb_print_SourceCodeInfo(str_t *a, struct SourceCodeInfo const *m) {
	str_grow(a, a->len + 1);
	char *p = a->buf + a->len;
	*p++ = '{';
	str_setlen(a, (unsigned) (p - a->buf));
	if (m->location.len) {
		str_add(a, "\"location\":");
		str_addch(a, '[');
		for (int i = 0; i < m->location.len; i++) {
			pb_print_SourceCodeInfo_Location(a, m->location.v[i]);
			a->buf[a->len-1] = ','; // replace trailing newline
		}
		pb_print_array_end(a);
	}
	pb_print_map_end(a);
}
void pb_get_SourceCodeInfo(const char *p, const char *e, pb_alloc_t *obj, struct SourceCodeInfo *m) {
	(void) obj;
	if (pb_skipto_1(&p, e, 10)) {
		struct SourceCodeInfo_Location *prev = NULL;
		do {
			struct pb_string msg;
			p = pb_get_string(p + 1, e, &msg);
			struct SourceCodeInfo_Location *c = (struct SourceCodeInfo_Location*) pb_calloc(obj, 1, sizeof(struct SourceCodeInfo_Location));
			pb_get_SourceCodeInfo_Location(msg.p, msg.p + msg.len, obj, c);
			m->location.len++;
			c->pb_hdr.prev = prev;
			prev = c;
		} while (!pb_cmp_tag_1(p, e, 10));

		m->location.v = (const struct SourceCodeInfo_Location**) pb_calloc(obj, m->location.len, sizeof(struct SourceCodeInfo_Location*));

		for (int i = m->location.len - 1; i >= 0; i--) {
			m->location.v[i] = prev;
			prev = (struct SourceCodeInfo_Location*) prev->pb_hdr.prev;
		}
	}
}
char *pb_encode_SourceCodeInfo(char *p, struct SourceCodeInfo const *m) {
	if (m->location.len) {
		for (int i = 0; i < m->location.len; i++) {
			p = pb_put_tag_1(p, 10);
			int szlen = pb_u32_size(m->location.v[i]->pb_hdr.maxsz);
			char *start = p+szlen;
			p = pb_encode_SourceCodeInfo_Location(start, m->location.v[i]);
			pb_put_finish(p, start, szlen);
		}
	}
	return p;
}
int pb_maxsz_SourceCodeInfo(struct SourceCodeInfo const *m) {
	int n = 0;
	if (m->location.len) {
		for (int i = 0; i < m->location.len; i++) {
			n += 1 + pb_maxsz_SourceCodeInfo_Location(m->location.v[i]);
		}
	}
	n += pb_u32_size(n);
	((union pb_msg*) m)->maxsz = n;
	return n;
}

char *pb_parse_SourceCodeInfo_Location(char *p, pb_alloc_t *a, struct SourceCodeInfo_Location *m) {
	(void) a;
	(void) m;
	if (!pb_parse_map(&p)) {
		return p;
	}
	for (;;) {
		struct pb_string key;
		switch (pb_parse_field(&p, &key, 88573) % 17) {
		case 0:
			return p;
		case 3:
			if (pb_cmp(key, "path")) {
				goto unknown;
			}
			p = pb_parse_array_i32(p, a, &m->path.v, &m->path.len);
			continue;
		case 10:
			if (pb_cmp(key, "span")) {
				goto unknown;
			}
			p = pb_parse_array_i32(p, a, &m->span.v, &m->span.len);
			continue;
		case 2:
			if (pb_cmp(key, "leading_comments")) {
				goto unknown;
			}
			p = pb_parse_string(p, &m->leading_comments);
			continue;
		case 12:
			if (pb_cmp(key, "trailing_comments")) {
				goto unknown;
			}
			p = pb_parse_string(p, &m->trailing_comments);
			continue;
		case 14:
			if (pb_cmp(key, "leading_detached_comments")) {
				goto unknown;
			}
			p = pb_parse_array_string(p, a, &m->leading_detached_comments.v, &m->leading_detached_comments.len);
			continue;
		default:
		unknown:
			p = pb_parse_skip(p);
		}
	}
}
int pb_fparse_SourceCodeInfo_Location(const char *fn, str_t *b, pb_alloc_t *a, struct SourceCodeInfo_Location *m) {
	int off = b->len;
	str_read_file(b, fn);
	return pb_parse_SourceCodeInfo_Location(b->buf + off, a, m) == pb_errret ? -1 : 0; 
}
void pb_print_SourceCodeInfo_Location(str_t *a, struct SourceCodeInfo_Location const *m) {
	str_grow(a, a->len + 1);
	char *p = a->buf + a->len;
	*p++ = '{';
	str_setlen(a, (unsigned) (p - a->buf));
	if (m->path.len) {
		str_add(a, "\"path\":");
		str_grow(a, a->len + 3 /*[],*/ + m->path.len * 12);
		char *mp = a->buf + a->len;
		*mp++ = '[';
		for (int i = 0; i < m->path.len; i++) {
			mp = pb_print_i32(mp, m->path.v[i]);
		}
		mp = pb_print_array_end_i(mp);
		a->len = (int) (mp - a->buf);
	}
	if (m->span.len) {
		str_add(a, "\"span\":");
		str_grow(a, a->len + 3 /*[],*/ + m->span.len * 12);
		char *mp = a->buf + a->len;
		*mp++ = '[';
		for (int i = 0; i < m->span.len; i++) {
			mp = pb_print_i32(mp, m->span.v[i]);
		}
		mp = pb_print_array_end_i(mp);
		a->len = (int) (mp - a->buf);
	}
	if (m->leading_comments.len) {
		str_add(a, "\"leading_comments\":");
		pb_print_string(a, m->leading_comments); 
		a->buf[a->len-1] = ','; // replace trailing newline
	}
	if (m->trailing_comments.len) {
		str_add(a, "\"trailing_comments\":");
		pb_print_string(a, m->trailing_comments); 
		a->buf[a->len-1] = ','; // replace trailing newline
	}
	if (m->leading_detached_comments.len) {
		str_add(a, "\"leading_detached_comments\":");
		str_addch(a, '[');
		for (int i = 0; i < m->leading_detached_comments.len; i++) {
			pb_print_string(a, m->leading_detached_comments.v[i]);
			a->buf[a->len-1] = ','; // replace trailing newline
		}
		pb_print_array_end(a);
	}
	pb_print_map_end(a);
}
void pb_get_SourceCodeInfo_Location(const char *p, const char *e, pb_alloc_t *obj, struct SourceCodeInfo_Location *m) {
	(void) obj;
	if (pb_skipto_1(&p, e, 10)) {
		p = pb_get_packed_u32(p, e, obj, (uint32_t**)&m->path.v, &m->path.len);
	}
	if (pb_skipto_1(&p, e, 18)) {
		p = pb_get_packed_u32(p, e, obj, (uint32_t**)&m->span.v, &m->span.len);
	}
	if (pb_skipto_1(&p, e, 26)) {
		p = pb_get_string(p + 1, e, (struct pb_string*)&m->leading_comments);
	}
	if (pb_skipto_1(&p, e, 34)) {
		p = pb_get_string(p + 1, e, (struct pb_string*)&m->trailing_comments);
	}
	if (pb_skipto_1(&p, e, 50)) {
		do {
			m->leading_detached_comments.v = (struct pb_string*) pb_reserve(obj, m->leading_detached_comments.len, sizeof(struct pb_string));
			p = pb_get_string(p + 1, e, (struct pb_string*)&m->leading_detached_comments.v[m->leading_detached_comments.len]);
			m->leading_detached_comments.len++;
		} while (!pb_cmp_tag_1(p, e, 50));

		pb_commit(obj, m->leading_detached_comments.len, sizeof(struct pb_string));
	}
}
char *pb_encode_SourceCodeInfo_Location(char *p, struct SourceCodeInfo_Location const *m) {
	if (m->path.len) {
		p = pb_put_tag_1(p, 10);
		p = pb_put_packed_u32(p, *(uint32_t**)& m->path.v, m->path.len);
	}
	if (m->span.len) {
		p = pb_put_tag_1(p, 18);
		p = pb_put_packed_u32(p, *(uint32_t**)& m->span.v, m->span.len);
	}
	if (m->leading_comments.len) {
		p = pb_put_tag_1(p, 26);
		p = pb_put_string(p, *(struct pb_string*)& m->leading_comments);
	}
	if (m->trailing_comments.len) {
		p = pb_put_tag_1(p, 34);
		p = pb_put_string(p, *(struct pb_string*)& m->trailing_comments);
	}
	if (m->leading_detached_comments.len) {
		for (int i = 0; i < m->leading_detached_comments.len; i++) {
			p = pb_put_tag_1(p, 50);
			p = pb_put_string(p, *(struct pb_string*)& m->leading_detached_comments.v[i]);
		}
	}
	return p;
}
int pb_maxsz_SourceCodeInfo_Location(struct SourceCodeInfo_Location const *m) {
	int n = 0;
	if (m->path.len) {
		n += 1 + (5 * m->path.len);
	}
	if (m->span.len) {
		n += 1 + (5 * m->span.len);
	}
	if (m->leading_comments.len) {
		n += 1 + 5 + m->leading_comments.len;
	}
	if (m->trailing_comments.len) {
		n += 1 + 5 + m->trailing_comments.len;
	}
	if (m->leading_detached_comments.len) {
		for (int i = 0; i < m->leading_detached_comments.len; i++) {
			n += 1 + 5 + m->leading_detached_comments.v[i].len;
		}
	}
	n += pb_u32_size(n);
	((union pb_msg*) m)->maxsz = n;
	return n;
}

char *pb_parse_GeneratedCodeInfo(char *p, pb_alloc_t *a, struct GeneratedCodeInfo *m) {
	(void) a;
	(void) m;
	if (!pb_parse_map(&p)) {
		return p;
	}
	for (;;) {
		struct pb_string key;
		switch (pb_parse_field(&p, &key, 4) % 4) {
		case 0:
			return p;
		case 2:
			if (pb_cmp(key, "annotation")) {
				goto unknown;
			}
			if (pb_parse_array(&p)) {
				struct GeneratedCodeInfo_Annotation *prev = NULL;
				do {
					struct GeneratedCodeInfo_Annotation *c = (struct GeneratedCodeInfo_Annotation*) pb_calloc(a, 1, sizeof(struct GeneratedCodeInfo_Annotation));
					p = pb_parse_GeneratedCodeInfo_Annotation(p, a, c);
					m->annotation.len++;
					c->pb_hdr.prev = prev;
					prev = c;
				} while (pb_more_array(&p));

				m->annotation.v = (const struct GeneratedCodeInfo_Annotation**) pb_calloc(a, m->annotation.len, sizeof(struct GeneratedCodeInfo_Annotation*));

				for (int i = m->annotation.len - 1; i >= 0; i--) {
					m->annotation.v[i] = prev;
					prev = (struct GeneratedCodeInfo_Annotation*) prev->pb_hdr.prev;
				}
			}
			continue;
		default:
		unknown:
			p = pb_parse_skip(p);
		}
	}
}
int pb_fparse_GeneratedCodeInfo(const char *fn, str_t *b, pb_alloc_t *a, struct GeneratedCodeInfo *m) {
	int off = b->len;
	str_read_file(b, fn);
	return pb_parse_GeneratedCodeInfo(b->buf + off, a, m) == pb_errret ? -1 : 0; 
}
void pb_print_GeneratedCodeInfo(str_t *a, struct GeneratedCodeInfo const *m) {
	str_grow(a, a->len + 1);
	char *p = a->buf + a->len;
	*p++ = '{';
	str_setlen(a, (unsigned) (p - a->buf));
	if (m->annotation.len) {
		str_add(a, "\"annotation\":");
		str_addch(a, '[');
		for (int i = 0; i < m->annotation.len; i++) {
			pb_print_GeneratedCodeInfo_Annotation(a, m->annotation.v[i]);
			a->buf[a->len-1] = ','; // replace trailing newline
		}
		pb_print_array_end(a);
	}
	pb_print_map_end(a);
}
void pb_get_GeneratedCodeInfo(const char *p, const char *e, pb_alloc_t *obj, struct GeneratedCodeInfo *m) {
	(void) obj;
	if (pb_skipto_1(&p, e, 10)) {
		struct GeneratedCodeInfo_Annotation *prev = NULL;
		do {
			struct pb_string msg;
			p = pb_get_string(p + 1, e, &msg);
			struct GeneratedCodeInfo_Annotation *c = (struct GeneratedCodeInfo_Annotation*) pb_calloc(obj, 1, sizeof(struct GeneratedCodeInfo_Annotation));
			pb_get_GeneratedCodeInfo_Annotation(msg.p, msg.p + msg.len, obj, c);
			m->annotation.len++;
			c->pb_hdr.prev = prev;
			prev = c;
		} while (!pb_cmp_tag_1(p, e, 10));

		m->annotation.v = (const struct GeneratedCodeInfo_Annotation**) pb_calloc(obj, m->annotation.len, sizeof(struct GeneratedCodeInfo_Annotation*));

		for (int i = m->annotation.len - 1; i >= 0; i--) {
			m->annotation.v[i] = prev;
			prev = (struct GeneratedCodeInfo_Annotation*) prev->pb_hdr.prev;
		}
	}
}
char *pb_encode_GeneratedCodeInfo(char *p, struct GeneratedCodeInfo const *m) {
	if (m->annotation.len) {
		for (int i = 0; i < m->annotation.len; i++) {
			p = pb_put_tag_1(p, 10);
			int szlen = pb_u32_size(m->annotation.v[i]->pb_hdr.maxsz);
			char *start = p+szlen;
			p = pb_encode_GeneratedCodeInfo_Annotation(start, m->annotation.v[i]);
			pb_put_finish(p, start, szlen);
		}
	}
	return p;
}
int pb_maxsz_GeneratedCodeInfo(struct GeneratedCodeInfo const *m) {
	int n = 0;
	if (m->annotation.len) {
		for (int i = 0; i < m->annotation.len; i++) {
			n += 1 + pb_maxsz_GeneratedCodeInfo_Annotation(m->annotation.v[i]);
		}
	}
	n += pb_u32_size(n);
	((union pb_msg*) m)->maxsz = n;
	return n;
}

char *pb_parse_GeneratedCodeInfo_Annotation(char *p, pb_alloc_t *a, struct GeneratedCodeInfo_Annotation *m) {
	(void) a;
	(void) m;
	if (!pb_parse_map(&p)) {
		return p;
	}
	for (;;) {
		struct pb_string key;
		switch (pb_parse_field(&p, &key, 1) % 6) {
		case 0:
			return p;
		case 3:
			if (pb_cmp(key, "path")) {
				goto unknown;
			}
			p = pb_parse_array_i32(p, a, &m->path.v, &m->path.len);
			continue;
		case 4:
			if (pb_cmp(key, "source_file")) {
				goto unknown;
			}
			p = pb_parse_string(p, &m->source_file);
			continue;
		case 1:
			if (pb_cmp(key, "begin")) {
				goto unknown;
			}
			p = pb_parse_i32(p, &m->begin);
			continue;
		case 5:
			if (pb_cmp(key, "end")) {
				goto unknown;
			}
			p = pb_parse_i32(p, &m->end);
			continue;
		default:
		unknown:
			p = pb_parse_skip(p);
		}
	}
}
int pb_fparse_GeneratedCodeInfo_Annotation(const char *fn, str_t *b, pb_alloc_t *a, struct GeneratedCodeInfo_Annotation *m) {
	int off = b->len;
	str_read_file(b, fn);
	return pb_parse_GeneratedCodeInfo_Annotation(b->buf + off, a, m) == pb_errret ? -1 : 0; 
}
void pb_print_GeneratedCodeInfo_Annotation(str_t *a, struct GeneratedCodeInfo_Annotation const *m) {
	str_grow(a, a->len + 39);
	char *p = a->buf + a->len;
	*p++ = '{';
	if (m->begin) {
		memcpy(p, "\"begin\":", 8);
		p += 8;
		p = pb_print_i32(p, m->begin);
	}
	if (m->end) {
		memcpy(p, "\"end\":", 6);
		p += 6;
		p = pb_print_i32(p, m->end);
	}
	str_setlen(a, (unsigned) (p - a->buf));
	if (m->path.len) {
		str_add(a, "\"path\":");
		str_grow(a, a->len + 3 /*[],*/ + m->path.len * 12);
		char *mp = a->buf + a->len;
		*mp++ = '[';
		for (int i = 0; i < m->path.len; i++) {
			mp = pb_print_i32(mp, m->path.v[i]);
		}
		mp = pb_print_array_end_i(mp);
		a->len = (int) (mp - a->buf);
	}
	if (m->source_file.len) {
		str_add(a, "\"source_file\":");
		pb_print_string(a, m->source_file); 
		a->buf[a->len-1] = ','; // replace trailing newline
	}
	pb_print_map_end(a);
}
void pb_get_GeneratedCodeInfo_Annotation(const char *p, const char *e, pb_alloc_t *obj, struct GeneratedCodeInfo_Annotation *m) {
	(void) obj;
	if (pb_skipto_1(&p, e, 10)) {
		p = pb_get_packed_u32(p, e, obj, (uint32_t**)&m->path.v, &m->path.len);
	}
	if (pb_skipto_1(&p, e, 18)) {
		p = pb_get_string(p + 1, e, (struct pb_string*)&m->source_file);
	}
	if (pb_skipto_1(&p, e, 24)) {
		p = pb_get_u32(p + 1, e, (uint32_t*)&m->begin);
	}
	if (pb_skipto_1(&p, e, 32)) {
		p = pb_get_u32(p + 1, e, (uint32_t*)&m->end);
	}
}
char *pb_encode_GeneratedCodeInfo_Annotation(char *p, struct GeneratedCodeInfo_Annotation const *m) {
	if (m->path.len) {
		p = pb_put_tag_1(p, 10);
		p = pb_put_packed_u32(p, *(uint32_t**)& m->path.v, m->path.len);
	}
	if (m->source_file.len) {
		p = pb_put_tag_1(p, 18);
		p = pb_put_string(p, *(struct pb_string*)& m->source_file);
	}
	if (m->begin) {
		p = pb_put_tag_1(p, 24);
		p = pb_put_u32(p, *(uint32_t*)& m->begin);
	}
	if (m->end) {
		p = pb_put_tag_1(p, 32);
		p = pb_put_u32(p, *(uint32_t*)& m->end);
	}
	return p;
}
int pb_maxsz_GeneratedCodeInfo_Annotation(struct GeneratedCodeInfo_Annotation const *m) {
	int n = 0;
	if (m->path.len) {
		n += 1 + (5 * m->path.len);
	}
	if (m->source_file.len) {
		n += 1 + 5 + m->source_file.len;
	}
	if (m->begin) {
		n += 1 + 5;
	}
	if (m->end) {
		n += 1 + 5;
	}
	n += pb_u32_size(n);
	((union pb_msg*) m)->maxsz = n;
	return n;
}
