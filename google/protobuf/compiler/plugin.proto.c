#include "google/protobuf/compiler/plugin.proto.h"

char *pb_parse_Version(char *p, pb_buf_t *obj, struct Version *m) {
	(void) obj;
	(void) m;
	if (!pb_parse_map(&p)) {
		return p;
	}
	for (;;) {
		struct pb_string key;
		switch (pb_parse_field(&p, &key, 121) % 10) {
		case 0:
			return p;
		case 7:
			if (pb_cmp(key, "major")) {
				goto unknown;
			}
			p = pb_parse_i32(p, &m->major);
			continue;
		case 9:
			if (pb_cmp(key, "minor")) {
				goto unknown;
			}
			p = pb_parse_i32(p, &m->minor);
			continue;
		case 8:
			if (pb_cmp(key, "patch")) {
				goto unknown;
			}
			p = pb_parse_i32(p, &m->patch);
			continue;
		case 1:
			if (pb_cmp(key, "suffix")) {
				goto unknown;
			}
			p = pb_parse_string(p, &m->suffix);
			continue;
		default:
		unknown:
			p = pb_parse_skip(p);
		}
	}
}
int pb_print_Version(pb_buf_t *a, struct Version const *m) {
	char *p = pb_appendsz(a, 61);
	if (!p) {return -1;}
	*p++ = '{';
	if (m->major) {
		memcpy(p, "\"major\":", 8);
		p += 8;
		p = pb_print_i32(p, m->major);
	}
	if (m->minor) {
		memcpy(p, "\"minor\":", 8);
		p += 8;
		p = pb_print_i32(p, m->minor);
	}
	if (m->patch) {
		memcpy(p, "\"patch\":", 8);
		p += 8;
		p = pb_print_i32(p, m->patch);
	}
	a->next = p;
	if (m->suffix.len) {
		if (pb_append(a, "\"suffix\":", 9)) {return -1;}
		if (pb_print_string(a, m->suffix)) {return -1;}
		a->next[-1] = ','; // replace trailing newline
	}
	return pb_print_map_end(a);
}
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
		p = pb_get_string(p + 1, e, (struct pb_string*)&m->suffix);
	}
	return 0;
}
char *pb_encode_Version(char *p, struct Version const *m) {
	if (m->major) {
		p = pb_put_tag_1(p, 8);
		p = pb_put_u32(p, *(uint32_t*)& m->major);
	}
	if (m->minor) {
		p = pb_put_tag_1(p, 16);
		p = pb_put_u32(p, *(uint32_t*)& m->minor);
	}
	if (m->patch) {
		p = pb_put_tag_1(p, 24);
		p = pb_put_u32(p, *(uint32_t*)& m->patch);
	}
	if (m->suffix.len) {
		p = pb_put_tag_1(p, 34);
		p = pb_put_string(p, *(struct pb_string*)& m->suffix);
	}
	return p;
}
int pb_maxsz_Version(struct Version const *m) {
	int n = 0;
	if (m->major) {
		n += 1 + 5;
	}
	if (m->minor) {
		n += 1 + 5;
	}
	if (m->patch) {
		n += 1 + 5;
	}
	if (m->suffix.len) {
		n += 1 + 5 + m->suffix.len;
	}
	n += pb_u32_size(n);
	((union pb_msg*) m)->maxsz = n;
	return n;
}

