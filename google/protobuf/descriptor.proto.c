#include "google/protobuf/descriptor.proto.h"

char *pb_parse_FileDescriptorSet(char *p, pb_buf_t *obj, struct FileDescriptorSet *m) {
	(void) obj;
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
					struct FileDescriptorProto *c = (struct FileDescriptorProto*) pb_calloc(obj, sizeof(struct FileDescriptorProto));
					if (!c) {return pb_errret;}
					p = pb_parse_FileDescriptorProto(p, obj, c);
					m->file.len++;
					c->pb_hdr.prev = prev;
					prev = c;
				} while (pb_more_array(&p));

				m->file.v = (const struct FileDescriptorProto**) pb_calloc(obj, m->file.len * sizeof(struct FileDescriptorProto*));
				if (!m->file.v) {return pb_errret;}

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
int pb_print_FileDescriptorSet(pb_buf_t *a, struct FileDescriptorSet const *m) {
	char *p = pb_appendsz(a, 1);
	if (!p) {return -1;}
	*p++ = '{';
	a->next = p;
	if (m->file.len) {
		if (pb_append(a, "\"file\":", 7)) {return -1;}
		if (pb_append(a, "[", 1)) {return -1;}
		for (int i = 0; i < m->file.len; i++) {
			if (pb_print_FileDescriptorProto(a, m->file.v[i])) {return -1;}
			a->next[-1] = ','; // replace trailing newline
		}
		if (pb_print_array_end(a)) {return -1;}
	}
	return pb_print_map_end(a);
}
int pb_get_FileDescriptorSet(const char *p, const char *e, pb_buf_t *obj, struct FileDescriptorSet *m) {
	(void) obj;
	if (pb_skipto_1(&p, e, 10)) {
		struct FileDescriptorProto *prev = NULL;
		do {
			struct pb_string msg;
			p = pb_get_string(p + 1, e, &msg);
			struct FileDescriptorProto *c = (struct FileDescriptorProto*) pb_calloc(obj, sizeof(struct FileDescriptorProto));
			if (!c || pb_get_FileDescriptorProto(msg.buf, msg.buf + msg.len, obj, c)) {
				return -1;
			}
			m->file.len++;
			c->pb_hdr.prev = prev;
			prev = c;
		} while (!pb_cmp_tag_1(p, e, 10));

		m->file.v = (const struct FileDescriptorProto**) pb_calloc(obj, m->file.len * sizeof(struct FileDescriptorProto*));
		if (!m->file.v) {
			return -1;
		}

		for (int i = m->file.len - 1; i >= 0; i--) {
			m->file.v[i] = prev;
			prev = (struct FileDescriptorProto*) prev->pb_hdr.prev;
		}
	}
	return 0;
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

char *pb_parse_FileDescriptorProto(char *p, pb_buf_t *obj, struct FileDescriptorProto *m) {
	(void) obj;
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
			p = pb_parse_array_string(p, obj, &m->dependency.v, &m->dependency.len);
			continue;
		case 15:
			if (pb_cmp(key, "public_dependency")) {
				goto unknown;
			}
			p = pb_parse_array_i32(p, obj, &m->public_dependency.v, &m->public_dependency.len);
			continue;
		case 4:
			if (pb_cmp(key, "weak_dependency")) {
				goto unknown;
			}
			p = pb_parse_array_i32(p, obj, &m->weak_dependency.v, &m->weak_dependency.len);
			continue;
		case 10:
			if (pb_cmp(key, "message_type")) {
				goto unknown;
			}
			if (pb_parse_array(&p)) {
				struct DescriptorProto *prev = NULL;
				do {
					struct DescriptorProto *c = (struct DescriptorProto*) pb_calloc(obj, sizeof(struct DescriptorProto));
					if (!c) {return pb_errret;}
					p = pb_parse_DescriptorProto(p, obj, c);
					m->message_type.len++;
					c->pb_hdr.prev = prev;
					prev = c;
				} while (pb_more_array(&p));

				m->message_type.v = (const struct DescriptorProto**) pb_calloc(obj, m->message_type.len * sizeof(struct DescriptorProto*));
				if (!m->message_type.v) {return pb_errret;}

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
					struct EnumDescriptorProto *c = (struct EnumDescriptorProto*) pb_calloc(obj, sizeof(struct EnumDescriptorProto));
					if (!c) {return pb_errret;}
					p = pb_parse_EnumDescriptorProto(p, obj, c);
					m->enum_type.len++;
					c->pb_hdr.prev = prev;
					prev = c;
				} while (pb_more_array(&p));

				m->enum_type.v = (const struct EnumDescriptorProto**) pb_calloc(obj, m->enum_type.len * sizeof(struct EnumDescriptorProto*));
				if (!m->enum_type.v) {return pb_errret;}

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
					struct ServiceDescriptorProto *c = (struct ServiceDescriptorProto*) pb_calloc(obj, sizeof(struct ServiceDescriptorProto));
					if (!c) {return pb_errret;}
					p = pb_parse_ServiceDescriptorProto(p, obj, c);
					m->service.len++;
					c->pb_hdr.prev = prev;
					prev = c;
				} while (pb_more_array(&p));

				m->service.v = (const struct ServiceDescriptorProto**) pb_calloc(obj, m->service.len * sizeof(struct ServiceDescriptorProto*));
				if (!m->service.v) {return pb_errret;}

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
					struct FieldDescriptorProto *c = (struct FieldDescriptorProto*) pb_calloc(obj, sizeof(struct FieldDescriptorProto));
					if (!c) {return pb_errret;}
					p = pb_parse_FieldDescriptorProto(p, obj, c);
					m->extension.len++;
					c->pb_hdr.prev = prev;
					prev = c;
				} while (pb_more_array(&p));

				m->extension.v = (const struct FieldDescriptorProto**) pb_calloc(obj, m->extension.len * sizeof(struct FieldDescriptorProto*));
				if (!m->extension.v) {return pb_errret;}

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
			m->options = (struct FileOptions*) pb_calloc(obj, sizeof(struct FileOptions));
			if (!m->options) {return pb_errret;}
			p = pb_parse_FileOptions(p, obj, (struct FileOptions*) m->options);
			continue;
		case 18:
			if (pb_cmp(key, "source_code_info")) {
				goto unknown;
			}
			m->source_code_info = (struct SourceCodeInfo*) pb_calloc(obj, sizeof(struct SourceCodeInfo));
			if (!m->source_code_info) {return pb_errret;}
			p = pb_parse_SourceCodeInfo(p, obj, (struct SourceCodeInfo*) m->source_code_info);
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
int pb_print_FileDescriptorProto(pb_buf_t *a, struct FileDescriptorProto const *m) {
	char *p = pb_appendsz(a, 1);
	if (!p) {return -1;}
	*p++ = '{';
	a->next = p;
	if (m->name.len) {
		if (pb_append(a, "\"name\":", 7)) {return -1;}
		if (pb_print_string(a, m->name)) {return -1;}
		a->next[-1] = ','; // replace trailing newline
	}
	if (m->package.len) {
		if (pb_append(a, "\"package\":", 10)) {return -1;}
		if (pb_print_string(a, m->package)) {return -1;}
		a->next[-1] = ','; // replace trailing newline
	}
	if (m->dependency.len) {
		if (pb_append(a, "\"dependency\":", 13)) {return -1;}
		if (pb_append(a, "[", 1)) {return -1;}
		for (int i = 0; i < m->dependency.len; i++) {
			if (pb_print_string(a, m->dependency.v[i])) {return -1;}
			a->next[-1] = ','; // replace trailing newline
		}
		if (pb_print_array_end(a)) {return -1;}
	}
	if (m->public_dependency.len) {
		if (pb_append(a, "\"public_dependency\":", 20)) {return -1;}
		char *mp = pb_appendsz(a, 3 /*[],*/ + m->public_dependency.len * 12);
		if (!mp) {return -1;}
		*mp++ = '[';
		for (int i = 0; i < m->public_dependency.len; i++) {
			mp = pb_print_i32(mp, m->public_dependency.v[i]);
		}
		mp = pb_print_array_end_i(mp);
		a->next = mp;
	}
	if (m->weak_dependency.len) {
		if (pb_append(a, "\"weak_dependency\":", 18)) {return -1;}
		char *mp = pb_appendsz(a, 3 /*[],*/ + m->weak_dependency.len * 12);
		if (!mp) {return -1;}
		*mp++ = '[';
		for (int i = 0; i < m->weak_dependency.len; i++) {
			mp = pb_print_i32(mp, m->weak_dependency.v[i]);
		}
		mp = pb_print_array_end_i(mp);
		a->next = mp;
	}
	if (m->message_type.len) {
		if (pb_append(a, "\"message_type\":", 15)) {return -1;}
		if (pb_append(a, "[", 1)) {return -1;}
		for (int i = 0; i < m->message_type.len; i++) {
			if (pb_print_DescriptorProto(a, m->message_type.v[i])) {return -1;}
			a->next[-1] = ','; // replace trailing newline
		}
		if (pb_print_array_end(a)) {return -1;}
	}
	if (m->enum_type.len) {
		if (pb_append(a, "\"enum_type\":", 12)) {return -1;}
		if (pb_append(a, "[", 1)) {return -1;}
		for (int i = 0; i < m->enum_type.len; i++) {
			if (pb_print_EnumDescriptorProto(a, m->enum_type.v[i])) {return -1;}
			a->next[-1] = ','; // replace trailing newline
		}
		if (pb_print_array_end(a)) {return -1;}
	}
	if (m->service.len) {
		if (pb_append(a, "\"service\":", 10)) {return -1;}
		if (pb_append(a, "[", 1)) {return -1;}
		for (int i = 0; i < m->service.len; i++) {
			if (pb_print_ServiceDescriptorProto(a, m->service.v[i])) {return -1;}
			a->next[-1] = ','; // replace trailing newline
		}
		if (pb_print_array_end(a)) {return -1;}
	}
	if (m->extension.len) {
		if (pb_append(a, "\"extension\":", 12)) {return -1;}
		if (pb_append(a, "[", 1)) {return -1;}
		for (int i = 0; i < m->extension.len; i++) {
			if (pb_print_FieldDescriptorProto(a, m->extension.v[i])) {return -1;}
			a->next[-1] = ','; // replace trailing newline
		}
		if (pb_print_array_end(a)) {return -1;}
	}
	if (m->options) {
		if (pb_append(a, "\"options\":", 10)) {return -1;}
		if (pb_print_FileOptions(a, m->options)) {return -1;}
		a->next[-1] = ','; // replace trailing newline
	}
	if (m->source_code_info) {
		if (pb_append(a, "\"source_code_info\":", 19)) {return -1;}
		if (pb_print_SourceCodeInfo(a, m->source_code_info)) {return -1;}
		a->next[-1] = ','; // replace trailing newline
	}
	if (m->syntax.len) {
		if (pb_append(a, "\"syntax\":", 9)) {return -1;}
		if (pb_print_string(a, m->syntax)) {return -1;}
		a->next[-1] = ','; // replace trailing newline
	}
	return pb_print_map_end(a);
}
int pb_get_FileDescriptorProto(const char *p, const char *e, pb_buf_t *obj, struct FileDescriptorProto *m) {
	(void) obj;
	if (pb_skipto_1(&p, e, 10)) {
		p = pb_get_string(p + 1, e, (struct pb_string*)&m->name);
	}
	if (pb_skipto_1(&p, e, 18)) {
		p = pb_get_string(p + 1, e, (struct pb_string*)&m->package);
	}
	if (pb_skipto_1(&p, e, 26)) {
		m->dependency.v = (struct pb_string*) obj->next;
		do {
			if (obj->next + m->dependency.len * sizeof(struct pb_string) > obj->end) {
				return -1;
			}
			p = pb_get_string(p + 1, e, (struct pb_string*)&m->dependency.v[m->dependency.len]);
			m->dependency.len++;
		} while (!pb_cmp_tag_1(p, e, 26));

		obj->next += m->dependency.len * sizeof(struct pb_string);
	}
	if (pb_skipto_1(&p, e, 34)) {
		struct DescriptorProto *prev = NULL;
		do {
			struct pb_string msg;
			p = pb_get_string(p + 1, e, &msg);
			struct DescriptorProto *c = (struct DescriptorProto*) pb_calloc(obj, sizeof(struct DescriptorProto));
			if (!c || pb_get_DescriptorProto(msg.buf, msg.buf + msg.len, obj, c)) {
				return -1;
			}
			m->message_type.len++;
			c->pb_hdr.prev = prev;
			prev = c;
		} while (!pb_cmp_tag_1(p, e, 34));

		m->message_type.v = (const struct DescriptorProto**) pb_calloc(obj, m->message_type.len * sizeof(struct DescriptorProto*));
		if (!m->message_type.v) {
			return -1;
		}

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
			struct EnumDescriptorProto *c = (struct EnumDescriptorProto*) pb_calloc(obj, sizeof(struct EnumDescriptorProto));
			if (!c || pb_get_EnumDescriptorProto(msg.buf, msg.buf + msg.len, obj, c)) {
				return -1;
			}
			m->enum_type.len++;
			c->pb_hdr.prev = prev;
			prev = c;
		} while (!pb_cmp_tag_1(p, e, 42));

		m->enum_type.v = (const struct EnumDescriptorProto**) pb_calloc(obj, m->enum_type.len * sizeof(struct EnumDescriptorProto*));
		if (!m->enum_type.v) {
			return -1;
		}

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
			struct ServiceDescriptorProto *c = (struct ServiceDescriptorProto*) pb_calloc(obj, sizeof(struct ServiceDescriptorProto));
			if (!c || pb_get_ServiceDescriptorProto(msg.buf, msg.buf + msg.len, obj, c)) {
				return -1;
			}
			m->service.len++;
			c->pb_hdr.prev = prev;
			prev = c;
		} while (!pb_cmp_tag_1(p, e, 50));

		m->service.v = (const struct ServiceDescriptorProto**) pb_calloc(obj, m->service.len * sizeof(struct ServiceDescriptorProto*));
		if (!m->service.v) {
			return -1;
		}

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
			struct FieldDescriptorProto *c = (struct FieldDescriptorProto*) pb_calloc(obj, sizeof(struct FieldDescriptorProto));
			if (!c || pb_get_FieldDescriptorProto(msg.buf, msg.buf + msg.len, obj, c)) {
				return -1;
			}
			m->extension.len++;
			c->pb_hdr.prev = prev;
			prev = c;
		} while (!pb_cmp_tag_1(p, e, 58));

		m->extension.v = (const struct FieldDescriptorProto**) pb_calloc(obj, m->extension.len * sizeof(struct FieldDescriptorProto*));
		if (!m->extension.v) {
			return -1;
		}

		for (int i = m->extension.len - 1; i >= 0; i--) {
			m->extension.v[i] = prev;
			prev = (struct FieldDescriptorProto*) prev->pb_hdr.prev;
		}
	}
	if (pb_skipto_1(&p, e, 66)) {
		struct pb_string msg;
		p = pb_get_string(p + 1, e, &msg);
		m->options = (struct FileOptions*) pb_calloc(obj, sizeof(struct FileOptions));
		if (!m->options || pb_get_FileOptions(msg.buf, msg.buf + msg.len, obj, (struct FileOptions*) m->options)) {
			return -1;
		}
	}
	if (pb_skipto_1(&p, e, 74)) {
		struct pb_string msg;
		p = pb_get_string(p + 1, e, &msg);
		m->source_code_info = (struct SourceCodeInfo*) pb_calloc(obj, sizeof(struct SourceCodeInfo));
		if (!m->source_code_info || pb_get_SourceCodeInfo(msg.buf, msg.buf + msg.len, obj, (struct SourceCodeInfo*) m->source_code_info)) {
			return -1;
		}
	}
	if (pb_skipto_1(&p, e, 82)) {
		m->public_dependency.v = (int32_t*) obj->next;
		do {
			if (obj->next + m->public_dependency.len * sizeof(int32_t) > obj->end) {
				return -1;
			}
			p = pb_get_u32(p + 1, e, (uint32_t*)&m->public_dependency.v[m->public_dependency.len]);
			m->public_dependency.len++;
		} while (!pb_cmp_tag_1(p, e, 82));

		obj->next += m->public_dependency.len * sizeof(int32_t);
	}
	if (pb_skipto_1(&p, e, 90)) {
		m->weak_dependency.v = (int32_t*) obj->next;
		do {
			if (obj->next + m->weak_dependency.len * sizeof(int32_t) > obj->end) {
				return -1;
			}
			p = pb_get_u32(p + 1, e, (uint32_t*)&m->weak_dependency.v[m->weak_dependency.len]);
			m->weak_dependency.len++;
		} while (!pb_cmp_tag_1(p, e, 90));

		obj->next += m->weak_dependency.len * sizeof(int32_t);
	}
	if (pb_skipto_1(&p, e, 98)) {
		p = pb_get_string(p + 1, e, (struct pb_string*)&m->syntax);
	}
	return 0;
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

