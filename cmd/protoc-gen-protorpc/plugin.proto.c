#include "protorpc/cmd/protoc-gen-protorpc/plugin.proto.h"
#include <os/log.h>
#include <os/pipe.h>

char *pb_parse_CodeGeneratorRequest(char *p, pb_alloc_t *a, struct CodeGeneratorRequest *m) {
	(void) a;
	(void) m;
	if (!pb_parse_map(&p)) {
		return p;
	}
	for (;;) {
		struct pb_string key;
		switch (pb_parse_field(&p, &key, 13) % 7) {
		case 0:
			return p;
		case 5:
			if (pb_cmp(key, "file_to_generate")) {
				goto unknown;
			}
			p = pb_parse_array_string(p, a, &m->file_to_generate.v, &m->file_to_generate.len);
			continue;
		case 4:
			if (pb_cmp(key, "parameter")) {
				goto unknown;
			}
			p = pb_parse_string(p, &m->parameter);
			continue;
		case 2:
			if (pb_cmp(key, "proto_file")) {
				goto unknown;
			}
			if (pb_parse_array(&p)) {
				struct FileDescriptorProto *prev = NULL;
				do {
					struct FileDescriptorProto *c = (struct FileDescriptorProto*) pb_calloc(a, 1, sizeof(struct FileDescriptorProto));
					p = pb_parse_FileDescriptorProto(p, a, c);
					m->proto_file.len++;
					c->pb_hdr.prev = prev;
					prev = c;
				} while (pb_more_array(&p));

				m->proto_file.v = (const struct FileDescriptorProto**) pb_calloc(a, m->proto_file.len, sizeof(struct FileDescriptorProto*));

				for (int i = m->proto_file.len - 1; i >= 0; i--) {
					m->proto_file.v[i] = prev;
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
int pb_fparse_CodeGeneratorRequest(const char *fn, str_t *b, pb_alloc_t *a, struct CodeGeneratorRequest *m) {
	int off = b->len;
	str_read_file(b, fn);
	return pb_parse_CodeGeneratorRequest(b->buf + off, a, m) == pb_errret ? -1 : 0; 
}
void pb_print_CodeGeneratorRequest(str_t *a, struct CodeGeneratorRequest const *m) {
	str_grow(a, a->len + 1);
	char *p = a->buf + a->len;
	*p++ = '{';
	str_setlen(a, (unsigned) (p - a->buf));
	if (m->file_to_generate.len) {
		str_add(a, "\"file_to_generate\":");
		str_addch(a, '[');
		for (int i = 0; i < m->file_to_generate.len; i++) {
			pb_print_string(a, m->file_to_generate.v[i]);
			a->buf[a->len-1] = ','; // replace trailing newline
		}
		pb_print_array_end(a);
	}
	if (m->parameter.len) {
		str_add(a, "\"parameter\":");
		pb_print_string(a, m->parameter); 
		a->buf[a->len-1] = ','; // replace trailing newline
	}
	if (m->proto_file.len) {
		str_add(a, "\"proto_file\":");
		str_addch(a, '[');
		for (int i = 0; i < m->proto_file.len; i++) {
			pb_print_FileDescriptorProto(a, m->proto_file.v[i]);
			a->buf[a->len-1] = ','; // replace trailing newline
		}
		pb_print_array_end(a);
	}
	pb_print_map_end(a);
}
void pb_get_CodeGeneratorRequest(const char *p, const char *e, pb_alloc_t *obj, struct CodeGeneratorRequest *m) {
	(void) obj;
	if (pb_skipto_1(&p, e, 10)) {
		do {
			m->file_to_generate.v = (struct pb_string*) pb_reserve(obj, m->file_to_generate.len, sizeof(struct pb_string));
			p = pb_get_string(p + 1, e, (struct pb_string*)&m->file_to_generate.v[m->file_to_generate.len]);
			m->file_to_generate.len++;
		} while (!pb_cmp_tag_1(p, e, 10));

		pb_commit(obj, m->file_to_generate.len, sizeof(struct pb_string));
	}
	if (pb_skipto_1(&p, e, 18)) {
		p = pb_get_string(p + 1, e, (struct pb_string*)&m->parameter);
	}
	if (pb_skipto_1(&p, e, 122)) {
		struct FileDescriptorProto *prev = NULL;
		do {
			struct pb_string msg;
			p = pb_get_string(p + 1, e, &msg);
			struct FileDescriptorProto *c = (struct FileDescriptorProto*) pb_calloc(obj, 1, sizeof(struct FileDescriptorProto));
			pb_get_FileDescriptorProto(msg.p, msg.p + msg.len, obj, c);
			m->proto_file.len++;
			c->pb_hdr.prev = prev;
			prev = c;
		} while (!pb_cmp_tag_1(p, e, 122));

		m->proto_file.v = (const struct FileDescriptorProto**) pb_calloc(obj, m->proto_file.len, sizeof(struct FileDescriptorProto*));

		for (int i = m->proto_file.len - 1; i >= 0; i--) {
			m->proto_file.v[i] = prev;
			prev = (struct FileDescriptorProto*) prev->pb_hdr.prev;
		}
	}
}
char *pb_encode_CodeGeneratorRequest(char *p, struct CodeGeneratorRequest const *m) {
	if (m->file_to_generate.len) {
		for (int i = 0; i < m->file_to_generate.len; i++) {
			p = pb_put_tag_1(p, 10);
			p = pb_put_string(p, *(struct pb_string*)& m->file_to_generate.v[i]);
		}
	}
	if (m->parameter.len) {
		p = pb_put_tag_1(p, 18);
		p = pb_put_string(p, *(struct pb_string*)& m->parameter);
	}
	if (m->proto_file.len) {
		for (int i = 0; i < m->proto_file.len; i++) {
			p = pb_put_tag_1(p, 122);
			int szlen = pb_u32_size(m->proto_file.v[i]->pb_hdr.maxsz);
			char *start = p+szlen;
			p = pb_encode_FileDescriptorProto(start, m->proto_file.v[i]);
			pb_put_finish(p, start, szlen);
		}
	}
	return p;
}
int pb_maxsz_CodeGeneratorRequest(struct CodeGeneratorRequest const *m) {
	int n = 0;
	if (m->file_to_generate.len) {
		for (int i = 0; i < m->file_to_generate.len; i++) {
			n += 1 + 5 + m->file_to_generate.v[i].len;
		}
	}
	if (m->parameter.len) {
		n += 1 + 5 + m->parameter.len;
	}
	if (m->proto_file.len) {
		for (int i = 0; i < m->proto_file.len; i++) {
			n += 1 + pb_maxsz_FileDescriptorProto(m->proto_file.v[i]);
		}
	}
	n += pb_u32_size(n);
	((union pb_msg*) m)->maxsz = n;
	return n;
}

char *pb_parse_CodeGeneratorResponse(char *p, pb_alloc_t *a, struct CodeGeneratorResponse *m) {
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
			if (pb_cmp(key, "error")) {
				goto unknown;
			}
			p = pb_parse_string(p, &m->error);
			continue;
		case 1:
			if (pb_cmp(key, "file")) {
				goto unknown;
			}
			if (pb_parse_array(&p)) {
				struct CodeGeneratorResponse_File *prev = NULL;
				do {
					struct CodeGeneratorResponse_File *c = (struct CodeGeneratorResponse_File*) pb_calloc(a, 1, sizeof(struct CodeGeneratorResponse_File));
					p = pb_parse_CodeGeneratorResponse_File(p, a, c);
					m->file.len++;
					c->pb_hdr.prev = prev;
					prev = c;
				} while (pb_more_array(&p));

				m->file.v = (const struct CodeGeneratorResponse_File**) pb_calloc(a, m->file.len, sizeof(struct CodeGeneratorResponse_File*));

				for (int i = m->file.len - 1; i >= 0; i--) {
					m->file.v[i] = prev;
					prev = (struct CodeGeneratorResponse_File*) prev->pb_hdr.prev;
				}
			}
			continue;
		default:
		unknown:
			p = pb_parse_skip(p);
		}
	}
}
int pb_fparse_CodeGeneratorResponse(const char *fn, str_t *b, pb_alloc_t *a, struct CodeGeneratorResponse *m) {
	int off = b->len;
	str_read_file(b, fn);
	return pb_parse_CodeGeneratorResponse(b->buf + off, a, m) == pb_errret ? -1 : 0; 
}
void pb_print_CodeGeneratorResponse(str_t *a, struct CodeGeneratorResponse const *m) {
	str_grow(a, a->len + 1);
	char *p = a->buf + a->len;
	*p++ = '{';
	str_setlen(a, (unsigned) (p - a->buf));
	if (m->error.len) {
		str_add(a, "\"error\":");
		pb_print_string(a, m->error); 
		a->buf[a->len-1] = ','; // replace trailing newline
	}
	if (m->file.len) {
		str_add(a, "\"file\":");
		str_addch(a, '[');
		for (int i = 0; i < m->file.len; i++) {
			pb_print_CodeGeneratorResponse_File(a, m->file.v[i]);
			a->buf[a->len-1] = ','; // replace trailing newline
		}
		pb_print_array_end(a);
	}
	pb_print_map_end(a);
}
void pb_get_CodeGeneratorResponse(const char *p, const char *e, pb_alloc_t *obj, struct CodeGeneratorResponse *m) {
	(void) obj;
	if (pb_skipto_1(&p, e, 10)) {
		p = pb_get_string(p + 1, e, (struct pb_string*)&m->error);
	}
	if (pb_skipto_1(&p, e, 122)) {
		struct CodeGeneratorResponse_File *prev = NULL;
		do {
			struct pb_string msg;
			p = pb_get_string(p + 1, e, &msg);
			struct CodeGeneratorResponse_File *c = (struct CodeGeneratorResponse_File*) pb_calloc(obj, 1, sizeof(struct CodeGeneratorResponse_File));
			pb_get_CodeGeneratorResponse_File(msg.p, msg.p + msg.len, obj, c);
			m->file.len++;
			c->pb_hdr.prev = prev;
			prev = c;
		} while (!pb_cmp_tag_1(p, e, 122));

		m->file.v = (const struct CodeGeneratorResponse_File**) pb_calloc(obj, m->file.len, sizeof(struct CodeGeneratorResponse_File*));

		for (int i = m->file.len - 1; i >= 0; i--) {
			m->file.v[i] = prev;
			prev = (struct CodeGeneratorResponse_File*) prev->pb_hdr.prev;
		}
	}
}
char *pb_encode_CodeGeneratorResponse(char *p, struct CodeGeneratorResponse const *m) {
	if (m->error.len) {
		p = pb_put_tag_1(p, 10);
		p = pb_put_string(p, *(struct pb_string*)& m->error);
	}
	if (m->file.len) {
		for (int i = 0; i < m->file.len; i++) {
			p = pb_put_tag_1(p, 122);
			int szlen = pb_u32_size(m->file.v[i]->pb_hdr.maxsz);
			char *start = p+szlen;
			p = pb_encode_CodeGeneratorResponse_File(start, m->file.v[i]);
			pb_put_finish(p, start, szlen);
		}
	}
	return p;
}
int pb_maxsz_CodeGeneratorResponse(struct CodeGeneratorResponse const *m) {
	int n = 0;
	if (m->error.len) {
		n += 1 + 5 + m->error.len;
	}
	if (m->file.len) {
		for (int i = 0; i < m->file.len; i++) {
			n += 1 + pb_maxsz_CodeGeneratorResponse_File(m->file.v[i]);
		}
	}
	n += pb_u32_size(n);
	((union pb_msg*) m)->maxsz = n;
	return n;
}

char *pb_parse_CodeGeneratorResponse_File(char *p, pb_alloc_t *a, struct CodeGeneratorResponse_File *m) {
	(void) a;
	(void) m;
	if (!pb_parse_map(&p)) {
		return p;
	}
	for (;;) {
		struct pb_string key;
		switch (pb_parse_field(&p, &key, 1) % 5) {
		case 0:
			return p;
		case 2:
			if (pb_cmp(key, "name")) {
				goto unknown;
			}
			p = pb_parse_string(p, &m->name);
			continue;
		case 1:
			if (pb_cmp(key, "insertion_point")) {
				goto unknown;
			}
			p = pb_parse_string(p, &m->insertion_point);
			continue;
		case 3:
			if (pb_cmp(key, "content")) {
				goto unknown;
			}
			p = pb_parse_string(p, &m->content);
			continue;
		default:
		unknown:
			p = pb_parse_skip(p);
		}
	}
}
int pb_fparse_CodeGeneratorResponse_File(const char *fn, str_t *b, pb_alloc_t *a, struct CodeGeneratorResponse_File *m) {
	int off = b->len;
	str_read_file(b, fn);
	return pb_parse_CodeGeneratorResponse_File(b->buf + off, a, m) == pb_errret ? -1 : 0; 
}
void pb_print_CodeGeneratorResponse_File(str_t *a, struct CodeGeneratorResponse_File const *m) {
	str_grow(a, a->len + 1);
	char *p = a->buf + a->len;
	*p++ = '{';
	str_setlen(a, (unsigned) (p - a->buf));
	if (m->name.len) {
		str_add(a, "\"name\":");
		pb_print_string(a, m->name); 
		a->buf[a->len-1] = ','; // replace trailing newline
	}
	if (m->insertion_point.len) {
		str_add(a, "\"insertion_point\":");
		pb_print_string(a, m->insertion_point); 
		a->buf[a->len-1] = ','; // replace trailing newline
	}
	if (m->content.len) {
		str_add(a, "\"content\":");
		pb_print_string(a, m->content); 
		a->buf[a->len-1] = ','; // replace trailing newline
	}
	pb_print_map_end(a);
}
void pb_get_CodeGeneratorResponse_File(const char *p, const char *e, pb_alloc_t *obj, struct CodeGeneratorResponse_File *m) {
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
}
char *pb_encode_CodeGeneratorResponse_File(char *p, struct CodeGeneratorResponse_File const *m) {
	if (m->name.len) {
		p = pb_put_tag_1(p, 10);
		p = pb_put_string(p, *(struct pb_string*)& m->name);
	}
	if (m->insertion_point.len) {
		p = pb_put_tag_1(p, 18);
		p = pb_put_string(p, *(struct pb_string*)& m->insertion_point);
	}
	if (m->content.len) {
		p = pb_put_tag_1(p, 122);
		p = pb_put_string(p, *(struct pb_string*)& m->content);
	}
	return p;
}
int pb_maxsz_CodeGeneratorResponse_File(struct CodeGeneratorResponse_File const *m) {
	int n = 0;
	if (m->name.len) {
		n += 1 + 5 + m->name.len;
	}
	if (m->insertion_point.len) {
		n += 1 + 5 + m->insertion_point.len;
	}
	if (m->content.len) {
		n += 1 + 5 + m->content.len;
	}
	n += pb_u32_size(n);
	((union pb_msg*) m)->maxsz = n;
	return n;
}