char *pb_parse_CodeGeneratorRequest(char *p, pb_buf_t *obj, struct CodeGeneratorRequest *m) {
	(void) obj;
	(void) m;
	if (!pb_parse_map(&p)) {
		return p;
	}
	for (;;) {
		struct pb_string key;
		switch (pb_parse_field(&p, &key, 121) % 10) {
		case 0:
			return p;
		case 6:
			if (pb_cmp(key, "file_to_generate")) {
				goto unknown;
			}
			p = pb_parse_array_string(p, obj, &m->file_to_generate.v, &m->file_to_generate.len);
			continue;
		case 5:
			if (pb_cmp(key, "parameter")) {
				goto unknown;
			}
			p = pb_parse_string(p, &m->parameter);
			continue;
		case 7:
			if (pb_cmp(key, "proto_file")) {
				goto unknown;
			}
			if (pb_parse_array(&p)) {
				struct FileDescriptorProto *prev = NULL;
				do {
					struct FileDescriptorProto *c = (struct FileDescriptorProto*) pb_calloc(obj, sizeof(struct FileDescriptorProto));
					if (!c) {return pb_errret;}
					p = pb_parse_FileDescriptorProto(p, obj, c);
					m->proto_file.len++;
					c->pb_hdr.prev = prev;
					prev = c;
				} while (pb_more_array(&p));

				m->proto_file.v = (const struct FileDescriptorProto**) pb_calloc(obj, m->proto_file.len * sizeof(struct FileDescriptorProto*));
				if (!m->proto_file.v) {return pb_errret;}

				for (int i = m->proto_file.len - 1; i >= 0; i--) {
					m->proto_file.v[i] = prev;
					prev = (struct FileDescriptorProto*) prev->pb_hdr.prev;
				}
			}
			continue;
		case 8:
			if (pb_cmp(key, "compiler_version")) {
				goto unknown;
			}
			m->compiler_version = (struct Version*) pb_calloc(obj, sizeof(struct Version));
			if (!m->compiler_version) {return pb_errret;}
			p = pb_parse_Version(p, obj, (struct Version*) m->compiler_version);
			continue;
		default:
		unknown:
			p = pb_parse_skip(p);
		}
	}
}
int pb_print_CodeGeneratorRequest(pb_buf_t *a, struct CodeGeneratorRequest const *m) {
	char *p = pb_appendsz(a, 1);
	if (!p) {return -1;}
	*p++ = '{';
	a->next = p;
	if (m->file_to_generate.len) {
		if (pb_append(a, "\"file_to_generate\":", 19)) {return -1;}
		if (pb_append(a, "[", 1)) {return -1;}
		for (int i = 0; i < m->file_to_generate.len; i++) {
			if (pb_print_string(a, m->file_to_generate.v[i])) {return -1;}
			a->next[-1] = ','; // replace trailing newline
		}
		if (pb_print_array_end(a)) {return -1;}
	}
	if (m->parameter.len) {
		if (pb_append(a, "\"parameter\":", 12)) {return -1;}
		if (pb_print_string(a, m->parameter)) {return -1;}
		a->next[-1] = ','; // replace trailing newline
	}
	if (m->proto_file.len) {
		if (pb_append(a, "\"proto_file\":", 13)) {return -1;}
		if (pb_append(a, "[", 1)) {return -1;}
		for (int i = 0; i < m->proto_file.len; i++) {
			if (pb_print_FileDescriptorProto(a, m->proto_file.v[i])) {return -1;}
			a->next[-1] = ','; // replace trailing newline
		}
		if (pb_print_array_end(a)) {return -1;}
	}
	if (m->compiler_version) {
		if (pb_append(a, "\"compiler_version\":", 19)) {return -1;}
		if (pb_print_Version(a, m->compiler_version)) {return -1;}
		a->next[-1] = ','; // replace trailing newline
	}
	return pb_print_map_end(a);
}
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
	if (pb_skipto_1(&p, e, 26)) {
		struct pb_string msg;
		p = pb_get_string(p + 1, e, &msg);
		m->compiler_version = (struct Version*) pb_calloc(obj, sizeof(struct Version));
		if (!m->compiler_version || pb_get_Version(msg.buf, msg.buf + msg.len, obj, (struct Version*) m->compiler_version)) {
			return -1;
		}
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
	if (m->compiler_version) {
		p = pb_put_tag_1(p, 26);
		int szlen = pb_u32_size(m->compiler_version->pb_hdr.maxsz);
		char *start = p+szlen;
		p = pb_encode_Version(start, m->compiler_version);
		pb_put_finish(p, start, szlen);
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
	if (m->compiler_version) {
		n += 1 + pb_maxsz_Version(m->compiler_version);
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

char *pb_parse_CodeGeneratorResponse(char *p, pb_buf_t *obj, struct CodeGeneratorResponse *m) {
	(void) obj;
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
					struct CodeGeneratorResponse_File *c = (struct CodeGeneratorResponse_File*) pb_calloc(obj, sizeof(struct CodeGeneratorResponse_File));
					if (!c) {return pb_errret;}
					p = pb_parse_CodeGeneratorResponse_File(p, obj, c);
					m->file.len++;
					c->pb_hdr.prev = prev;
					prev = c;
				} while (pb_more_array(&p));

				m->file.v = (const struct CodeGeneratorResponse_File**) pb_calloc(obj, m->file.len * sizeof(struct CodeGeneratorResponse_File*));
				if (!m->file.v) {return pb_errret;}

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
int pb_print_CodeGeneratorResponse(pb_buf_t *a, struct CodeGeneratorResponse const *m) {
	char *p = pb_appendsz(a, 1);
	if (!p) {return -1;}
	*p++ = '{';
	a->next = p;
	if (m->error.len) {
		if (pb_append(a, "\"error\":", 8)) {return -1;}
		if (pb_print_string(a, m->error)) {return -1;}
		a->next[-1] = ','; // replace trailing newline
	}
	if (m->file.len) {
		if (pb_append(a, "\"file\":", 7)) {return -1;}
		if (pb_append(a, "[", 1)) {return -1;}
		for (int i = 0; i < m->file.len; i++) {
			if (pb_print_CodeGeneratorResponse_File(a, m->file.v[i])) {return -1;}
			a->next[-1] = ','; // replace trailing newline
		}
		if (pb_print_array_end(a)) {return -1;}
	}
	return pb_print_map_end(a);
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

char *pb_parse_CodeGeneratorResponse_File(char *p, pb_buf_t *obj, struct CodeGeneratorResponse_File *m) {
	(void) obj;
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
int pb_print_CodeGeneratorResponse_File(pb_buf_t *a, struct CodeGeneratorResponse_File const *m) {
	char *p = pb_appendsz(a, 1);
	if (!p) {return -1;}
	*p++ = '{';
	a->next = p;
	if (m->name.len) {
		if (pb_append(a, "\"name\":", 7)) {return -1;}
		if (pb_print_string(a, m->name)) {return -1;}
		a->next[-1] = ','; // replace trailing newline
	}
	if (m->insertion_point.len) {
		if (pb_append(a, "\"insertion_point\":", 18)) {return -1;}
		if (pb_print_string(a, m->insertion_point)) {return -1;}
		a->next[-1] = ','; // replace trailing newline
	}
	if (m->content.len) {
		if (pb_append(a, "\"content\":", 10)) {return -1;}
		if (pb_print_string(a, m->content)) {return -1;}
		a->next[-1] = ','; // replace trailing newline
	}
	return pb_print_map_end(a);
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