char *pb_parse_DescriptorProto(char *p, pb_buf_t *obj, struct DescriptorProto *m) {
	(void) obj;
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
					struct FieldDescriptorProto *c = (struct FieldDescriptorProto*) pb_calloc(obj, sizeof(struct FieldDescriptorProto));
					if (!c) {return pb_errret;}
					p = pb_parse_FieldDescriptorProto(p, obj, c);
					m->field.len++;
					c->pb_hdr.prev = prev;
					prev = c;
				} while (pb_more_array(&p));

				m->field.v = (const struct FieldDescriptorProto**) pb_calloc(obj, m->field.len * sizeof(struct FieldDescriptorProto*));
				if (!m->field.v) {return pb_errret;}

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
					struct FieldDescriptorProto *c = (struct FieldDescriptorProto*) pb_calloc(obj, sizeof(struct FieldDescriptorProto));
					if (!c) {return pb_errret;}
					p = pb_parse_FieldDescriptorProto(p, obj, c);
					m->extension.len++;
					c->pb_hdr.prev = prev;
					prev = c;
				} while (pb_more_array(&p));

				m->extension.v = (const struct FieldDescriptorProto**) pb_calloc(obj, m->extension.len * sizeof(struct FieldDescriptorProto*));
				if (!m->extension.v) {return pb_errret;}

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
					struct DescriptorProto *c = (struct DescriptorProto*) pb_calloc(obj, sizeof(struct DescriptorProto));
					if (!c) {return pb_errret;}
					p = pb_parse_DescriptorProto(p, obj, c);
					m->nested_type.len++;
					c->pb_hdr.prev = prev;
					prev = c;
				} while (pb_more_array(&p));

				m->nested_type.v = (const struct DescriptorProto**) pb_calloc(obj, m->nested_type.len * sizeof(struct DescriptorProto*));
				if (!m->nested_type.v) {return pb_errret;}

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
					struct EnumDescriptorProto *c = (struct EnumDescriptorProto*) pb_calloc(obj, sizeof(struct EnumDescriptorProto));
					if (!c) {return pb_errret;}
					p = pb_parse_EnumDescriptorProto(p, obj, c);
					m->enum_type.len++;
					c->pb_hdr.prev = prev;
					prev = c;
				} while (pb_more_array(&p));

				m->enum_type.v = (const struct EnumDescriptorProto**) pb_calloc(obj, m->enum_type.len * sizeof(struct EnumDescriptorProto*));
				if (!m->enum_type.v) {return pb_errret;}

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
				struct DescriptorProto_ExtensionRange *prev = NULL;
				do {
					struct DescriptorProto_ExtensionRange *c = (struct DescriptorProto_ExtensionRange*) pb_calloc(obj, sizeof(struct DescriptorProto_ExtensionRange));
					if (!c) {return pb_errret;}
					p = pb_parse_DescriptorProto_ExtensionRange(p, obj, c);
					m->extension_range.len++;
					c->pb_hdr.prev = prev;
					prev = c;
				} while (pb_more_array(&p));

				m->extension_range.v = (const struct DescriptorProto_ExtensionRange**) pb_calloc(obj, m->extension_range.len * sizeof(struct DescriptorProto_ExtensionRange*));
				if (!m->extension_range.v) {return pb_errret;}

				for (int i = m->extension_range.len - 1; i >= 0; i--) {
					m->extension_range.v[i] = prev;
					prev = (struct DescriptorProto_ExtensionRange*) prev->pb_hdr.prev;
				}
			}
			continue;
		case 34:
			if (pb_cmp(key, "oneof_decl")) {
				goto unknown;
			}
			if (pb_parse_array(&p)) {
				struct OneofDescriptorProto *prev = NULL;
				do {
					struct OneofDescriptorProto *c = (struct OneofDescriptorProto*) pb_calloc(obj, sizeof(struct OneofDescriptorProto));
					if (!c) {return pb_errret;}
					p = pb_parse_OneofDescriptorProto(p, obj, c);
					m->oneof_decl.len++;
					c->pb_hdr.prev = prev;
					prev = c;
				} while (pb_more_array(&p));

				m->oneof_decl.v = (const struct OneofDescriptorProto**) pb_calloc(obj, m->oneof_decl.len * sizeof(struct OneofDescriptorProto*));
				if (!m->oneof_decl.v) {return pb_errret;}

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
			m->options = (struct MessageOptions*) pb_calloc(obj, sizeof(struct MessageOptions));
			if (!m->options) {return pb_errret;}
			p = pb_parse_MessageOptions(p, obj, (struct MessageOptions*) m->options);
			continue;
		case 5:
			if (pb_cmp(key, "reserved_range")) {
				goto unknown;
			}
			if (pb_parse_array(&p)) {
				int n = 0;
				m->reserved_range.v = (struct DescriptorProto_ReservedRange*) obj->next;
				do {
					if (obj->next + n * sizeof(struct DescriptorProto_ReservedRange) > obj->end) {return pb_errret;}
					p = pb_parse_DescriptorProto_ReservedRange(p, (struct DescriptorProto_ReservedRange*) &m->reserved_range.v[n++]); 
				} while (pb_more_array(&p));
				obj->next += n * sizeof(struct DescriptorProto_ReservedRange);
				m->reserved_range.len = n;
			}
			continue;
		case 3:
			if (pb_cmp(key, "reserved_name")) {
				goto unknown;
			}
			p = pb_parse_array_string(p, obj, &m->reserved_name.v, &m->reserved_name.len);
			continue;
		default:
		unknown:
			p = pb_parse_skip(p);
		}
	}
}
int pb_print_DescriptorProto(pb_buf_t *a, struct DescriptorProto const *m) {
	char *p = pb_appendsz(a, 1);
	if (!p) {return -1;}
	*p++ = '{';
	a->next = p;
	if (m->name.len) {
		if (pb_append(a, "\"name\":", 7)) {return -1;}
		if (pb_print_string(a, m->name)) {return -1;}
		a->next[-1] = ','; // replace trailing newline
	}
	if (m->field.len) {
		if (pb_append(a, "\"field\":", 8)) {return -1;}
		if (pb_append(a, "[", 1)) {return -1;}
		for (int i = 0; i < m->field.len; i++) {
			if (pb_print_FieldDescriptorProto(a, m->field.v[i])) {return -1;}
			a->next[-1] = ','; // replace trailing newline
		}
		if (pb_print_array_end(a)) {return -1;}
	}
	if (m->extension.len) {
		if (pb_append(a, "\"extension\":", 12)) {return -1;}
		if (pb_append(a, "[", 1)) {return -1;}
		for (int i = 0; i < m->extension.len; i++) {
			if (pb_print_FieldDescriptorProto(a, m->extension.v[i])) {return -1;}
			a->next[-1] = ','; // replace trailing newline
		}
		if (pb_print_array_end(a)) {return -1;}
	}
	if (m->nested_type.len) {
		if (pb_append(a, "\"nested_type\":", 14)) {return -1;}
		if (pb_append(a, "[", 1)) {return -1;}
		for (int i = 0; i < m->nested_type.len; i++) {
			if (pb_print_DescriptorProto(a, m->nested_type.v[i])) {return -1;}
			a->next[-1] = ','; // replace trailing newline
		}
		if (pb_print_array_end(a)) {return -1;}
	}
	if (m->enum_type.len) {
		if (pb_append(a, "\"enum_type\":", 12)) {return -1;}
		if (pb_append(a, "[", 1)) {return -1;}
		for (int i = 0; i < m->enum_type.len; i++) {
			if (pb_print_EnumDescriptorProto(a, m->enum_type.v[i])) {return -1;}
			a->next[-1] = ','; // replace trailing newline
		}
		if (pb_print_array_end(a)) {return -1;}
	}
	if (m->extension_range.len) {
		if (pb_append(a, "\"extension_range\":", 18)) {return -1;}
		if (pb_append(a, "[", 1)) {return -1;}
		for (int i = 0; i < m->extension_range.len; i++) {
			if (pb_print_DescriptorProto_ExtensionRange(a, m->extension_range.v[i])) {return -1;}
			a->next[-1] = ','; // replace trailing newline
		}
		if (pb_print_array_end(a)) {return -1;}
	}
	if (m->oneof_decl.len) {
		if (pb_append(a, "\"oneof_decl\":", 13)) {return -1;}
		if (pb_append(a, "[", 1)) {return -1;}
		for (int i = 0; i < m->oneof_decl.len; i++) {
			if (pb_print_OneofDescriptorProto(a, m->oneof_decl.v[i])) {return -1;}
			a->next[-1] = ','; // replace trailing newline
		}
		if (pb_print_array_end(a)) {return -1;}
	}
	if (m->options) {
		if (pb_append(a, "\"options\":", 10)) {return -1;}
		if (pb_print_MessageOptions(a, m->options)) {return -1;}
		a->next[-1] = ','; // replace trailing newline
	}
	if (m->reserved_range.len) {
		if (pb_append(a, "\"reserved_range\":", 17)) {return -1;}
		if (pb_append(a, "[", 1)) {return -1;}
		for (int i = 0; i < m->reserved_range.len; i++) {
			if (pb_print_DescriptorProto_ReservedRange(a, &m->reserved_range.v[i])) {return -1;}
			a->next[-1] = ','; // replace trailing newline
		}
		if (pb_print_array_end(a)) {return -1;}
	}
	if (m->reserved_name.len) {
		if (pb_append(a, "\"reserved_name\":", 16)) {return -1;}
		if (pb_append(a, "[", 1)) {return -1;}
		for (int i = 0; i < m->reserved_name.len; i++) {
			if (pb_print_string(a, m->reserved_name.v[i])) {return -1;}
			a->next[-1] = ','; // replace trailing newline
		}
		if (pb_print_array_end(a)) {return -1;}
	}
	return pb_print_map_end(a);
}
int pb_get_DescriptorProto(const char *p, const char *e, pb_buf_t *obj, struct DescriptorProto *m) {
	(void) obj;
	if (pb_skipto_1(&p, e, 10)) {
		p = pb_get_string(p + 1, e, (struct pb_string*)&m->name);
	}
	if (pb_skipto_1(&p, e, 18)) {
		struct FieldDescriptorProto *prev = NULL;
		do {
			struct pb_string msg;
			p = pb_get_string(p + 1, e, &msg);
			struct FieldDescriptorProto *c = (struct FieldDescriptorProto*) pb_calloc(obj, sizeof(struct FieldDescriptorProto));
			if (!c || pb_get_FieldDescriptorProto(msg.buf, msg.buf + msg.len, obj, c)) {
				return -1;
			}
			m->field.len++;
			c->pb_hdr.prev = prev;
			prev = c;
		} while (!pb_cmp_tag_1(p, e, 18));

		m->field.v = (const struct FieldDescriptorProto**) pb_calloc(obj, m->field.len * sizeof(struct FieldDescriptorProto*));
		if (!m->field.v) {
			return -1;
		}

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
			struct DescriptorProto *c = (struct DescriptorProto*) pb_calloc(obj, sizeof(struct DescriptorProto));
			if (!c || pb_get_DescriptorProto(msg.buf, msg.buf + msg.len, obj, c)) {
				return -1;
			}
			m->nested_type.len++;
			c->pb_hdr.prev = prev;
			prev = c;
		} while (!pb_cmp_tag_1(p, e, 26));

		m->nested_type.v = (const struct DescriptorProto**) pb_calloc(obj, m->nested_type.len * sizeof(struct DescriptorProto*));
		if (!m->nested_type.v) {
			return -1;
		}

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
			struct EnumDescriptorProto *c = (struct EnumDescriptorProto*) pb_calloc(obj, sizeof(struct EnumDescriptorProto));
			if (!c || pb_get_EnumDescriptorProto(msg.buf, msg.buf + msg.len, obj, c)) {
				return -1;
			}
			m->enum_type.len++;
			c->pb_hdr.prev = prev;
			prev = c;
		} while (!pb_cmp_tag_1(p, e, 34));

		m->enum_type.v = (const struct EnumDescriptorProto**) pb_calloc(obj, m->enum_type.len * sizeof(struct EnumDescriptorProto*));
		if (!m->enum_type.v) {
			return -1;
		}

		for (int i = m->enum_type.len - 1; i >= 0; i--) {
			m->enum_type.v[i] = prev;
			prev = (struct EnumDescriptorProto*) prev->pb_hdr.prev;
		}
	}
	if (pb_skipto_1(&p, e, 42)) {
		struct DescriptorProto_ExtensionRange *prev = NULL;
		do {
			struct pb_string msg;
			p = pb_get_string(p + 1, e, &msg);
			struct DescriptorProto_ExtensionRange *c = (struct DescriptorProto_ExtensionRange*) pb_calloc(obj, sizeof(struct DescriptorProto_ExtensionRange));
			if (!c || pb_get_DescriptorProto_ExtensionRange(msg.buf, msg.buf + msg.len, obj, c)) {
				return -1;
			}
			m->extension_range.len++;
			c->pb_hdr.prev = prev;
			prev = c;
		} while (!pb_cmp_tag_1(p, e, 42));

		m->extension_range.v = (const struct DescriptorProto_ExtensionRange**) pb_calloc(obj, m->extension_range.len * sizeof(struct DescriptorProto_ExtensionRange*));
		if (!m->extension_range.v) {
			return -1;
		}

		for (int i = m->extension_range.len - 1; i >= 0; i--) {
			m->extension_range.v[i] = prev;
			prev = (struct DescriptorProto_ExtensionRange*) prev->pb_hdr.prev;
		}
	}
	if (pb_skipto_1(&p, e, 50)) {
		struct FieldDescriptorProto *prev = NULL;
		do {
			struct pb_string msg;
			p = pb_get_string(p + 1, e, &msg);
			struct FieldDescriptorProto *c = (struct FieldDescriptorProto*) pb_calloc(obj, sizeof(struct FieldDescriptorProto));
			if (!c || pb_get_FieldDescriptorProto(msg.buf, msg.buf + msg.len, obj, c)) {
				return -1;
			}
			m->extension.len++;
			c->pb_hdr.prev = prev;
			prev = c;
		} while (!pb_cmp_tag_1(p, e, 50));

		m->extension.v = (const struct FieldDescriptorProto**) pb_calloc(obj, m->extension.len * sizeof(struct FieldDescriptorProto*));
		if (!m->extension.v) {
			return -1;
		}

		for (int i = m->extension.len - 1; i >= 0; i--) {
			m->extension.v[i] = prev;
			prev = (struct FieldDescriptorProto*) prev->pb_hdr.prev;
		}
	}
	if (pb_skipto_1(&p, e, 58)) {
		struct pb_string msg;
		p = pb_get_string(p + 1, e, &msg);
		m->options = (struct MessageOptions*) pb_calloc(obj, sizeof(struct MessageOptions));
		if (!m->options || pb_get_MessageOptions(msg.buf, msg.buf + msg.len, obj, (struct MessageOptions*) m->options)) {
			return -1;
		}
	}
	if (pb_skipto_1(&p, e, 66)) {
		struct OneofDescriptorProto *prev = NULL;
		do {
			struct pb_string msg;
			p = pb_get_string(p + 1, e, &msg);
			struct OneofDescriptorProto *c = (struct OneofDescriptorProto*) pb_calloc(obj, sizeof(struct OneofDescriptorProto));
			if (!c || pb_get_OneofDescriptorProto(msg.buf, msg.buf + msg.len, obj, c)) {
				return -1;
			}
			m->oneof_decl.len++;
			c->pb_hdr.prev = prev;
			prev = c;
		} while (!pb_cmp_tag_1(p, e, 66));

		m->oneof_decl.v = (const struct OneofDescriptorProto**) pb_calloc(obj, m->oneof_decl.len * sizeof(struct OneofDescriptorProto*));
		if (!m->oneof_decl.v) {
			return -1;
		}

		for (int i = m->oneof_decl.len - 1; i >= 0; i--) {
			m->oneof_decl.v[i] = prev;
			prev = (struct OneofDescriptorProto*) prev->pb_hdr.prev;
		}
	}
	if (pb_skipto_1(&p, e, 74)) {
		m->reserved_range.v = (struct DescriptorProto_ReservedRange*) obj->next;
		do {
			if (obj->next + m->reserved_range.len * sizeof(struct DescriptorProto_ReservedRange) > obj->end) {
				return -1;
			}
			struct pb_string msg;
			p = pb_get_string(p + 1, e, &msg);
			if (pb_get_DescriptorProto_ReservedRange(msg.buf, msg.buf + msg.len, (struct DescriptorProto_ReservedRange*) &m->reserved_range.v[m->reserved_range.len])) {
				return -1;
			}
			m->reserved_range.len++;
		} while (!pb_cmp_tag_1(p, e, 74));

		obj->next += m->reserved_range.len * sizeof(struct DescriptorProto_ReservedRange);
	}
	if (pb_skipto_1(&p, e, 82)) {
		m->reserved_name.v = (struct pb_string*) obj->next;
		do {
			if (obj->next + m->reserved_name.len * sizeof(struct pb_string) > obj->end) {
				return -1;
			}
			p = pb_get_string(p + 1, e, (struct pb_string*)&m->reserved_name.v[m->reserved_name.len]);
			m->reserved_name.len++;
		} while (!pb_cmp_tag_1(p, e, 82));

		obj->next += m->reserved_name.len * sizeof(struct pb_string);
	}
	return 0;
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
			int szlen = pb_u32_size(m->extension_range.v[i]->pb_hdr.maxsz);
			char *start = p+szlen;
			p = pb_encode_DescriptorProto_ExtensionRange(start, m->extension_range.v[i]);
			pb_put_finish(p, start, szlen);
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
		for (int i = 0; i < m->extension_range.len; i++) {
			n += 1 + pb_maxsz_DescriptorProto_ExtensionRange(m->extension_range.v[i]);
		}
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

char *pb_parse_DescriptorProto_ExtensionRange(char *p, pb_buf_t *obj, struct DescriptorProto_ExtensionRange *m) {
	(void) obj;
	(void) m;
	if (!pb_parse_map(&p)) {
		return p;
	}
	for (;;) {
		struct pb_string key;
		switch (pb_parse_field(&p, &key, 121) % 9) {
		case 0:
			return p;
		case 7:
			if (pb_cmp(key, "start")) {
				goto unknown;
			}
			p = pb_parse_i32(p, &m->start);
			continue;
		case 5:
			if (pb_cmp(key, "end")) {
				goto unknown;
			}
			p = pb_parse_i32(p, &m->end);
			continue;
		case 6:
			if (pb_cmp(key, "options")) {
				goto unknown;
			}
			m->options = (struct ExtensionRangeOptions*) pb_calloc(obj, sizeof(struct ExtensionRangeOptions));
			if (!m->options) {return pb_errret;}
			p = pb_parse_ExtensionRangeOptions(p, obj, (struct ExtensionRangeOptions*) m->options);
			continue;
		default:
		unknown:
			p = pb_parse_skip(p);
		}
	}
}
int pb_print_DescriptorProto_ExtensionRange(pb_buf_t *a, struct DescriptorProto_ExtensionRange const *m) {
	char *p = pb_appendsz(a, 39);
	if (!p) {return -1;}
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
	a->next = p;
	if (m->options) {
		if (pb_append(a, "\"options\":", 10)) {return -1;}
		if (pb_print_ExtensionRangeOptions(a, m->options)) {return -1;}
		a->next[-1] = ','; // replace trailing newline
	}
	return pb_print_map_end(a);
}
int pb_get_DescriptorProto_ExtensionRange(const char *p, const char *e, pb_buf_t *obj, struct DescriptorProto_ExtensionRange *m) {
	(void) obj;
	if (pb_skipto_1(&p, e, 8)) {
		p = pb_get_u32(p + 1, e, (uint32_t*)&m->start);
	}
	if (pb_skipto_1(&p, e, 16)) {
		p = pb_get_u32(p + 1, e, (uint32_t*)&m->end);
	}
	if (pb_skipto_1(&p, e, 26)) {
		struct pb_string msg;
		p = pb_get_string(p + 1, e, &msg);
		m->options = (struct ExtensionRangeOptions*) pb_calloc(obj, sizeof(struct ExtensionRangeOptions));
		if (!m->options || pb_get_ExtensionRangeOptions(msg.buf, msg.buf + msg.len, obj, (struct ExtensionRangeOptions*) m->options)) {
			return -1;
		}
	}
	return 0;
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
	if (m->options) {
		p = pb_put_tag_1(p, 26);
		int szlen = pb_u32_size(m->options->pb_hdr.maxsz);
		char *start = p+szlen;
		p = pb_encode_ExtensionRangeOptions(start, m->options);
		pb_put_finish(p, start, szlen);
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
	if (m->options) {
		n += 1 + pb_maxsz_ExtensionRangeOptions(m->options);
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
int pb_print_DescriptorProto_ReservedRange(pb_buf_t *a, struct DescriptorProto_ReservedRange const *m) {
	char *p = pb_appendsz(a, 39);
	if (!p) {return -1;}
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
	a->next = p;
	return pb_print_map_end(a);
}
int pb_get_DescriptorProto_ReservedRange(const char *p, const char *e, struct DescriptorProto_ReservedRange *m) {
	if (pb_skipto_1(&p, e, 8)) {
		p = pb_get_u32(p + 1, e, (uint32_t*)&m->start);
	}
	if (pb_skipto_1(&p, e, 16)) {
		p = pb_get_u32(p + 1, e, (uint32_t*)&m->end);
	}
	return 0;
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

char *pb_parse_ExtensionRangeOptions(char *p, pb_buf_t *obj, struct ExtensionRangeOptions *m) {
	(void) obj;
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
					struct UninterpretedOption *c = (struct UninterpretedOption*) pb_calloc(obj, sizeof(struct UninterpretedOption));
					if (!c) {return pb_errret;}
					p = pb_parse_UninterpretedOption(p, obj, c);
					m->uninterpreted_option.len++;
					c->pb_hdr.prev = prev;
					prev = c;
				} while (pb_more_array(&p));

				m->uninterpreted_option.v = (const struct UninterpretedOption**) pb_calloc(obj, m->uninterpreted_option.len * sizeof(struct UninterpretedOption*));
				if (!m->uninterpreted_option.v) {return pb_errret;}

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
int pb_print_ExtensionRangeOptions(pb_buf_t *a, struct ExtensionRangeOptions const *m) {
	char *p = pb_appendsz(a, 1);
	if (!p) {return -1;}
	*p++ = '{';
	a->next = p;
	if (m->uninterpreted_option.len) {
		if (pb_append(a, "\"uninterpreted_option\":", 23)) {return -1;}
		if (pb_append(a, "[", 1)) {return -1;}
		for (int i = 0; i < m->uninterpreted_option.len; i++) {
			if (pb_print_UninterpretedOption(a, m->uninterpreted_option.v[i])) {return -1;}
			a->next[-1] = ','; // replace trailing newline
		}
		if (pb_print_array_end(a)) {return -1;}
	}
	return pb_print_map_end(a);
}
int pb_get_ExtensionRangeOptions(const char *p, const char *e, pb_buf_t *obj, struct ExtensionRangeOptions *m) {
	(void) obj;
	p = pb_toend_1(p,e);
	if (pb_skipto_2(&p, e, 16058)) {
		struct UninterpretedOption *prev = NULL;
		do {
			struct pb_string msg;
			p = pb_get_string(p + 2, e, &msg);
			struct UninterpretedOption *c = (struct UninterpretedOption*) pb_calloc(obj, sizeof(struct UninterpretedOption));
			if (!c || pb_get_UninterpretedOption(msg.buf, msg.buf + msg.len, obj, c)) {
				return -1;
			}
			m->uninterpreted_option.len++;
			c->pb_hdr.prev = prev;
			prev = c;
		} while (!pb_cmp_tag_2(p, e, 16058));

		m->uninterpreted_option.v = (const struct UninterpretedOption**) pb_calloc(obj, m->uninterpreted_option.len * sizeof(struct UninterpretedOption*));
		if (!m->uninterpreted_option.v) {
			return -1;
		}

		for (int i = m->uninterpreted_option.len - 1; i >= 0; i--) {
			m->uninterpreted_option.v[i] = prev;
			prev = (struct UninterpretedOption*) prev->pb_hdr.prev;
		}
	}
	return 0;
}
char *pb_encode_ExtensionRangeOptions(char *p, struct ExtensionRangeOptions const *m) {
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
int pb_maxsz_ExtensionRangeOptions(struct ExtensionRangeOptions const *m) {
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

char *pb_parse_FieldDescriptorProto(char *p, pb_buf_t *obj, struct FieldDescriptorProto *m) {
	(void) obj;
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
			m->options = (struct FieldOptions*) pb_calloc(obj, sizeof(struct FieldOptions));
			if (!m->options) {return pb_errret;}
			p = pb_parse_FieldOptions(p, obj, (struct FieldOptions*) m->options);
			continue;
		default:
		unknown:
			p = pb_parse_skip(p);
		}
	}
}
int pb_print_FieldDescriptorProto(pb_buf_t *a, struct FieldDescriptorProto const *m) {
	char *p = pb_appendsz(a, 98);
	if (!p) {return -1;}
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
	a->next = p;
	if (m->name.len) {
		if (pb_append(a, "\"name\":", 7)) {return -1;}
		if (pb_print_string(a, m->name)) {return -1;}
		a->next[-1] = ','; // replace trailing newline
	}
	if (m->type_name.len) {
		if (pb_append(a, "\"type_name\":", 12)) {return -1;}
		if (pb_print_string(a, m->type_name)) {return -1;}
		a->next[-1] = ','; // replace trailing newline
	}
	if (m->extendee.len) {
		if (pb_append(a, "\"extendee\":", 11)) {return -1;}
		if (pb_print_string(a, m->extendee)) {return -1;}
		a->next[-1] = ','; // replace trailing newline
	}
	if (m->default_value.len) {
		if (pb_append(a, "\"default_value\":", 16)) {return -1;}
		if (pb_print_string(a, m->default_value)) {return -1;}
		a->next[-1] = ','; // replace trailing newline
	}
	if (m->json_name.len) {
		if (pb_append(a, "\"json_name\":", 12)) {return -1;}
		if (pb_print_string(a, m->json_name)) {return -1;}
		a->next[-1] = ','; // replace trailing newline
	}
	if (m->options) {
		if (pb_append(a, "\"options\":", 10)) {return -1;}
		if (pb_print_FieldOptions(a, m->options)) {return -1;}
		a->next[-1] = ','; // replace trailing newline
	}
	return pb_print_map_end(a);
}
int pb_get_FieldDescriptorProto(const char *p, const char *e, pb_buf_t *obj, struct FieldDescriptorProto *m) {
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
		m->options = (struct FieldOptions*) pb_calloc(obj, sizeof(struct FieldOptions));
		if (!m->options || pb_get_FieldOptions(msg.buf, msg.buf + msg.len, obj, (struct FieldOptions*) m->options)) {
			return -1;
		}
	}
	if (pb_skipto_1(&p, e, 72)) {
		m->oneof_index_set = true;
		p = pb_get_u32(p + 1, e, (uint32_t*)&m->oneof_index);
	}
	if (pb_skipto_1(&p, e, 82)) {
		p = pb_get_string(p + 1, e, (struct pb_string*)&m->json_name);
	}
	return 0;
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

char *pb_parse_OneofDescriptorProto(char *p, pb_buf_t *obj, struct OneofDescriptorProto *m) {
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
			if (pb_cmp(key, "name")) {
				goto unknown;
			}
			p = pb_parse_string(p, &m->name);
			continue;
		case 1:
			if (pb_cmp(key, "options")) {
				goto unknown;
			}
			m->options = (struct OneofOptions*) pb_calloc(obj, sizeof(struct OneofOptions));
			if (!m->options) {return pb_errret;}
			p = pb_parse_OneofOptions(p, obj, (struct OneofOptions*) m->options);
			continue;
		default:
		unknown:
			p = pb_parse_skip(p);
		}
	}
}
int pb_print_OneofDescriptorProto(pb_buf_t *a, struct OneofDescriptorProto const *m) {
	char *p = pb_appendsz(a, 1);
	if (!p) {return -1;}
	*p++ = '{';
	a->next = p;
	if (m->name.len) {
		if (pb_append(a, "\"name\":", 7)) {return -1;}
		if (pb_print_string(a, m->name)) {return -1;}
		a->next[-1] = ','; // replace trailing newline
	}
	if (m->options) {
		if (pb_append(a, "\"options\":", 10)) {return -1;}
		if (pb_print_OneofOptions(a, m->options)) {return -1;}
		a->next[-1] = ','; // replace trailing newline
	}
	return pb_print_map_end(a);
}
int pb_get_OneofDescriptorProto(const char *p, const char *e, pb_buf_t *obj, struct OneofDescriptorProto *m) {
	(void) obj;
	if (pb_skipto_1(&p, e, 10)) {
		p = pb_get_string(p + 1, e, (struct pb_string*)&m->name);
	}
	if (pb_skipto_1(&p, e, 18)) {
		struct pb_string msg;
		p = pb_get_string(p + 1, e, &msg);
		m->options = (struct OneofOptions*) pb_calloc(obj, sizeof(struct OneofOptions));
		if (!m->options || pb_get_OneofOptions(msg.buf, msg.buf + msg.len, obj, (struct OneofOptions*) m->options)) {
			return -1;
		}
	}
	return 0;
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

char *pb_parse_EnumDescriptorProto(char *p, pb_buf_t *obj, struct EnumDescriptorProto *m) {
	(void) obj;
	(void) m;
	if (!pb_parse_map(&p)) {
		return p;
	}
	for (;;) {
		struct pb_string key;
		switch (pb_parse_field(&p, &key, 9841) % 15) {
		case 0:
			return p;
		case 6:
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
					struct EnumValueDescriptorProto *c = (struct EnumValueDescriptorProto*) pb_calloc(obj, sizeof(struct EnumValueDescriptorProto));
					if (!c) {return pb_errret;}
					p = pb_parse_EnumValueDescriptorProto(p, obj, c);
					m->value.len++;
					c->pb_hdr.prev = prev;
					prev = c;
				} while (pb_more_array(&p));

				m->value.v = (const struct EnumValueDescriptorProto**) pb_calloc(obj, m->value.len * sizeof(struct EnumValueDescriptorProto*));
				if (!m->value.v) {return pb_errret;}

				for (int i = m->value.len - 1; i >= 0; i--) {
					m->value.v[i] = prev;
					prev = (struct EnumValueDescriptorProto*) prev->pb_hdr.prev;
				}
			}
			continue;
		case 5:
			if (pb_cmp(key, "options")) {
				goto unknown;
			}
			m->options = (struct EnumOptions*) pb_calloc(obj, sizeof(struct EnumOptions));
			if (!m->options) {return pb_errret;}
			p = pb_parse_EnumOptions(p, obj, (struct EnumOptions*) m->options);
			continue;
		case 9:
			if (pb_cmp(key, "reserved_range")) {
				goto unknown;
			}
			if (pb_parse_array(&p)) {
				int n = 0;
				m->reserved_range.v = (struct EnumDescriptorProto_EnumReservedRange*) obj->next;
				do {
					if (obj->next + n * sizeof(struct EnumDescriptorProto_EnumReservedRange) > obj->end) {return pb_errret;}
					p = pb_parse_EnumDescriptorProto_EnumReservedRange(p, (struct EnumDescriptorProto_EnumReservedRange*) &m->reserved_range.v[n++]); 
				} while (pb_more_array(&p));
				obj->next += n * sizeof(struct EnumDescriptorProto_EnumReservedRange);
				m->reserved_range.len = n;
			}
			continue;
		case 13:
			if (pb_cmp(key, "reserved_name")) {
				goto unknown;
			}
			p = pb_parse_array_string(p, obj, &m->reserved_name.v, &m->reserved_name.len);
			continue;
		default:
		unknown:
			p = pb_parse_skip(p);
		}
	}
}
int pb_print_EnumDescriptorProto(pb_buf_t *a, struct EnumDescriptorProto const *m) {
	char *p = pb_appendsz(a, 1);
	if (!p) {return -1;}
	*p++ = '{';
	a->next = p;
	if (m->name.len) {
		if (pb_append(a, "\"name\":", 7)) {return -1;}
		if (pb_print_string(a, m->name)) {return -1;}
		a->next[-1] = ','; // replace trailing newline
	}
	if (m->value.len) {
		if (pb_append(a, "\"value\":", 8)) {return -1;}
		if (pb_append(a, "[", 1)) {return -1;}
		for (int i = 0; i < m->value.len; i++) {
			if (pb_print_EnumValueDescriptorProto(a, m->value.v[i])) {return -1;}
			a->next[-1] = ','; // replace trailing newline
		}
		if (pb_print_array_end(a)) {return -1;}
	}
	if (m->options) {
		if (pb_append(a, "\"options\":", 10)) {return -1;}
		if (pb_print_EnumOptions(a, m->options)) {return -1;}
		a->next[-1] = ','; // replace trailing newline
	}
	if (m->reserved_range.len) {
		if (pb_append(a, "\"reserved_range\":", 17)) {return -1;}
		if (pb_append(a, "[", 1)) {return -1;}
		for (int i = 0; i < m->reserved_range.len; i++) {
			if (pb_print_EnumDescriptorProto_EnumReservedRange(a, &m->reserved_range.v[i])) {return -1;}
			a->next[-1] = ','; // replace trailing newline
		}
		if (pb_print_array_end(a)) {return -1;}
	}
	if (m->reserved_name.len) {
		if (pb_append(a, "\"reserved_name\":", 16)) {return -1;}
		if (pb_append(a, "[", 1)) {return -1;}
		for (int i = 0; i < m->reserved_name.len; i++) {
			if (pb_print_string(a, m->reserved_name.v[i])) {return -1;}
			a->next[-1] = ','; // replace trailing newline
		}
		if (pb_print_array_end(a)) {return -1;}
	}
	return pb_print_map_end(a);
}
int pb_get_EnumDescriptorProto(const char *p, const char *e, pb_buf_t *obj, struct EnumDescriptorProto *m) {
	(void) obj;
	if (pb_skipto_1(&p, e, 10)) {
		p = pb_get_string(p + 1, e, (struct pb_string*)&m->name);
	}
	if (pb_skipto_1(&p, e, 18)) {
		struct EnumValueDescriptorProto *prev = NULL;
		do {
			struct pb_string msg;
			p = pb_get_string(p + 1, e, &msg);
			struct EnumValueDescriptorProto *c = (struct EnumValueDescriptorProto*) pb_calloc(obj, sizeof(struct EnumValueDescriptorProto));
			if (!c || pb_get_EnumValueDescriptorProto(msg.buf, msg.buf + msg.len, obj, c)) {
				return -1;
			}
			m->value.len++;
			c->pb_hdr.prev = prev;
			prev = c;
		} while (!pb_cmp_tag_1(p, e, 18));

		m->value.v = (const struct EnumValueDescriptorProto**) pb_calloc(obj, m->value.len * sizeof(struct EnumValueDescriptorProto*));
		if (!m->value.v) {
			return -1;
		}

		for (int i = m->value.len - 1; i >= 0; i--) {
			m->value.v[i] = prev;
			prev = (struct EnumValueDescriptorProto*) prev->pb_hdr.prev;
		}
	}
	if (pb_skipto_1(&p, e, 26)) {
		struct pb_string msg;
		p = pb_get_string(p + 1, e, &msg);
		m->options = (struct EnumOptions*) pb_calloc(obj, sizeof(struct EnumOptions));
		if (!m->options || pb_get_EnumOptions(msg.buf, msg.buf + msg.len, obj, (struct EnumOptions*) m->options)) {
			return -1;
		}
	}
	if (pb_skipto_1(&p, e, 34)) {
		m->reserved_range.v = (struct EnumDescriptorProto_EnumReservedRange*) obj->next;
		do {
			if (obj->next + m->reserved_range.len * sizeof(struct EnumDescriptorProto_EnumReservedRange) > obj->end) {
				return -1;
			}
			struct pb_string msg;
			p = pb_get_string(p + 1, e, &msg);
			if (pb_get_EnumDescriptorProto_EnumReservedRange(msg.buf, msg.buf + msg.len, (struct EnumDescriptorProto_EnumReservedRange*) &m->reserved_range.v[m->reserved_range.len])) {
				return -1;
			}
			m->reserved_range.len++;
		} while (!pb_cmp_tag_1(p, e, 34));

		obj->next += m->reserved_range.len * sizeof(struct EnumDescriptorProto_EnumReservedRange);
	}
	if (pb_skipto_1(&p, e, 42)) {
		m->reserved_name.v = (struct pb_string*) obj->next;
		do {
			if (obj->next + m->reserved_name.len * sizeof(struct pb_string) > obj->end) {
				return -1;
			}
			p = pb_get_string(p + 1, e, (struct pb_string*)&m->reserved_name.v[m->reserved_name.len]);
			m->reserved_name.len++;
		} while (!pb_cmp_tag_1(p, e, 42));

		obj->next += m->reserved_name.len * sizeof(struct pb_string);
	}
	return 0;
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
	if (m->reserved_range.len) {
		for (int i = 0; i < m->reserved_range.len; i++) {
			p = pb_put_tag_1(p, 34);
			char *start = p+1;
			p = pb_encode_EnumDescriptorProto_EnumReservedRange(start, &m->reserved_range.v[i]);
			start[-1] = (char) (p - start);
		}
	}
	if (m->reserved_name.len) {
		for (int i = 0; i < m->reserved_name.len; i++) {
			p = pb_put_tag_1(p, 42);
			p = pb_put_string(p, *(struct pb_string*)& m->reserved_name.v[i]);
		}
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

bool pb_nonzero_EnumDescriptorProto_EnumReservedRange(struct EnumDescriptorProto_EnumReservedRange const *m) {
	return false
	    || m->start
	    || m->end;
}
char *pb_parse_EnumDescriptorProto_EnumReservedRange(char *p, struct EnumDescriptorProto_EnumReservedRange *m) {
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
int pb_print_EnumDescriptorProto_EnumReservedRange(pb_buf_t *a, struct EnumDescriptorProto_EnumReservedRange const *m) {
	char *p = pb_appendsz(a, 39);
	if (!p) {return -1;}
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
	a->next = p;
	return pb_print_map_end(a);
}
int pb_get_EnumDescriptorProto_EnumReservedRange(const char *p, const char *e, struct EnumDescriptorProto_EnumReservedRange *m) {
	if (pb_skipto_1(&p, e, 8)) {
		p = pb_get_u32(p + 1, e, (uint32_t*)&m->start);
	}
	if (pb_skipto_1(&p, e, 16)) {
		p = pb_get_u32(p + 1, e, (uint32_t*)&m->end);
	}
	return 0;
}
char *pb_encode_EnumDescriptorProto_EnumReservedRange(char *p, struct EnumDescriptorProto_EnumReservedRange const *m) {
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
int pb_maxsz_EnumDescriptorProto_EnumReservedRange(struct EnumDescriptorProto_EnumReservedRange const *m) {
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

char *pb_parse_EnumValueDescriptorProto(char *p, pb_buf_t *obj, struct EnumValueDescriptorProto *m) {
	(void) obj;
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
			m->options = (struct EnumValueOptions*) pb_calloc(obj, sizeof(struct EnumValueOptions));
			if (!m->options) {return pb_errret;}
			p = pb_parse_EnumValueOptions(p, obj, (struct EnumValueOptions*) m->options);
			continue;
		default:
		unknown:
			p = pb_parse_skip(p);
		}
	}
}
int pb_print_EnumValueDescriptorProto(pb_buf_t *a, struct EnumValueDescriptorProto const *m) {
	char *p = pb_appendsz(a, 22);
	if (!p) {return -1;}
	*p++ = '{';
	if (m->number) {
		memcpy(p, "\"number\":", 9);
		p += 9;
		p = pb_print_i32(p, m->number);
	}
	a->next = p;
	if (m->name.len) {
		if (pb_append(a, "\"name\":", 7)) {return -1;}
		if (pb_print_string(a, m->name)) {return -1;}
		a->next[-1] = ','; // replace trailing newline
	}
	if (m->options) {
		if (pb_append(a, "\"options\":", 10)) {return -1;}
		if (pb_print_EnumValueOptions(a, m->options)) {return -1;}
		a->next[-1] = ','; // replace trailing newline
	}
	return pb_print_map_end(a);
}
int pb_get_EnumValueDescriptorProto(const char *p, const char *e, pb_buf_t *obj, struct EnumValueDescriptorProto *m) {
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
		m->options = (struct EnumValueOptions*) pb_calloc(obj, sizeof(struct EnumValueOptions));
		if (!m->options || pb_get_EnumValueOptions(msg.buf, msg.buf + msg.len, obj, (struct EnumValueOptions*) m->options)) {
			return -1;
		}
	}
	return 0;
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

char *pb_parse_ServiceDescriptorProto(char *p, pb_buf_t *obj, struct ServiceDescriptorProto *m) {
	(void) obj;
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
					struct MethodDescriptorProto *c = (struct MethodDescriptorProto*) pb_calloc(obj, sizeof(struct MethodDescriptorProto));
					if (!c) {return pb_errret;}
					p = pb_parse_MethodDescriptorProto(p, obj, c);
					m->method.len++;
					c->pb_hdr.prev = prev;
					prev = c;
				} while (pb_more_array(&p));

				m->method.v = (const struct MethodDescriptorProto**) pb_calloc(obj, m->method.len * sizeof(struct MethodDescriptorProto*));
				if (!m->method.v) {return pb_errret;}

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
			m->options = (struct ServiceOptions*) pb_calloc(obj, sizeof(struct ServiceOptions));
			if (!m->options) {return pb_errret;}
			p = pb_parse_ServiceOptions(p, obj, (struct ServiceOptions*) m->options);
			continue;
		default:
		unknown:
			p = pb_parse_skip(p);
		}
	}
}
int pb_print_ServiceDescriptorProto(pb_buf_t *a, struct ServiceDescriptorProto const *m) {
	char *p = pb_appendsz(a, 1);
	if (!p) {return -1;}
	*p++ = '{';
	a->next = p;
	if (m->name.len) {
		if (pb_append(a, "\"name\":", 7)) {return -1;}
		if (pb_print_string(a, m->name)) {return -1;}
		a->next[-1] = ','; // replace trailing newline
	}
	if (m->method.len) {
		if (pb_append(a, "\"method\":", 9)) {return -1;}
		if (pb_append(a, "[", 1)) {return -1;}
		for (int i = 0; i < m->method.len; i++) {
			if (pb_print_MethodDescriptorProto(a, m->method.v[i])) {return -1;}
			a->next[-1] = ','; // replace trailing newline
		}
		if (pb_print_array_end(a)) {return -1;}
	}
	if (m->options) {
		if (pb_append(a, "\"options\":", 10)) {return -1;}
		if (pb_print_ServiceOptions(a, m->options)) {return -1;}
		a->next[-1] = ','; // replace trailing newline
	}
	return pb_print_map_end(a);
}
int pb_get_ServiceDescriptorProto(const char *p, const char *e, pb_buf_t *obj, struct ServiceDescriptorProto *m) {
	(void) obj;
	if (pb_skipto_1(&p, e, 10)) {
		p = pb_get_string(p + 1, e, (struct pb_string*)&m->name);
	}
	if (pb_skipto_1(&p, e, 18)) {
		struct MethodDescriptorProto *prev = NULL;
		do {
			struct pb_string msg;
			p = pb_get_string(p + 1, e, &msg);
			struct MethodDescriptorProto *c = (struct MethodDescriptorProto*) pb_calloc(obj, sizeof(struct MethodDescriptorProto));
			if (!c || pb_get_MethodDescriptorProto(msg.buf, msg.buf + msg.len, obj, c)) {
				return -1;
			}
			m->method.len++;
			c->pb_hdr.prev = prev;
			prev = c;
		} while (!pb_cmp_tag_1(p, e, 18));

		m->method.v = (const struct MethodDescriptorProto**) pb_calloc(obj, m->method.len * sizeof(struct MethodDescriptorProto*));
		if (!m->method.v) {
			return -1;
		}

		for (int i = m->method.len - 1; i >= 0; i--) {
			m->method.v[i] = prev;
			prev = (struct MethodDescriptorProto*) prev->pb_hdr.prev;
		}
	}
	if (pb_skipto_1(&p, e, 26)) {
		struct pb_string msg;
		p = pb_get_string(p + 1, e, &msg);
		m->options = (struct ServiceOptions*) pb_calloc(obj, sizeof(struct ServiceOptions));
		if (!m->options || pb_get_ServiceOptions(msg.buf, msg.buf + msg.len, obj, (struct ServiceOptions*) m->options)) {
			return -1;
		}
	}
	return 0;
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

char *pb_parse_MethodDescriptorProto(char *p, pb_buf_t *obj, struct MethodDescriptorProto *m) {
	(void) obj;
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
			m->options = (struct MethodOptions*) pb_calloc(obj, sizeof(struct MethodOptions));
			if (!m->options) {return pb_errret;}
			p = pb_parse_MethodOptions(p, obj, (struct MethodOptions*) m->options);
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
int pb_print_MethodDescriptorProto(pb_buf_t *a, struct MethodDescriptorProto const *m) {
	char *p = pb_appendsz(a, 51);
	if (!p) {return -1;}
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
	a->next = p;
	if (m->name.len) {
		if (pb_append(a, "\"name\":", 7)) {return -1;}
		if (pb_print_string(a, m->name)) {return -1;}
		a->next[-1] = ','; // replace trailing newline
	}
	if (m->input_type.len) {
		if (pb_append(a, "\"input_type\":", 13)) {return -1;}
		if (pb_print_string(a, m->input_type)) {return -1;}
		a->next[-1] = ','; // replace trailing newline
	}
	if (m->output_type.len) {
		if (pb_append(a, "\"output_type\":", 14)) {return -1;}
		if (pb_print_string(a, m->output_type)) {return -1;}
		a->next[-1] = ','; // replace trailing newline
	}
	if (m->options) {
		if (pb_append(a, "\"options\":", 10)) {return -1;}
		if (pb_print_MethodOptions(a, m->options)) {return -1;}
		a->next[-1] = ','; // replace trailing newline
	}
	return pb_print_map_end(a);
}
int pb_get_MethodDescriptorProto(const char *p, const char *e, pb_buf_t *obj, struct MethodDescriptorProto *m) {
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
		m->options = (struct MethodOptions*) pb_calloc(obj, sizeof(struct MethodOptions));
		if (!m->options || pb_get_MethodOptions(msg.buf, msg.buf + msg.len, obj, (struct MethodOptions*) m->options)) {
			return -1;
		}
	}
	if (pb_skipto_1(&p, e, 40)) {
		p = pb_get_bool(p + 1, e, &m->client_streaming);
	}
	if (pb_skipto_1(&p, e, 48)) {
		p = pb_get_bool(p + 1, e, &m->server_streaming);
	}
	return 0;
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

char *pb_parse_FileOptions(char *p, pb_buf_t *obj, struct FileOptions *m) {
	(void) obj;
	(void) m;
	if (!pb_parse_map(&p)) {
		return p;
	}
	for (;;) {
		struct pb_string key;
		switch (pb_parse_field(&p, &key, 2590800657) % 77) {
		case 0:
			return p;
		case 50:
			if (pb_cmp(key, "java_package")) {
				goto unknown;
			}
			p = pb_parse_string(p, &m->java_package);
			continue;
		case 75:
			if (pb_cmp(key, "java_outer_classname")) {
				goto unknown;
			}
			p = pb_parse_string(p, &m->java_outer_classname);
			continue;
		case 9:
			if (pb_cmp(key, "java_multiple_files")) {
				goto unknown;
			}
			p = pb_parse_bool(p, &m->java_multiple_files);
			continue;
		case 33:
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
		case 54:
			if (pb_cmp(key, "optimize_for")) {
				goto unknown;
			}
			p = pb_parse_FileOptions_OptimizeMode(p, &m->optimize_for);
			continue;
		case 41:
			if (pb_cmp(key, "go_package")) {
				goto unknown;
			}
			p = pb_parse_string(p, &m->go_package);
			continue;
		case 69:
			if (pb_cmp(key, "cc_generic_services")) {
				goto unknown;
			}
			p = pb_parse_bool(p, &m->cc_generic_services);
			continue;
		case 19:
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
		case 68:
			if (pb_cmp(key, "php_generic_services")) {
				goto unknown;
			}
			p = pb_parse_bool(p, &m->php_generic_services);
			continue;
		case 46:
			if (pb_cmp(key, "deprecated")) {
				goto unknown;
			}
			p = pb_parse_bool(p, &m->deprecated);
			continue;
		case 5:
			if (pb_cmp(key, "cc_enable_arenas")) {
				goto unknown;
			}
			p = pb_parse_bool(p, &m->cc_enable_arenas);
			continue;
		case 55:
			if (pb_cmp(key, "objc_class_prefix")) {
				goto unknown;
			}
			p = pb_parse_string(p, &m->objc_class_prefix);
			continue;
		case 10:
			if (pb_cmp(key, "csharp_namespace")) {
				goto unknown;
			}
			p = pb_parse_string(p, &m->csharp_namespace);
			continue;
		case 64:
			if (pb_cmp(key, "swift_prefix")) {
				goto unknown;
			}
			p = pb_parse_string(p, &m->swift_prefix);
			continue;
		case 34:
			if (pb_cmp(key, "php_class_prefix")) {
				goto unknown;
			}
			p = pb_parse_string(p, &m->php_class_prefix);
			continue;
		case 43:
			if (pb_cmp(key, "php_namespace")) {
				goto unknown;
			}
			p = pb_parse_string(p, &m->php_namespace);
			continue;
		case 71:
			if (pb_cmp(key, "uninterpreted_option")) {
				goto unknown;
			}
			if (pb_parse_array(&p)) {
				struct UninterpretedOption *prev = NULL;
				do {
					struct UninterpretedOption *c = (struct UninterpretedOption*) pb_calloc(obj, sizeof(struct UninterpretedOption));
					if (!c) {return pb_errret;}
					p = pb_parse_UninterpretedOption(p, obj, c);
					m->uninterpreted_option.len++;
					c->pb_hdr.prev = prev;
					prev = c;
				} while (pb_more_array(&p));

				m->uninterpreted_option.v = (const struct UninterpretedOption**) pb_calloc(obj, m->uninterpreted_option.len * sizeof(struct UninterpretedOption*));
				if (!m->uninterpreted_option.v) {return pb_errret;}

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
int pb_print_FileOptions(pb_buf_t *a, struct FileOptions const *m) {
	char *p = pb_appendsz(a, 288);
	if (!p) {return -1;}
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
	if (m->php_generic_services) {
		memcpy(p, "\"php_generic_services\":", 23);
		p += 23;
		p = pb_print_bool(p, m->php_generic_services);
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
	a->next = p;
	if (m->java_package.len) {
		if (pb_append(a, "\"java_package\":", 15)) {return -1;}
		if (pb_print_string(a, m->java_package)) {return -1;}
		a->next[-1] = ','; // replace trailing newline
	}
	if (m->java_outer_classname.len) {
		if (pb_append(a, "\"java_outer_classname\":", 23)) {return -1;}
		if (pb_print_string(a, m->java_outer_classname)) {return -1;}
		a->next[-1] = ','; // replace trailing newline
	}
	if (m->go_package.len) {
		if (pb_append(a, "\"go_package\":", 13)) {return -1;}
		if (pb_print_string(a, m->go_package)) {return -1;}
		a->next[-1] = ','; // replace trailing newline
	}
	if (m->objc_class_prefix.len) {
		if (pb_append(a, "\"objc_class_prefix\":", 20)) {return -1;}
		if (pb_print_string(a, m->objc_class_prefix)) {return -1;}
		a->next[-1] = ','; // replace trailing newline
	}
	if (m->csharp_namespace.len) {
		if (pb_append(a, "\"csharp_namespace\":", 19)) {return -1;}
		if (pb_print_string(a, m->csharp_namespace)) {return -1;}
		a->next[-1] = ','; // replace trailing newline
	}
	if (m->swift_prefix.len) {
		if (pb_append(a, "\"swift_prefix\":", 15)) {return -1;}
		if (pb_print_string(a, m->swift_prefix)) {return -1;}
		a->next[-1] = ','; // replace trailing newline
	}
	if (m->php_class_prefix.len) {
		if (pb_append(a, "\"php_class_prefix\":", 19)) {return -1;}
		if (pb_print_string(a, m->php_class_prefix)) {return -1;}
		a->next[-1] = ','; // replace trailing newline
	}
	if (m->php_namespace.len) {
		if (pb_append(a, "\"php_namespace\":", 16)) {return -1;}
		if (pb_print_string(a, m->php_namespace)) {return -1;}
		a->next[-1] = ','; // replace trailing newline
	}
	if (m->uninterpreted_option.len) {
		if (pb_append(a, "\"uninterpreted_option\":", 23)) {return -1;}
		if (pb_append(a, "[", 1)) {return -1;}
		for (int i = 0; i < m->uninterpreted_option.len; i++) {
			if (pb_print_UninterpretedOption(a, m->uninterpreted_option.v[i])) {return -1;}
			a->next[-1] = ','; // replace trailing newline
		}
		if (pb_print_array_end(a)) {return -1;}
	}
	return pb_print_map_end(a);
}
int pb_get_FileOptions(const char *p, const char *e, pb_buf_t *obj, struct FileOptions *m) {
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
	if (pb_skipto_2(&p, e, 698)) {
		p = pb_get_string(p + 2, e, (struct pb_string*)&m->swift_prefix);
	}
	if (pb_skipto_2(&p, e, 706)) {
		p = pb_get_string(p + 2, e, (struct pb_string*)&m->php_class_prefix);
	}
	if (pb_skipto_2(&p, e, 714)) {
		p = pb_get_string(p + 2, e, (struct pb_string*)&m->php_namespace);
	}
	if (pb_skipto_2(&p, e, 720)) {
		p = pb_get_bool(p + 2, e, &m->php_generic_services);
	}
	if (pb_skipto_2(&p, e, 16058)) {
		struct UninterpretedOption *prev = NULL;
		do {
			struct pb_string msg;
			p = pb_get_string(p + 2, e, &msg);
			struct UninterpretedOption *c = (struct UninterpretedOption*) pb_calloc(obj, sizeof(struct UninterpretedOption));
			if (!c || pb_get_UninterpretedOption(msg.buf, msg.buf + msg.len, obj, c)) {
				return -1;
			}
			m->uninterpreted_option.len++;
			c->pb_hdr.prev = prev;
			prev = c;
		} while (!pb_cmp_tag_2(p, e, 16058));

		m->uninterpreted_option.v = (const struct UninterpretedOption**) pb_calloc(obj, m->uninterpreted_option.len * sizeof(struct UninterpretedOption*));
		if (!m->uninterpreted_option.v) {
			return -1;
		}

		for (int i = m->uninterpreted_option.len - 1; i >= 0; i--) {
			m->uninterpreted_option.v[i] = prev;
			prev = (struct UninterpretedOption*) prev->pb_hdr.prev;
		}
	}
	return 0;
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
	if (m->swift_prefix.len) {
		p = pb_put_tag_2(p, 698);
		p = pb_put_string(p, *(struct pb_string*)& m->swift_prefix);
	}
	if (m->php_class_prefix.len) {
		p = pb_put_tag_2(p, 706);
		p = pb_put_string(p, *(struct pb_string*)& m->php_class_prefix);
	}
	if (m->php_namespace.len) {
		p = pb_put_tag_2(p, 714);
		p = pb_put_string(p, *(struct pb_string*)& m->php_namespace);
	}
	if (m->php_generic_services) {
		p = pb_put_tag_2(p, 720);
		p = pb_put_bool(p,  m->php_generic_services);
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
	if (m->swift_prefix.len) {
		n += 2 + 5 + m->swift_prefix.len;
	}
	if (m->php_class_prefix.len) {
		n += 2 + 5 + m->php_class_prefix.len;
	}
	if (m->php_namespace.len) {
		n += 2 + 5 + m->php_namespace.len;
	}
	if (m->php_generic_services) {
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

char *pb_parse_MessageOptions(char *p, pb_buf_t *obj, struct MessageOptions *m) {
	(void) obj;
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
					struct UninterpretedOption *c = (struct UninterpretedOption*) pb_calloc(obj, sizeof(struct UninterpretedOption));
					if (!c) {return pb_errret;}
					p = pb_parse_UninterpretedOption(p, obj, c);
					m->uninterpreted_option.len++;
					c->pb_hdr.prev = prev;
					prev = c;
				} while (pb_more_array(&p));

				m->uninterpreted_option.v = (const struct UninterpretedOption**) pb_calloc(obj, m->uninterpreted_option.len * sizeof(struct UninterpretedOption*));
				if (!m->uninterpreted_option.v) {return pb_errret;}

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
int pb_print_MessageOptions(pb_buf_t *a, struct MessageOptions const *m) {
	char *p = pb_appendsz(a, 110);
	if (!p) {return -1;}
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
	a->next = p;
	if (m->uninterpreted_option.len) {
		if (pb_append(a, "\"uninterpreted_option\":", 23)) {return -1;}
		if (pb_append(a, "[", 1)) {return -1;}
		for (int i = 0; i < m->uninterpreted_option.len; i++) {
			if (pb_print_UninterpretedOption(a, m->uninterpreted_option.v[i])) {return -1;}
			a->next[-1] = ','; // replace trailing newline
		}
		if (pb_print_array_end(a)) {return -1;}
	}
	return pb_print_map_end(a);
}
int pb_get_MessageOptions(const char *p, const char *e, pb_buf_t *obj, struct MessageOptions *m) {
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
			struct UninterpretedOption *c = (struct UninterpretedOption*) pb_calloc(obj, sizeof(struct UninterpretedOption));
			if (!c || pb_get_UninterpretedOption(msg.buf, msg.buf + msg.len, obj, c)) {
				return -1;
			}
			m->uninterpreted_option.len++;
			c->pb_hdr.prev = prev;
			prev = c;
		} while (!pb_cmp_tag_2(p, e, 16058));

		m->uninterpreted_option.v = (const struct UninterpretedOption**) pb_calloc(obj, m->uninterpreted_option.len * sizeof(struct UninterpretedOption*));
		if (!m->uninterpreted_option.v) {
			return -1;
		}

		for (int i = m->uninterpreted_option.len - 1; i >= 0; i--) {
			m->uninterpreted_option.v[i] = prev;
			prev = (struct UninterpretedOption*) prev->pb_hdr.prev;
		}
	}
	return 0;
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

char *pb_parse_FieldOptions(char *p, pb_buf_t *obj, struct FieldOptions *m) {
	(void) obj;
	(void) m;
	if (!pb_parse_map(&p)) {
		return p;
	}
	for (;;) {
		struct pb_string key;
		switch (pb_parse_field(&p, &key, 29524) % 18) {
		case 0:
			return p;
		case 17:
			if (pb_cmp(key, "ctype")) {
				goto unknown;
			}
			p = pb_parse_FieldOptions_CType(p, &m->ctype);
			continue;
		case 12:
			if (pb_cmp(key, "packed")) {
				goto unknown;
			}
			p = pb_parse_bool(p, &m->packed);
			continue;
		case 7:
			if (pb_cmp(key, "jstype")) {
				goto unknown;
			}
			p = pb_parse_FieldOptions_JSType(p, &m->jstype);
			continue;
		case 15:
			if (pb_cmp(key, "lazy")) {
				goto unknown;
			}
			p = pb_parse_bool(p, &m->lazy);
			continue;
		case 6:
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
		case 10:
			if (pb_cmp(key, "uninterpreted_option")) {
				goto unknown;
			}
			if (pb_parse_array(&p)) {
				struct UninterpretedOption *prev = NULL;
				do {
					struct UninterpretedOption *c = (struct UninterpretedOption*) pb_calloc(obj, sizeof(struct UninterpretedOption));
					if (!c) {return pb_errret;}
					p = pb_parse_UninterpretedOption(p, obj, c);
					m->uninterpreted_option.len++;
					c->pb_hdr.prev = prev;
					prev = c;
				} while (pb_more_array(&p));

				m->uninterpreted_option.v = (const struct UninterpretedOption**) pb_calloc(obj, m->uninterpreted_option.len * sizeof(struct UninterpretedOption*));
				if (!m->uninterpreted_option.v) {return pb_errret;}

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
int pb_print_FieldOptions(pb_buf_t *a, struct FieldOptions const *m) {
	char *p = pb_appendsz(a, 107);
	if (!p) {return -1;}
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
	a->next = p;
	if (m->uninterpreted_option.len) {
		if (pb_append(a, "\"uninterpreted_option\":", 23)) {return -1;}
		if (pb_append(a, "[", 1)) {return -1;}
		for (int i = 0; i < m->uninterpreted_option.len; i++) {
			if (pb_print_UninterpretedOption(a, m->uninterpreted_option.v[i])) {return -1;}
			a->next[-1] = ','; // replace trailing newline
		}
		if (pb_print_array_end(a)) {return -1;}
	}
	return pb_print_map_end(a);
}
int pb_get_FieldOptions(const char *p, const char *e, pb_buf_t *obj, struct FieldOptions *m) {
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
			struct UninterpretedOption *c = (struct UninterpretedOption*) pb_calloc(obj, sizeof(struct UninterpretedOption));
			if (!c || pb_get_UninterpretedOption(msg.buf, msg.buf + msg.len, obj, c)) {
				return -1;
			}
			m->uninterpreted_option.len++;
			c->pb_hdr.prev = prev;
			prev = c;
		} while (!pb_cmp_tag_2(p, e, 16058));

		m->uninterpreted_option.v = (const struct UninterpretedOption**) pb_calloc(obj, m->uninterpreted_option.len * sizeof(struct UninterpretedOption*));
		if (!m->uninterpreted_option.v) {
			return -1;
		}

		for (int i = m->uninterpreted_option.len - 1; i >= 0; i--) {
			m->uninterpreted_option.v[i] = prev;
			prev = (struct UninterpretedOption*) prev->pb_hdr.prev;
		}
	}
	return 0;
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

char *pb_parse_OneofOptions(char *p, pb_buf_t *obj, struct OneofOptions *m) {
	(void) obj;
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
					struct UninterpretedOption *c = (struct UninterpretedOption*) pb_calloc(obj, sizeof(struct UninterpretedOption));
					if (!c) {return pb_errret;}
					p = pb_parse_UninterpretedOption(p, obj, c);
					m->uninterpreted_option.len++;
					c->pb_hdr.prev = prev;
					prev = c;
				} while (pb_more_array(&p));

				m->uninterpreted_option.v = (const struct UninterpretedOption**) pb_calloc(obj, m->uninterpreted_option.len * sizeof(struct UninterpretedOption*));
				if (!m->uninterpreted_option.v) {return pb_errret;}

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
int pb_print_OneofOptions(pb_buf_t *a, struct OneofOptions const *m) {
	char *p = pb_appendsz(a, 1);
	if (!p) {return -1;}
	*p++ = '{';
	a->next = p;
	if (m->uninterpreted_option.len) {
		if (pb_append(a, "\"uninterpreted_option\":", 23)) {return -1;}
		if (pb_append(a, "[", 1)) {return -1;}
		for (int i = 0; i < m->uninterpreted_option.len; i++) {
			if (pb_print_UninterpretedOption(a, m->uninterpreted_option.v[i])) {return -1;}
			a->next[-1] = ','; // replace trailing newline
		}
		if (pb_print_array_end(a)) {return -1;}
	}
	return pb_print_map_end(a);
}
int pb_get_OneofOptions(const char *p, const char *e, pb_buf_t *obj, struct OneofOptions *m) {
	(void) obj;
	p = pb_toend_1(p,e);
	if (pb_skipto_2(&p, e, 16058)) {
		struct UninterpretedOption *prev = NULL;
		do {
			struct pb_string msg;
			p = pb_get_string(p + 2, e, &msg);
			struct UninterpretedOption *c = (struct UninterpretedOption*) pb_calloc(obj, sizeof(struct UninterpretedOption));
			if (!c || pb_get_UninterpretedOption(msg.buf, msg.buf + msg.len, obj, c)) {
				return -1;
			}
			m->uninterpreted_option.len++;
			c->pb_hdr.prev = prev;
			prev = c;
		} while (!pb_cmp_tag_2(p, e, 16058));

		m->uninterpreted_option.v = (const struct UninterpretedOption**) pb_calloc(obj, m->uninterpreted_option.len * sizeof(struct UninterpretedOption*));
		if (!m->uninterpreted_option.v) {
			return -1;
		}

		for (int i = m->uninterpreted_option.len - 1; i >= 0; i--) {
			m->uninterpreted_option.v[i] = prev;
			prev = (struct UninterpretedOption*) prev->pb_hdr.prev;
		}
	}
	return 0;
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

char *pb_parse_EnumOptions(char *p, pb_buf_t *obj, struct EnumOptions *m) {
	(void) obj;
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
					struct UninterpretedOption *c = (struct UninterpretedOption*) pb_calloc(obj, sizeof(struct UninterpretedOption));
					if (!c) {return pb_errret;}
					p = pb_parse_UninterpretedOption(p, obj, c);
					m->uninterpreted_option.len++;
					c->pb_hdr.prev = prev;
					prev = c;
				} while (pb_more_array(&p));

				m->uninterpreted_option.v = (const struct UninterpretedOption**) pb_calloc(obj, m->uninterpreted_option.len * sizeof(struct UninterpretedOption*));
				if (!m->uninterpreted_option.v) {return pb_errret;}

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
int pb_print_EnumOptions(pb_buf_t *a, struct EnumOptions const *m) {
	char *p = pb_appendsz(a, 40);
	if (!p) {return -1;}
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
	a->next = p;
	if (m->uninterpreted_option.len) {
		if (pb_append(a, "\"uninterpreted_option\":", 23)) {return -1;}
		if (pb_append(a, "[", 1)) {return -1;}
		for (int i = 0; i < m->uninterpreted_option.len; i++) {
			if (pb_print_UninterpretedOption(a, m->uninterpreted_option.v[i])) {return -1;}
			a->next[-1] = ','; // replace trailing newline
		}
		if (pb_print_array_end(a)) {return -1;}
	}
	return pb_print_map_end(a);
}
int pb_get_EnumOptions(const char *p, const char *e, pb_buf_t *obj, struct EnumOptions *m) {
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
			struct UninterpretedOption *c = (struct UninterpretedOption*) pb_calloc(obj, sizeof(struct UninterpretedOption));
			if (!c || pb_get_UninterpretedOption(msg.buf, msg.buf + msg.len, obj, c)) {
				return -1;
			}
			m->uninterpreted_option.len++;
			c->pb_hdr.prev = prev;
			prev = c;
		} while (!pb_cmp_tag_2(p, e, 16058));

		m->uninterpreted_option.v = (const struct UninterpretedOption**) pb_calloc(obj, m->uninterpreted_option.len * sizeof(struct UninterpretedOption*));
		if (!m->uninterpreted_option.v) {
			return -1;
		}

		for (int i = m->uninterpreted_option.len - 1; i >= 0; i--) {
			m->uninterpreted_option.v[i] = prev;
			prev = (struct UninterpretedOption*) prev->pb_hdr.prev;
		}
	}
	return 0;
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

char *pb_parse_EnumValueOptions(char *p, pb_buf_t *obj, struct EnumValueOptions *m) {
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
					struct UninterpretedOption *c = (struct UninterpretedOption*) pb_calloc(obj, sizeof(struct UninterpretedOption));
					if (!c) {return pb_errret;}
					p = pb_parse_UninterpretedOption(p, obj, c);
					m->uninterpreted_option.len++;
					c->pb_hdr.prev = prev;
					prev = c;
				} while (pb_more_array(&p));

				m->uninterpreted_option.v = (const struct UninterpretedOption**) pb_calloc(obj, m->uninterpreted_option.len * sizeof(struct UninterpretedOption*));
				if (!m->uninterpreted_option.v) {return pb_errret;}

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
int pb_print_EnumValueOptions(pb_buf_t *a, struct EnumValueOptions const *m) {
	char *p = pb_appendsz(a, 20);
	if (!p) {return -1;}
	*p++ = '{';
	if (m->deprecated) {
		memcpy(p, "\"deprecated\":", 13);
		p += 13;
		p = pb_print_bool(p, m->deprecated);
	}
	a->next = p;
	if (m->uninterpreted_option.len) {
		if (pb_append(a, "\"uninterpreted_option\":", 23)) {return -1;}
		if (pb_append(a, "[", 1)) {return -1;}
		for (int i = 0; i < m->uninterpreted_option.len; i++) {
			if (pb_print_UninterpretedOption(a, m->uninterpreted_option.v[i])) {return -1;}
			a->next[-1] = ','; // replace trailing newline
		}
		if (pb_print_array_end(a)) {return -1;}
	}
	return pb_print_map_end(a);
}
int pb_get_EnumValueOptions(const char *p, const char *e, pb_buf_t *obj, struct EnumValueOptions *m) {
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
			struct UninterpretedOption *c = (struct UninterpretedOption*) pb_calloc(obj, sizeof(struct UninterpretedOption));
			if (!c || pb_get_UninterpretedOption(msg.buf, msg.buf + msg.len, obj, c)) {
				return -1;
			}
			m->uninterpreted_option.len++;
			c->pb_hdr.prev = prev;
			prev = c;
		} while (!pb_cmp_tag_2(p, e, 16058));

		m->uninterpreted_option.v = (const struct UninterpretedOption**) pb_calloc(obj, m->uninterpreted_option.len * sizeof(struct UninterpretedOption*));
		if (!m->uninterpreted_option.v) {
			return -1;
		}

		for (int i = m->uninterpreted_option.len - 1; i >= 0; i--) {
			m->uninterpreted_option.v[i] = prev;
			prev = (struct UninterpretedOption*) prev->pb_hdr.prev;
		}
	}
	return 0;
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

char *pb_parse_ServiceOptions(char *p, pb_buf_t *obj, struct ServiceOptions *m) {
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
					struct UninterpretedOption *c = (struct UninterpretedOption*) pb_calloc(obj, sizeof(struct UninterpretedOption));
					if (!c) {return pb_errret;}
					p = pb_parse_UninterpretedOption(p, obj, c);
					m->uninterpreted_option.len++;
					c->pb_hdr.prev = prev;
					prev = c;
				} while (pb_more_array(&p));

				m->uninterpreted_option.v = (const struct UninterpretedOption**) pb_calloc(obj, m->uninterpreted_option.len * sizeof(struct UninterpretedOption*));
				if (!m->uninterpreted_option.v) {return pb_errret;}

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
int pb_print_ServiceOptions(pb_buf_t *a, struct ServiceOptions const *m) {
	char *p = pb_appendsz(a, 20);
	if (!p) {return -1;}
	*p++ = '{';
	if (m->deprecated) {
		memcpy(p, "\"deprecated\":", 13);
		p += 13;
		p = pb_print_bool(p, m->deprecated);
	}
	a->next = p;
	if (m->uninterpreted_option.len) {
		if (pb_append(a, "\"uninterpreted_option\":", 23)) {return -1;}
		if (pb_append(a, "[", 1)) {return -1;}
		for (int i = 0; i < m->uninterpreted_option.len; i++) {
			if (pb_print_UninterpretedOption(a, m->uninterpreted_option.v[i])) {return -1;}
			a->next[-1] = ','; // replace trailing newline
		}
		if (pb_print_array_end(a)) {return -1;}
	}
	return pb_print_map_end(a);
}
int pb_get_ServiceOptions(const char *p, const char *e, pb_buf_t *obj, struct ServiceOptions *m) {
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
			struct UninterpretedOption *c = (struct UninterpretedOption*) pb_calloc(obj, sizeof(struct UninterpretedOption));
			if (!c || pb_get_UninterpretedOption(msg.buf, msg.buf + msg.len, obj, c)) {
				return -1;
			}
			m->uninterpreted_option.len++;
			c->pb_hdr.prev = prev;
			prev = c;
		} while (!pb_cmp_tag_2(p, e, 16058));

		m->uninterpreted_option.v = (const struct UninterpretedOption**) pb_calloc(obj, m->uninterpreted_option.len * sizeof(struct UninterpretedOption*));
		if (!m->uninterpreted_option.v) {
			return -1;
		}

		for (int i = m->uninterpreted_option.len - 1; i >= 0; i--) {
			m->uninterpreted_option.v[i] = prev;
			prev = (struct UninterpretedOption*) prev->pb_hdr.prev;
		}
	}
	return 0;
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

char *pb_parse_MethodOptions(char *p, pb_buf_t *obj, struct MethodOptions *m) {
	(void) obj;
	(void) m;
	if (!pb_parse_map(&p)) {
		return p;
	}
	for (;;) {
		struct pb_string key;
		switch (pb_parse_field(&p, &key, 13) % 7) {
		case 0:
			return p;
		case 1:
			if (pb_cmp(key, "deprecated")) {
				goto unknown;
			}
			p = pb_parse_bool(p, &m->deprecated);
			continue;
		case 4:
			if (pb_cmp(key, "idempotency_level")) {
				goto unknown;
			}
			p = pb_parse_MethodOptions_IdempotencyLevel(p, &m->idempotency_level);
			continue;
		case 3:
			if (pb_cmp(key, "uninterpreted_option")) {
				goto unknown;
			}
			if (pb_parse_array(&p)) {
				struct UninterpretedOption *prev = NULL;
				do {
					struct UninterpretedOption *c = (struct UninterpretedOption*) pb_calloc(obj, sizeof(struct UninterpretedOption));
					if (!c) {return pb_errret;}
					p = pb_parse_UninterpretedOption(p, obj, c);
					m->uninterpreted_option.len++;
					c->pb_hdr.prev = prev;
					prev = c;
				} while (pb_more_array(&p));

				m->uninterpreted_option.v = (const struct UninterpretedOption**) pb_calloc(obj, m->uninterpreted_option.len * sizeof(struct UninterpretedOption*));
				if (!m->uninterpreted_option.v) {return pb_errret;}

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
int pb_print_MethodOptions(pb_buf_t *a, struct MethodOptions const *m) {
	char *p = pb_appendsz(a, 63);
	if (!p) {return -1;}
	*p++ = '{';
	if (m->deprecated) {
		memcpy(p, "\"deprecated\":", 13);
		p += 13;
		p = pb_print_bool(p, m->deprecated);
	}
	if (m->idempotency_level) {
		memcpy(p, "\"idempotency_level\":", 20);
		p += 20;
		p = pb_print_MethodOptions_IdempotencyLevel(p, m->idempotency_level);
	}
	a->next = p;
	if (m->uninterpreted_option.len) {
		if (pb_append(a, "\"uninterpreted_option\":", 23)) {return -1;}
		if (pb_append(a, "[", 1)) {return -1;}
		for (int i = 0; i < m->uninterpreted_option.len; i++) {
			if (pb_print_UninterpretedOption(a, m->uninterpreted_option.v[i])) {return -1;}
			a->next[-1] = ','; // replace trailing newline
		}
		if (pb_print_array_end(a)) {return -1;}
	}
	return pb_print_map_end(a);
}
int pb_get_MethodOptions(const char *p, const char *e, pb_buf_t *obj, struct MethodOptions *m) {
	(void) obj;
	p = pb_toend_1(p,e);
	if (pb_skipto_2(&p, e, 648)) {
		p = pb_get_bool(p + 2, e, &m->deprecated);
	}
	if (pb_skipto_2(&p, e, 656)) {
		p = pb_get_u32(p + 2, e, (uint32_t*)&m->idempotency_level);
	}
	if (pb_skipto_2(&p, e, 16058)) {
		struct UninterpretedOption *prev = NULL;
		do {
			struct pb_string msg;
			p = pb_get_string(p + 2, e, &msg);
			struct UninterpretedOption *c = (struct UninterpretedOption*) pb_calloc(obj, sizeof(struct UninterpretedOption));
			if (!c || pb_get_UninterpretedOption(msg.buf, msg.buf + msg.len, obj, c)) {
				return -1;
			}
			m->uninterpreted_option.len++;
			c->pb_hdr.prev = prev;
			prev = c;
		} while (!pb_cmp_tag_2(p, e, 16058));

		m->uninterpreted_option.v = (const struct UninterpretedOption**) pb_calloc(obj, m->uninterpreted_option.len * sizeof(struct UninterpretedOption*));
		if (!m->uninterpreted_option.v) {
			return -1;
		}

		for (int i = m->uninterpreted_option.len - 1; i >= 0; i--) {
			m->uninterpreted_option.v[i] = prev;
			prev = (struct UninterpretedOption*) prev->pb_hdr.prev;
		}
	}
	return 0;
}
char *pb_encode_MethodOptions(char *p, struct MethodOptions const *m) {
	if (m->deprecated) {
		p = pb_put_tag_2(p, 648);
		p = pb_put_bool(p,  m->deprecated);
	}
	if (m->idempotency_level) {
		p = pb_put_tag_2(p, 656);
		p = pb_put_u32(p, *(uint32_t*)& m->idempotency_level);
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
int pb_maxsz_MethodOptions(struct MethodOptions const *m) {
	int n = 0;
	if (m->deprecated) {
		n += 2 + 1;
	}
	if (m->idempotency_level) {
		n += 2 + 5;
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

char *pb_print_MethodOptions_IdempotencyLevel(char *p, enum MethodOptions_IdempotencyLevel v){
	switch ((int32_t) v) {
	case 0:
		memcpy(p, "\"IDEMPOTENCY_UNKNOWN\",", 22);
		return p + 22;
	case 1:
		memcpy(p, "\"NO_SIDE_EFFECTS\",", 18);
		return p + 18;
	case 2:
		memcpy(p, "\"IDEMPOTENT\",", 13);
		return p + 13;
	default:
		memcpy(p, "\"\",", 3);
		return p + 3;
	}
}
char *pb_parse_MethodOptions_IdempotencyLevel(char *p, enum MethodOptions_IdempotencyLevel *v) {
	struct pb_string val;
	*v = (enum MethodOptions_IdempotencyLevel) 0;
	switch (pb_parse_enum(&p, &val, 1) % 5) {
	case 3:
		if (!pb_cmp(val, "IDEMPOTENCY_UNKNOWN")) {
			*v = (enum MethodOptions_IdempotencyLevel) 0;
		}
		break;
	case 2:
		if (!pb_cmp(val, "NO_SIDE_EFFECTS")) {
			*v = (enum MethodOptions_IdempotencyLevel) 1;
		}
		break;
	case 1:
		if (!pb_cmp(val, "IDEMPOTENT")) {
			*v = (enum MethodOptions_IdempotencyLevel) 2;
		}
		break;
	}
	return p;
}

char *pb_parse_UninterpretedOption(char *p, pb_buf_t *obj, struct UninterpretedOption *m) {
	(void) obj;
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
					struct UninterpretedOption_NamePart *c = (struct UninterpretedOption_NamePart*) pb_calloc(obj, sizeof(struct UninterpretedOption_NamePart));
					if (!c) {return pb_errret;}
					p = pb_parse_UninterpretedOption_NamePart(p, obj, c);
					m->name.len++;
					c->pb_hdr.prev = prev;
					prev = c;
				} while (pb_more_array(&p));

				m->name.v = (const struct UninterpretedOption_NamePart**) pb_calloc(obj, m->name.len * sizeof(struct UninterpretedOption_NamePart*));
				if (!m->name.v) {return pb_errret;}

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
int pb_print_UninterpretedOption(pb_buf_t *a, struct UninterpretedOption const *m) {
	char *p = pb_appendsz(a, 128);
	if (!p) {return -1;}
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
	a->next = p;
	if (m->name.len) {
		if (pb_append(a, "\"name\":", 7)) {return -1;}
		if (pb_append(a, "[", 1)) {return -1;}
		for (int i = 0; i < m->name.len; i++) {
			if (pb_print_UninterpretedOption_NamePart(a, m->name.v[i])) {return -1;}
			a->next[-1] = ','; // replace trailing newline
		}
		if (pb_print_array_end(a)) {return -1;}
	}
	if (m->identifier_value.len) {
		if (pb_append(a, "\"identifier_value\":", 19)) {return -1;}
		if (pb_print_string(a, m->identifier_value)) {return -1;}
		a->next[-1] = ','; // replace trailing newline
	}
	if (m->string_value.len) {
		if (pb_append(a, "\"string_value\":", 15)) {return -1;}
		if (pb_print_bytes(a, m->string_value)) {return -1;}
		a->next[-1] = ','; // replace trailing newline
	}
	if (m->aggregate_value.len) {
		if (pb_append(a, "\"aggregate_value\":", 18)) {return -1;}
		if (pb_print_string(a, m->aggregate_value)) {return -1;}
		a->next[-1] = ','; // replace trailing newline
	}
	return pb_print_map_end(a);
}
int pb_get_UninterpretedOption(const char *p, const char *e, pb_buf_t *obj, struct UninterpretedOption *m) {
	(void) obj;
	if (pb_skipto_1(&p, e, 18)) {
		struct UninterpretedOption_NamePart *prev = NULL;
		do {
			struct pb_string msg;
			p = pb_get_string(p + 1, e, &msg);
			struct UninterpretedOption_NamePart *c = (struct UninterpretedOption_NamePart*) pb_calloc(obj, sizeof(struct UninterpretedOption_NamePart));
			if (!c || pb_get_UninterpretedOption_NamePart(msg.buf, msg.buf + msg.len, obj, c)) {
				return -1;
			}
			m->name.len++;
			c->pb_hdr.prev = prev;
			prev = c;
		} while (!pb_cmp_tag_1(p, e, 18));

		m->name.v = (const struct UninterpretedOption_NamePart**) pb_calloc(obj, m->name.len * sizeof(struct UninterpretedOption_NamePart*));
		if (!m->name.v) {
			return -1;
		}

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
	return 0;
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

char *pb_parse_UninterpretedOption_NamePart(char *p, pb_buf_t *obj, struct UninterpretedOption_NamePart *m) {
	(void) obj;
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
int pb_print_UninterpretedOption_NamePart(pb_buf_t *a, struct UninterpretedOption_NamePart const *m) {
	char *p = pb_appendsz(a, 22);
	if (!p) {return -1;}
	*p++ = '{';
	if (m->is_extension) {
		memcpy(p, "\"is_extension\":", 15);
		p += 15;
		p = pb_print_bool(p, m->is_extension);
	}
	a->next = p;
	if (m->name_part.len) {
		if (pb_append(a, "\"name_part\":", 12)) {return -1;}
		if (pb_print_string(a, m->name_part)) {return -1;}
		a->next[-1] = ','; // replace trailing newline
	}
	return pb_print_map_end(a);
}
int pb_get_UninterpretedOption_NamePart(const char *p, const char *e, pb_buf_t *obj, struct UninterpretedOption_NamePart *m) {
	(void) obj;
	if (pb_skipto_1(&p, e, 10)) {
		p = pb_get_string(p + 1, e, (struct pb_string*)&m->name_part);
	}
	if (pb_skipto_1(&p, e, 16)) {
		p = pb_get_bool(p + 1, e, &m->is_extension);
	}
	return 0;
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

char *pb_parse_SourceCodeInfo(char *p, pb_buf_t *obj, struct SourceCodeInfo *m) {
	(void) obj;
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
					struct SourceCodeInfo_Location *c = (struct SourceCodeInfo_Location*) pb_calloc(obj, sizeof(struct SourceCodeInfo_Location));
					if (!c) {return pb_errret;}
					p = pb_parse_SourceCodeInfo_Location(p, obj, c);
					m->location.len++;
					c->pb_hdr.prev = prev;
					prev = c;
				} while (pb_more_array(&p));

				m->location.v = (const struct SourceCodeInfo_Location**) pb_calloc(obj, m->location.len * sizeof(struct SourceCodeInfo_Location*));
				if (!m->location.v) {return pb_errret;}

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
int pb_print_SourceCodeInfo(pb_buf_t *a, struct SourceCodeInfo const *m) {
	char *p = pb_appendsz(a, 1);
	if (!p) {return -1;}
	*p++ = '{';
	a->next = p;
	if (m->location.len) {
		if (pb_append(a, "\"location\":", 11)) {return -1;}
		if (pb_append(a, "[", 1)) {return -1;}
		for (int i = 0; i < m->location.len; i++) {
			if (pb_print_SourceCodeInfo_Location(a, m->location.v[i])) {return -1;}
			a->next[-1] = ','; // replace trailing newline
		}
		if (pb_print_array_end(a)) {return -1;}
	}
	return pb_print_map_end(a);
}
int pb_get_SourceCodeInfo(const char *p, const char *e, pb_buf_t *obj, struct SourceCodeInfo *m) {
	(void) obj;
	if (pb_skipto_1(&p, e, 10)) {
		struct SourceCodeInfo_Location *prev = NULL;
		do {
			struct pb_string msg;
			p = pb_get_string(p + 1, e, &msg);
			struct SourceCodeInfo_Location *c = (struct SourceCodeInfo_Location*) pb_calloc(obj, sizeof(struct SourceCodeInfo_Location));
			if (!c || pb_get_SourceCodeInfo_Location(msg.buf, msg.buf + msg.len, obj, c)) {
				return -1;
			}
			m->location.len++;
			c->pb_hdr.prev = prev;
			prev = c;
		} while (!pb_cmp_tag_1(p, e, 10));

		m->location.v = (const struct SourceCodeInfo_Location**) pb_calloc(obj, m->location.len * sizeof(struct SourceCodeInfo_Location*));
		if (!m->location.v) {
			return -1;
		}

		for (int i = m->location.len - 1; i >= 0; i--) {
			m->location.v[i] = prev;
			prev = (struct SourceCodeInfo_Location*) prev->pb_hdr.prev;
		}
	}
	return 0;
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

char *pb_parse_SourceCodeInfo_Location(char *p, pb_buf_t *obj, struct SourceCodeInfo_Location *m) {
	(void) obj;
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
			p = pb_parse_array_i32(p, obj, &m->path.v, &m->path.len);
			continue;
		case 10:
			if (pb_cmp(key, "span")) {
				goto unknown;
			}
			p = pb_parse_array_i32(p, obj, &m->span.v, &m->span.len);
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
			p = pb_parse_array_string(p, obj, &m->leading_detached_comments.v, &m->leading_detached_comments.len);
			continue;
		default:
		unknown:
			p = pb_parse_skip(p);
		}
	}
}
int pb_print_SourceCodeInfo_Location(pb_buf_t *a, struct SourceCodeInfo_Location const *m) {
	char *p = pb_appendsz(a, 1);
	if (!p) {return -1;}
	*p++ = '{';
	a->next = p;
	if (m->path.len) {
		if (pb_append(a, "\"path\":", 7)) {return -1;}
		char *mp = pb_appendsz(a, 3 /*[],*/ + m->path.len * 12);
		if (!mp) {return -1;}
		*mp++ = '[';
		for (int i = 0; i < m->path.len; i++) {
			mp = pb_print_i32(mp, m->path.v[i]);
		}
		mp = pb_print_array_end_i(mp);
		a->next = mp;
	}
	if (m->span.len) {
		if (pb_append(a, "\"span\":", 7)) {return -1;}
		char *mp = pb_appendsz(a, 3 /*[],*/ + m->span.len * 12);
		if (!mp) {return -1;}
		*mp++ = '[';
		for (int i = 0; i < m->span.len; i++) {
			mp = pb_print_i32(mp, m->span.v[i]);
		}
		mp = pb_print_array_end_i(mp);
		a->next = mp;
	}
	if (m->leading_comments.len) {
		if (pb_append(a, "\"leading_comments\":", 19)) {return -1;}
		if (pb_print_string(a, m->leading_comments)) {return -1;}
		a->next[-1] = ','; // replace trailing newline
	}
	if (m->trailing_comments.len) {
		if (pb_append(a, "\"trailing_comments\":", 20)) {return -1;}
		if (pb_print_string(a, m->trailing_comments)) {return -1;}
		a->next[-1] = ','; // replace trailing newline
	}
	if (m->leading_detached_comments.len) {
		if (pb_append(a, "\"leading_detached_comments\":", 28)) {return -1;}
		if (pb_append(a, "[", 1)) {return -1;}
		for (int i = 0; i < m->leading_detached_comments.len; i++) {
			if (pb_print_string(a, m->leading_detached_comments.v[i])) {return -1;}
			a->next[-1] = ','; // replace trailing newline
		}
		if (pb_print_array_end(a)) {return -1;}
	}
	return pb_print_map_end(a);
}
int pb_get_SourceCodeInfo_Location(const char *p, const char *e, pb_buf_t *obj, struct SourceCodeInfo_Location *m) {
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
		m->leading_detached_comments.v = (struct pb_string*) obj->next;
		do {
			if (obj->next + m->leading_detached_comments.len * sizeof(struct pb_string) > obj->end) {
				return -1;
			}
			p = pb_get_string(p + 1, e, (struct pb_string*)&m->leading_detached_comments.v[m->leading_detached_comments.len]);
			m->leading_detached_comments.len++;
		} while (!pb_cmp_tag_1(p, e, 50));

		obj->next += m->leading_detached_comments.len * sizeof(struct pb_string);
	}
	return 0;
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

char *pb_parse_GeneratedCodeInfo(char *p, pb_buf_t *obj, struct GeneratedCodeInfo *m) {
	(void) obj;
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
					struct GeneratedCodeInfo_Annotation *c = (struct GeneratedCodeInfo_Annotation*) pb_calloc(obj, sizeof(struct GeneratedCodeInfo_Annotation));
					if (!c) {return pb_errret;}
					p = pb_parse_GeneratedCodeInfo_Annotation(p, obj, c);
					m->annotation.len++;
					c->pb_hdr.prev = prev;
					prev = c;
				} while (pb_more_array(&p));

				m->annotation.v = (const struct GeneratedCodeInfo_Annotation**) pb_calloc(obj, m->annotation.len * sizeof(struct GeneratedCodeInfo_Annotation*));
				if (!m->annotation.v) {return pb_errret;}

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
int pb_print_GeneratedCodeInfo(pb_buf_t *a, struct GeneratedCodeInfo const *m) {
	char *p = pb_appendsz(a, 1);
	if (!p) {return -1;}
	*p++ = '{';
	a->next = p;
	if (m->annotation.len) {
		if (pb_append(a, "\"annotation\":", 13)) {return -1;}
		if (pb_append(a, "[", 1)) {return -1;}
		for (int i = 0; i < m->annotation.len; i++) {
			if (pb_print_GeneratedCodeInfo_Annotation(a, m->annotation.v[i])) {return -1;}
			a->next[-1] = ','; // replace trailing newline
		}
		if (pb_print_array_end(a)) {return -1;}
	}
	return pb_print_map_end(a);
}
int pb_get_GeneratedCodeInfo(const char *p, const char *e, pb_buf_t *obj, struct GeneratedCodeInfo *m) {
	(void) obj;
	if (pb_skipto_1(&p, e, 10)) {
		struct GeneratedCodeInfo_Annotation *prev = NULL;
		do {
			struct pb_string msg;
			p = pb_get_string(p + 1, e, &msg);
			struct GeneratedCodeInfo_Annotation *c = (struct GeneratedCodeInfo_Annotation*) pb_calloc(obj, sizeof(struct GeneratedCodeInfo_Annotation));
			if (!c || pb_get_GeneratedCodeInfo_Annotation(msg.buf, msg.buf + msg.len, obj, c)) {
				return -1;
			}
			m->annotation.len++;
			c->pb_hdr.prev = prev;
			prev = c;
		} while (!pb_cmp_tag_1(p, e, 10));

		m->annotation.v = (const struct GeneratedCodeInfo_Annotation**) pb_calloc(obj, m->annotation.len * sizeof(struct GeneratedCodeInfo_Annotation*));
		if (!m->annotation.v) {
			return -1;
		}

		for (int i = m->annotation.len - 1; i >= 0; i--) {
			m->annotation.v[i] = prev;
			prev = (struct GeneratedCodeInfo_Annotation*) prev->pb_hdr.prev;
		}
	}
	return 0;
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

char *pb_parse_GeneratedCodeInfo_Annotation(char *p, pb_buf_t *obj, struct GeneratedCodeInfo_Annotation *m) {
	(void) obj;
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
			p = pb_parse_array_i32(p, obj, &m->path.v, &m->path.len);
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
int pb_print_GeneratedCodeInfo_Annotation(pb_buf_t *a, struct GeneratedCodeInfo_Annotation const *m) {
	char *p = pb_appendsz(a, 39);
	if (!p) {return -1;}
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
	a->next = p;
	if (m->path.len) {
		if (pb_append(a, "\"path\":", 7)) {return -1;}
		char *mp = pb_appendsz(a, 3 /*[],*/ + m->path.len * 12);
		if (!mp) {return -1;}
		*mp++ = '[';
		for (int i = 0; i < m->path.len; i++) {
			mp = pb_print_i32(mp, m->path.v[i]);
		}
		mp = pb_print_array_end_i(mp);
		a->next = mp;
	}
	if (m->source_file.len) {
		if (pb_append(a, "\"source_file\":", 14)) {return -1;}
		if (pb_print_string(a, m->source_file)) {return -1;}
		a->next[-1] = ','; // replace trailing newline
	}
	return pb_print_map_end(a);
}
int pb_get_GeneratedCodeInfo_Annotation(const char *p, const char *e, pb_buf_t *obj, struct GeneratedCodeInfo_Annotation *m) {
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
	return 0;
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
