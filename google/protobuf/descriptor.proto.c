#include "google/protobuf/descriptor.proto.h"

int pb_get_FileDescriptorSet(const char *p, const char *e, pb_buf_t *obj, struct FileDescriptorSet *m) {
	(void) obj;
	if (pb_skipto_1(&p, e, 10)) {
		struct FileDescriptorProto *prev = NULL;
		do {
			pb_string_t msg;
			p = pb_get_string(p + 1, e, &msg);
			struct FileDescriptorProto *c = (struct FileDescriptorProto*) pb_calloc(obj, sizeof(struct FileDescriptorProto));
			if (!c || pb_get_FileDescriptorProto(msg.c_str, msg.c_str + msg.len, obj, c)) {
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
void pb_term_FileDescriptorSet(struct FileDescriptorSet *m) {
	for (int i = 0; i < m->file.len; i++) {
		pb_term_FileDescriptorProto((struct FileDescriptorProto*) m->file.v[i]);
	}
}

int pb_get_FileDescriptorProto(const char *p, const char *e, pb_buf_t *obj, struct FileDescriptorProto *m) {
	(void) obj;
	if (pb_skipto_1(&p, e, 10)) {
		p = pb_get_string(p + 1, e, (pb_string_t*)&m->name);
	}
	if (pb_skipto_1(&p, e, 18)) {
		p = pb_get_string(p + 1, e, (pb_string_t*)&m->package);
	}
	if (pb_skipto_1(&p, e, 26)) {
		m->dependency.v = (pb_string_t*) obj->next;
		do {
			if (obj->next + m->dependency.len * sizeof(pb_string_t) > obj->end) {
				return -1;
			}
			p = pb_get_string(p + 1, e, (pb_string_t*)&m->dependency.v[m->dependency.len]);
			m->dependency.len++;
		} while (!pb_cmp_tag_1(p, e, 26));

		obj->next += m->dependency.len * sizeof(pb_string_t);
	}
	if (pb_skipto_1(&p, e, 34)) {
		struct DescriptorProto *prev = NULL;
		do {
			pb_string_t msg;
			p = pb_get_string(p + 1, e, &msg);
			struct DescriptorProto *c = (struct DescriptorProto*) pb_calloc(obj, sizeof(struct DescriptorProto));
			if (!c || pb_get_DescriptorProto(msg.c_str, msg.c_str + msg.len, obj, c)) {
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
			pb_string_t msg;
			p = pb_get_string(p + 1, e, &msg);
			struct EnumDescriptorProto *c = (struct EnumDescriptorProto*) pb_calloc(obj, sizeof(struct EnumDescriptorProto));
			if (!c || pb_get_EnumDescriptorProto(msg.c_str, msg.c_str + msg.len, obj, c)) {
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
			pb_string_t msg;
			p = pb_get_string(p + 1, e, &msg);
			struct ServiceDescriptorProto *c = (struct ServiceDescriptorProto*) pb_calloc(obj, sizeof(struct ServiceDescriptorProto));
			if (!c || pb_get_ServiceDescriptorProto(msg.c_str, msg.c_str + msg.len, obj, c)) {
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
			pb_string_t msg;
			p = pb_get_string(p + 1, e, &msg);
			struct FieldDescriptorProto *c = (struct FieldDescriptorProto*) pb_calloc(obj, sizeof(struct FieldDescriptorProto));
			if (!c || pb_get_FieldDescriptorProto(msg.c_str, msg.c_str + msg.len, obj, c)) {
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
		pb_string_t msg;
		p = pb_get_string(p + 1, e, &msg);
		m->options = (struct FileOptions*) pb_calloc(obj, sizeof(struct FileOptions));
		if (!m->options || pb_get_FileOptions(msg.c_str, msg.c_str + msg.len, obj, (struct FileOptions*) m->options)) {
			return -1;
		}
	}
	if (pb_skipto_1(&p, e, 74)) {
		pb_string_t msg;
		p = pb_get_string(p + 1, e, &msg);
		m->source_code_info = (struct SourceCodeInfo*) pb_calloc(obj, sizeof(struct SourceCodeInfo));
		if (!m->source_code_info || pb_get_SourceCodeInfo(msg.c_str, msg.c_str + msg.len, obj, (struct SourceCodeInfo*) m->source_code_info)) {
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
		p = pb_get_string(p + 1, e, (pb_string_t*)&m->syntax);
	}
	return 0;
}
void pb_term_FileDescriptorProto(struct FileDescriptorProto *m) {
	if (m->name.c_str) {
		((char*)m->name.c_str)[m->name.len] = '\0';
	} else {
		m->name.c_str = "";
	}
	if (m->package.c_str) {
		((char*)m->package.c_str)[m->package.len] = '\0';
	} else {
		m->package.c_str = "";
	}
	for (int i = 0; i < m->dependency.len; i++) {
		if (m->dependency.v[i].c_str) {
			((char*)m->dependency.v[i].c_str)[m->dependency.v[i].len] = '\0';
		} else {
			((pb_string_t*)m->dependency.v)[i].c_str = "";
		}
	}
	for (int i = 0; i < m->message_type.len; i++) {
		pb_term_DescriptorProto((struct DescriptorProto*) m->message_type.v[i]);
	}
	for (int i = 0; i < m->enum_type.len; i++) {
		pb_term_EnumDescriptorProto((struct EnumDescriptorProto*) m->enum_type.v[i]);
	}
	for (int i = 0; i < m->service.len; i++) {
		pb_term_ServiceDescriptorProto((struct ServiceDescriptorProto*) m->service.v[i]);
	}
	for (int i = 0; i < m->extension.len; i++) {
		pb_term_FieldDescriptorProto((struct FieldDescriptorProto*) m->extension.v[i]);
	}
	if (m->options) {
		pb_term_FileOptions((struct FileOptions*) m->options);
	}
	if (m->source_code_info) {
		pb_term_SourceCodeInfo((struct SourceCodeInfo*) m->source_code_info);
	}
	if (m->syntax.c_str) {
		((char*)m->syntax.c_str)[m->syntax.len] = '\0';
	} else {
		m->syntax.c_str = "";
	}
}

int pb_get_DescriptorProto(const char *p, const char *e, pb_buf_t *obj, struct DescriptorProto *m) {
	(void) obj;
	if (pb_skipto_1(&p, e, 10)) {
		p = pb_get_string(p + 1, e, (pb_string_t*)&m->name);
	}
	if (pb_skipto_1(&p, e, 18)) {
		struct FieldDescriptorProto *prev = NULL;
		do {
			pb_string_t msg;
			p = pb_get_string(p + 1, e, &msg);
			struct FieldDescriptorProto *c = (struct FieldDescriptorProto*) pb_calloc(obj, sizeof(struct FieldDescriptorProto));
			if (!c || pb_get_FieldDescriptorProto(msg.c_str, msg.c_str + msg.len, obj, c)) {
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
			pb_string_t msg;
			p = pb_get_string(p + 1, e, &msg);
			struct DescriptorProto *c = (struct DescriptorProto*) pb_calloc(obj, sizeof(struct DescriptorProto));
			if (!c || pb_get_DescriptorProto(msg.c_str, msg.c_str + msg.len, obj, c)) {
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
			pb_string_t msg;
			p = pb_get_string(p + 1, e, &msg);
			struct EnumDescriptorProto *c = (struct EnumDescriptorProto*) pb_calloc(obj, sizeof(struct EnumDescriptorProto));
			if (!c || pb_get_EnumDescriptorProto(msg.c_str, msg.c_str + msg.len, obj, c)) {
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
			pb_string_t msg;
			p = pb_get_string(p + 1, e, &msg);
			struct DescriptorProto_ExtensionRange *c = (struct DescriptorProto_ExtensionRange*) pb_calloc(obj, sizeof(struct DescriptorProto_ExtensionRange));
			if (!c || pb_get_DescriptorProto_ExtensionRange(msg.c_str, msg.c_str + msg.len, obj, c)) {
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
			pb_string_t msg;
			p = pb_get_string(p + 1, e, &msg);
			struct FieldDescriptorProto *c = (struct FieldDescriptorProto*) pb_calloc(obj, sizeof(struct FieldDescriptorProto));
			if (!c || pb_get_FieldDescriptorProto(msg.c_str, msg.c_str + msg.len, obj, c)) {
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
		pb_string_t msg;
		p = pb_get_string(p + 1, e, &msg);
		m->options = (struct MessageOptions*) pb_calloc(obj, sizeof(struct MessageOptions));
		if (!m->options || pb_get_MessageOptions(msg.c_str, msg.c_str + msg.len, obj, (struct MessageOptions*) m->options)) {
			return -1;
		}
	}
	if (pb_skipto_1(&p, e, 66)) {
		struct OneofDescriptorProto *prev = NULL;
		do {
			pb_string_t msg;
			p = pb_get_string(p + 1, e, &msg);
			struct OneofDescriptorProto *c = (struct OneofDescriptorProto*) pb_calloc(obj, sizeof(struct OneofDescriptorProto));
			if (!c || pb_get_OneofDescriptorProto(msg.c_str, msg.c_str + msg.len, obj, c)) {
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
			pb_string_t msg;
			p = pb_get_string(p + 1, e, &msg);
			if (pb_get_DescriptorProto_ReservedRange(msg.c_str, msg.c_str + msg.len, (struct DescriptorProto_ReservedRange*) &m->reserved_range.v[m->reserved_range.len])) {
				return -1;
			}
			m->reserved_range.len++;
		} while (!pb_cmp_tag_1(p, e, 74));

		obj->next += m->reserved_range.len * sizeof(struct DescriptorProto_ReservedRange);
	}
	if (pb_skipto_1(&p, e, 82)) {
		m->reserved_name.v = (pb_string_t*) obj->next;
		do {
			if (obj->next + m->reserved_name.len * sizeof(pb_string_t) > obj->end) {
				return -1;
			}
			p = pb_get_string(p + 1, e, (pb_string_t*)&m->reserved_name.v[m->reserved_name.len]);
			m->reserved_name.len++;
		} while (!pb_cmp_tag_1(p, e, 82));

		obj->next += m->reserved_name.len * sizeof(pb_string_t);
	}
	return 0;
}
void pb_term_DescriptorProto(struct DescriptorProto *m) {
	if (m->name.c_str) {
		((char*)m->name.c_str)[m->name.len] = '\0';
	} else {
		m->name.c_str = "";
	}
	for (int i = 0; i < m->field.len; i++) {
		pb_term_FieldDescriptorProto((struct FieldDescriptorProto*) m->field.v[i]);
	}
	for (int i = 0; i < m->nested_type.len; i++) {
		pb_term_DescriptorProto((struct DescriptorProto*) m->nested_type.v[i]);
	}
	for (int i = 0; i < m->enum_type.len; i++) {
		pb_term_EnumDescriptorProto((struct EnumDescriptorProto*) m->enum_type.v[i]);
	}
	for (int i = 0; i < m->extension_range.len; i++) {
		pb_term_DescriptorProto_ExtensionRange((struct DescriptorProto_ExtensionRange*) m->extension_range.v[i]);
	}
	for (int i = 0; i < m->extension.len; i++) {
		pb_term_FieldDescriptorProto((struct FieldDescriptorProto*) m->extension.v[i]);
	}
	if (m->options) {
		pb_term_MessageOptions((struct MessageOptions*) m->options);
	}
	for (int i = 0; i < m->oneof_decl.len; i++) {
		pb_term_OneofDescriptorProto((struct OneofDescriptorProto*) m->oneof_decl.v[i]);
	}
	for (int i = 0; i < m->reserved_range.len; i++) {
		pb_term_DescriptorProto_ReservedRange((struct DescriptorProto_ReservedRange*) &m->reserved_range.v[i]);
	}
	for (int i = 0; i < m->reserved_name.len; i++) {
		if (m->reserved_name.v[i].c_str) {
			((char*)m->reserved_name.v[i].c_str)[m->reserved_name.v[i].len] = '\0';
		} else {
			((pb_string_t*)m->reserved_name.v)[i].c_str = "";
		}
	}
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
		pb_string_t msg;
		p = pb_get_string(p + 1, e, &msg);
		m->options = (struct ExtensionRangeOptions*) pb_calloc(obj, sizeof(struct ExtensionRangeOptions));
		if (!m->options || pb_get_ExtensionRangeOptions(msg.c_str, msg.c_str + msg.len, obj, (struct ExtensionRangeOptions*) m->options)) {
			return -1;
		}
	}
	return 0;
}
void pb_term_DescriptorProto_ExtensionRange(struct DescriptorProto_ExtensionRange *m) {
	if (m->options) {
		pb_term_ExtensionRangeOptions((struct ExtensionRangeOptions*) m->options);
	}
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
void pb_term_DescriptorProto_ReservedRange(struct DescriptorProto_ReservedRange *m) {
}

int pb_get_ExtensionRangeOptions(const char *p, const char *e, pb_buf_t *obj, struct ExtensionRangeOptions *m) {
	(void) obj;
	p = pb_toend_1(p,e);
	if (pb_skipto_2(&p, e, 16058)) {
		struct UninterpretedOption *prev = NULL;
		do {
			pb_string_t msg;
			p = pb_get_string(p + 2, e, &msg);
			struct UninterpretedOption *c = (struct UninterpretedOption*) pb_calloc(obj, sizeof(struct UninterpretedOption));
			if (!c || pb_get_UninterpretedOption(msg.c_str, msg.c_str + msg.len, obj, c)) {
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
void pb_term_ExtensionRangeOptions(struct ExtensionRangeOptions *m) {
	for (int i = 0; i < m->uninterpreted_option.len; i++) {
		pb_term_UninterpretedOption((struct UninterpretedOption*) m->uninterpreted_option.v[i]);
	}
}

int pb_get_FieldDescriptorProto(const char *p, const char *e, pb_buf_t *obj, struct FieldDescriptorProto *m) {
	(void) obj;
	if (pb_skipto_1(&p, e, 10)) {
		p = pb_get_string(p + 1, e, (pb_string_t*)&m->name);
	}
	if (pb_skipto_1(&p, e, 18)) {
		p = pb_get_string(p + 1, e, (pb_string_t*)&m->extendee);
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
		p = pb_get_string(p + 1, e, (pb_string_t*)&m->type_name);
	}
	if (pb_skipto_1(&p, e, 58)) {
		p = pb_get_string(p + 1, e, (pb_string_t*)&m->default_value);
	}
	if (pb_skipto_1(&p, e, 66)) {
		pb_string_t msg;
		p = pb_get_string(p + 1, e, &msg);
		m->options = (struct FieldOptions*) pb_calloc(obj, sizeof(struct FieldOptions));
		if (!m->options || pb_get_FieldOptions(msg.c_str, msg.c_str + msg.len, obj, (struct FieldOptions*) m->options)) {
			return -1;
		}
	}
	if (pb_skipto_1(&p, e, 72)) {
		m->oneof_index_set = true;
		p = pb_get_u32(p + 1, e, (uint32_t*)&m->oneof_index);
	}
	if (pb_skipto_1(&p, e, 82)) {
		p = pb_get_string(p + 1, e, (pb_string_t*)&m->json_name);
	}
	return 0;
}
void pb_term_FieldDescriptorProto(struct FieldDescriptorProto *m) {
	if (m->name.c_str) {
		((char*)m->name.c_str)[m->name.len] = '\0';
	} else {
		m->name.c_str = "";
	}
	if (m->extendee.c_str) {
		((char*)m->extendee.c_str)[m->extendee.len] = '\0';
	} else {
		m->extendee.c_str = "";
	}
	if (m->type_name.c_str) {
		((char*)m->type_name.c_str)[m->type_name.len] = '\0';
	} else {
		m->type_name.c_str = "";
	}
	if (m->default_value.c_str) {
		((char*)m->default_value.c_str)[m->default_value.len] = '\0';
	} else {
		m->default_value.c_str = "";
	}
	if (m->options) {
		pb_term_FieldOptions((struct FieldOptions*) m->options);
	}
	if (m->json_name.c_str) {
		((char*)m->json_name.c_str)[m->json_name.len] = '\0';
	} else {
		m->json_name.c_str = "";
	}
}

int pb_get_OneofDescriptorProto(const char *p, const char *e, pb_buf_t *obj, struct OneofDescriptorProto *m) {
	(void) obj;
	if (pb_skipto_1(&p, e, 10)) {
		p = pb_get_string(p + 1, e, (pb_string_t*)&m->name);
	}
	if (pb_skipto_1(&p, e, 18)) {
		pb_string_t msg;
		p = pb_get_string(p + 1, e, &msg);
		m->options = (struct OneofOptions*) pb_calloc(obj, sizeof(struct OneofOptions));
		if (!m->options || pb_get_OneofOptions(msg.c_str, msg.c_str + msg.len, obj, (struct OneofOptions*) m->options)) {
			return -1;
		}
	}
	return 0;
}
void pb_term_OneofDescriptorProto(struct OneofDescriptorProto *m) {
	if (m->name.c_str) {
		((char*)m->name.c_str)[m->name.len] = '\0';
	} else {
		m->name.c_str = "";
	}
	if (m->options) {
		pb_term_OneofOptions((struct OneofOptions*) m->options);
	}
}

int pb_get_EnumDescriptorProto(const char *p, const char *e, pb_buf_t *obj, struct EnumDescriptorProto *m) {
	(void) obj;
	if (pb_skipto_1(&p, e, 10)) {
		p = pb_get_string(p + 1, e, (pb_string_t*)&m->name);
	}
	if (pb_skipto_1(&p, e, 18)) {
		struct EnumValueDescriptorProto *prev = NULL;
		do {
			pb_string_t msg;
			p = pb_get_string(p + 1, e, &msg);
			struct EnumValueDescriptorProto *c = (struct EnumValueDescriptorProto*) pb_calloc(obj, sizeof(struct EnumValueDescriptorProto));
			if (!c || pb_get_EnumValueDescriptorProto(msg.c_str, msg.c_str + msg.len, obj, c)) {
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
		pb_string_t msg;
		p = pb_get_string(p + 1, e, &msg);
		m->options = (struct EnumOptions*) pb_calloc(obj, sizeof(struct EnumOptions));
		if (!m->options || pb_get_EnumOptions(msg.c_str, msg.c_str + msg.len, obj, (struct EnumOptions*) m->options)) {
			return -1;
		}
	}
	if (pb_skipto_1(&p, e, 34)) {
		m->reserved_range.v = (struct EnumDescriptorProto_EnumReservedRange*) obj->next;
		do {
			if (obj->next + m->reserved_range.len * sizeof(struct EnumDescriptorProto_EnumReservedRange) > obj->end) {
				return -1;
			}
			pb_string_t msg;
			p = pb_get_string(p + 1, e, &msg);
			if (pb_get_EnumDescriptorProto_EnumReservedRange(msg.c_str, msg.c_str + msg.len, (struct EnumDescriptorProto_EnumReservedRange*) &m->reserved_range.v[m->reserved_range.len])) {
				return -1;
			}
			m->reserved_range.len++;
		} while (!pb_cmp_tag_1(p, e, 34));

		obj->next += m->reserved_range.len * sizeof(struct EnumDescriptorProto_EnumReservedRange);
	}
	if (pb_skipto_1(&p, e, 42)) {
		m->reserved_name.v = (pb_string_t*) obj->next;
		do {
			if (obj->next + m->reserved_name.len * sizeof(pb_string_t) > obj->end) {
				return -1;
			}
			p = pb_get_string(p + 1, e, (pb_string_t*)&m->reserved_name.v[m->reserved_name.len]);
			m->reserved_name.len++;
		} while (!pb_cmp_tag_1(p, e, 42));

		obj->next += m->reserved_name.len * sizeof(pb_string_t);
	}
	return 0;
}
void pb_term_EnumDescriptorProto(struct EnumDescriptorProto *m) {
	if (m->name.c_str) {
		((char*)m->name.c_str)[m->name.len] = '\0';
	} else {
		m->name.c_str = "";
	}
	for (int i = 0; i < m->value.len; i++) {
		pb_term_EnumValueDescriptorProto((struct EnumValueDescriptorProto*) m->value.v[i]);
	}
	if (m->options) {
		pb_term_EnumOptions((struct EnumOptions*) m->options);
	}
	for (int i = 0; i < m->reserved_range.len; i++) {
		pb_term_EnumDescriptorProto_EnumReservedRange((struct EnumDescriptorProto_EnumReservedRange*) &m->reserved_range.v[i]);
	}
	for (int i = 0; i < m->reserved_name.len; i++) {
		if (m->reserved_name.v[i].c_str) {
			((char*)m->reserved_name.v[i].c_str)[m->reserved_name.v[i].len] = '\0';
		} else {
			((pb_string_t*)m->reserved_name.v)[i].c_str = "";
		}
	}
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
void pb_term_EnumDescriptorProto_EnumReservedRange(struct EnumDescriptorProto_EnumReservedRange *m) {
}

int pb_get_EnumValueDescriptorProto(const char *p, const char *e, pb_buf_t *obj, struct EnumValueDescriptorProto *m) {
	(void) obj;
	if (pb_skipto_1(&p, e, 10)) {
		p = pb_get_string(p + 1, e, (pb_string_t*)&m->name);
	}
	if (pb_skipto_1(&p, e, 16)) {
		p = pb_get_u32(p + 1, e, (uint32_t*)&m->number);
	}
	if (pb_skipto_1(&p, e, 26)) {
		pb_string_t msg;
		p = pb_get_string(p + 1, e, &msg);
		m->options = (struct EnumValueOptions*) pb_calloc(obj, sizeof(struct EnumValueOptions));
		if (!m->options || pb_get_EnumValueOptions(msg.c_str, msg.c_str + msg.len, obj, (struct EnumValueOptions*) m->options)) {
			return -1;
		}
	}
	return 0;
}
void pb_term_EnumValueDescriptorProto(struct EnumValueDescriptorProto *m) {
	if (m->name.c_str) {
		((char*)m->name.c_str)[m->name.len] = '\0';
	} else {
		m->name.c_str = "";
	}
	if (m->options) {
		pb_term_EnumValueOptions((struct EnumValueOptions*) m->options);
	}
}

int pb_get_ServiceDescriptorProto(const char *p, const char *e, pb_buf_t *obj, struct ServiceDescriptorProto *m) {
	(void) obj;
	if (pb_skipto_1(&p, e, 10)) {
		p = pb_get_string(p + 1, e, (pb_string_t*)&m->name);
	}
	if (pb_skipto_1(&p, e, 18)) {
		struct MethodDescriptorProto *prev = NULL;
		do {
			pb_string_t msg;
			p = pb_get_string(p + 1, e, &msg);
			struct MethodDescriptorProto *c = (struct MethodDescriptorProto*) pb_calloc(obj, sizeof(struct MethodDescriptorProto));
			if (!c || pb_get_MethodDescriptorProto(msg.c_str, msg.c_str + msg.len, obj, c)) {
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
		pb_string_t msg;
		p = pb_get_string(p + 1, e, &msg);
		m->options = (struct ServiceOptions*) pb_calloc(obj, sizeof(struct ServiceOptions));
		if (!m->options || pb_get_ServiceOptions(msg.c_str, msg.c_str + msg.len, obj, (struct ServiceOptions*) m->options)) {
			return -1;
		}
	}
	return 0;
}
void pb_term_ServiceDescriptorProto(struct ServiceDescriptorProto *m) {
	if (m->name.c_str) {
		((char*)m->name.c_str)[m->name.len] = '\0';
	} else {
		m->name.c_str = "";
	}
	for (int i = 0; i < m->method.len; i++) {
		pb_term_MethodDescriptorProto((struct MethodDescriptorProto*) m->method.v[i]);
	}
	if (m->options) {
		pb_term_ServiceOptions((struct ServiceOptions*) m->options);
	}
}

int pb_get_MethodDescriptorProto(const char *p, const char *e, pb_buf_t *obj, struct MethodDescriptorProto *m) {
	(void) obj;
	if (pb_skipto_1(&p, e, 10)) {
		p = pb_get_string(p + 1, e, (pb_string_t*)&m->name);
	}
	if (pb_skipto_1(&p, e, 18)) {
		p = pb_get_string(p + 1, e, (pb_string_t*)&m->input_type);
	}
	if (pb_skipto_1(&p, e, 26)) {
		p = pb_get_string(p + 1, e, (pb_string_t*)&m->output_type);
	}
	if (pb_skipto_1(&p, e, 34)) {
		pb_string_t msg;
		p = pb_get_string(p + 1, e, &msg);
		m->options = (struct MethodOptions*) pb_calloc(obj, sizeof(struct MethodOptions));
		if (!m->options || pb_get_MethodOptions(msg.c_str, msg.c_str + msg.len, obj, (struct MethodOptions*) m->options)) {
			return -1;
		}
	}
	if (pb_skipto_1(&p, e, 40)) {
		p = pb_get_bool(p + 1, e, (bool*)&m->client_streaming);
	}
	if (pb_skipto_1(&p, e, 48)) {
		p = pb_get_bool(p + 1, e, (bool*)&m->server_streaming);
	}
	return 0;
}
void pb_term_MethodDescriptorProto(struct MethodDescriptorProto *m) {
	if (m->name.c_str) {
		((char*)m->name.c_str)[m->name.len] = '\0';
	} else {
		m->name.c_str = "";
	}
	if (m->input_type.c_str) {
		((char*)m->input_type.c_str)[m->input_type.len] = '\0';
	} else {
		m->input_type.c_str = "";
	}
	if (m->output_type.c_str) {
		((char*)m->output_type.c_str)[m->output_type.len] = '\0';
	} else {
		m->output_type.c_str = "";
	}
	if (m->options) {
		pb_term_MethodOptions((struct MethodOptions*) m->options);
	}
}

int pb_get_FileOptions(const char *p, const char *e, pb_buf_t *obj, struct FileOptions *m) {
	(void) obj;
	if (pb_skipto_1(&p, e, 10)) {
		p = pb_get_string(p + 1, e, (pb_string_t*)&m->java_package);
	}
	if (pb_skipto_1(&p, e, 66)) {
		p = pb_get_string(p + 1, e, (pb_string_t*)&m->java_outer_classname);
	}
	if (pb_skipto_1(&p, e, 72)) {
		p = pb_get_u32(p + 1, e, (uint32_t*)&m->optimize_for);
	}
	if (pb_skipto_1(&p, e, 80)) {
		p = pb_get_bool(p + 1, e, (bool*)&m->java_multiple_files);
	}
	if (pb_skipto_1(&p, e, 90)) {
		p = pb_get_string(p + 1, e, (pb_string_t*)&m->go_package);
	}
	p = pb_toend_1(p,e);
	if (pb_skipto_2(&p, e, 384)) {
		p = pb_get_bool(p + 2, e, (bool*)&m->cc_generic_services);
	}
	if (pb_skipto_2(&p, e, 392)) {
		p = pb_get_bool(p + 2, e, (bool*)&m->java_generic_services);
	}
	if (pb_skipto_2(&p, e, 400)) {
		p = pb_get_bool(p + 2, e, (bool*)&m->py_generic_services);
	}
	if (pb_skipto_2(&p, e, 416)) {
		p = pb_get_bool(p + 2, e, (bool*)&m->java_generate_equals_and_hash);
	}
	if (pb_skipto_2(&p, e, 440)) {
		p = pb_get_bool(p + 2, e, (bool*)&m->deprecated);
	}
	if (pb_skipto_2(&p, e, 472)) {
		p = pb_get_bool(p + 2, e, (bool*)&m->java_string_check_utf8);
	}
	if (pb_skipto_2(&p, e, 504)) {
		p = pb_get_bool(p + 2, e, (bool*)&m->cc_enable_arenas);
	}
	if (pb_skipto_2(&p, e, 674)) {
		p = pb_get_string(p + 2, e, (pb_string_t*)&m->objc_class_prefix);
	}
	if (pb_skipto_2(&p, e, 682)) {
		p = pb_get_string(p + 2, e, (pb_string_t*)&m->csharp_namespace);
	}
	if (pb_skipto_2(&p, e, 698)) {
		p = pb_get_string(p + 2, e, (pb_string_t*)&m->swift_prefix);
	}
	if (pb_skipto_2(&p, e, 706)) {
		p = pb_get_string(p + 2, e, (pb_string_t*)&m->php_class_prefix);
	}
	if (pb_skipto_2(&p, e, 714)) {
		p = pb_get_string(p + 2, e, (pb_string_t*)&m->php_namespace);
	}
	if (pb_skipto_2(&p, e, 720)) {
		p = pb_get_bool(p + 2, e, (bool*)&m->php_generic_services);
	}
	if (pb_skipto_2(&p, e, 16058)) {
		struct UninterpretedOption *prev = NULL;
		do {
			pb_string_t msg;
			p = pb_get_string(p + 2, e, &msg);
			struct UninterpretedOption *c = (struct UninterpretedOption*) pb_calloc(obj, sizeof(struct UninterpretedOption));
			if (!c || pb_get_UninterpretedOption(msg.c_str, msg.c_str + msg.len, obj, c)) {
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
void pb_term_FileOptions(struct FileOptions *m) {
	if (m->java_package.c_str) {
		((char*)m->java_package.c_str)[m->java_package.len] = '\0';
	} else {
		m->java_package.c_str = "";
	}
	if (m->java_outer_classname.c_str) {
		((char*)m->java_outer_classname.c_str)[m->java_outer_classname.len] = '\0';
	} else {
		m->java_outer_classname.c_str = "";
	}
	if (m->go_package.c_str) {
		((char*)m->go_package.c_str)[m->go_package.len] = '\0';
	} else {
		m->go_package.c_str = "";
	}
	if (m->objc_class_prefix.c_str) {
		((char*)m->objc_class_prefix.c_str)[m->objc_class_prefix.len] = '\0';
	} else {
		m->objc_class_prefix.c_str = "";
	}
	if (m->csharp_namespace.c_str) {
		((char*)m->csharp_namespace.c_str)[m->csharp_namespace.len] = '\0';
	} else {
		m->csharp_namespace.c_str = "";
	}
	if (m->swift_prefix.c_str) {
		((char*)m->swift_prefix.c_str)[m->swift_prefix.len] = '\0';
	} else {
		m->swift_prefix.c_str = "";
	}
	if (m->php_class_prefix.c_str) {
		((char*)m->php_class_prefix.c_str)[m->php_class_prefix.len] = '\0';
	} else {
		m->php_class_prefix.c_str = "";
	}
	if (m->php_namespace.c_str) {
		((char*)m->php_namespace.c_str)[m->php_namespace.len] = '\0';
	} else {
		m->php_namespace.c_str = "";
	}
	for (int i = 0; i < m->uninterpreted_option.len; i++) {
		pb_term_UninterpretedOption((struct UninterpretedOption*) m->uninterpreted_option.v[i]);
	}
}

int pb_get_MessageOptions(const char *p, const char *e, pb_buf_t *obj, struct MessageOptions *m) {
	(void) obj;
	if (pb_skipto_1(&p, e, 8)) {
		p = pb_get_bool(p + 1, e, (bool*)&m->message_set_wire_format);
	}
	if (pb_skipto_1(&p, e, 16)) {
		p = pb_get_bool(p + 1, e, (bool*)&m->no_standard_descriptor_accessor);
	}
	if (pb_skipto_1(&p, e, 24)) {
		p = pb_get_bool(p + 1, e, (bool*)&m->deprecated);
	}
	if (pb_skipto_1(&p, e, 56)) {
		p = pb_get_bool(p + 1, e, (bool*)&m->map_entry);
	}
	p = pb_toend_1(p,e);
	if (pb_skipto_2(&p, e, 16058)) {
		struct UninterpretedOption *prev = NULL;
		do {
			pb_string_t msg;
			p = pb_get_string(p + 2, e, &msg);
			struct UninterpretedOption *c = (struct UninterpretedOption*) pb_calloc(obj, sizeof(struct UninterpretedOption));
			if (!c || pb_get_UninterpretedOption(msg.c_str, msg.c_str + msg.len, obj, c)) {
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
void pb_term_MessageOptions(struct MessageOptions *m) {
	for (int i = 0; i < m->uninterpreted_option.len; i++) {
		pb_term_UninterpretedOption((struct UninterpretedOption*) m->uninterpreted_option.v[i]);
	}
}

int pb_get_FieldOptions(const char *p, const char *e, pb_buf_t *obj, struct FieldOptions *m) {
	(void) obj;
	if (pb_skipto_1(&p, e, 8)) {
		p = pb_get_u32(p + 1, e, (uint32_t*)&m->ctype);
	}
	if (pb_skipto_1(&p, e, 16)) {
		m->packed_set = true;
		p = pb_get_bool(p + 1, e, (bool*)&m->packed);
	}
	if (pb_skipto_1(&p, e, 24)) {
		p = pb_get_bool(p + 1, e, (bool*)&m->deprecated);
	}
	if (pb_skipto_1(&p, e, 40)) {
		p = pb_get_bool(p + 1, e, (bool*)&m->lazy);
	}
	if (pb_skipto_1(&p, e, 48)) {
		p = pb_get_u32(p + 1, e, (uint32_t*)&m->jstype);
	}
	if (pb_skipto_1(&p, e, 80)) {
		p = pb_get_bool(p + 1, e, (bool*)&m->weak);
	}
	p = pb_toend_1(p,e);
	if (pb_skipto_2(&p, e, 16058)) {
		struct UninterpretedOption *prev = NULL;
		do {
			pb_string_t msg;
			p = pb_get_string(p + 2, e, &msg);
			struct UninterpretedOption *c = (struct UninterpretedOption*) pb_calloc(obj, sizeof(struct UninterpretedOption));
			if (!c || pb_get_UninterpretedOption(msg.c_str, msg.c_str + msg.len, obj, c)) {
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
void pb_term_FieldOptions(struct FieldOptions *m) {
	for (int i = 0; i < m->uninterpreted_option.len; i++) {
		pb_term_UninterpretedOption((struct UninterpretedOption*) m->uninterpreted_option.v[i]);
	}
}

int pb_get_OneofOptions(const char *p, const char *e, pb_buf_t *obj, struct OneofOptions *m) {
	(void) obj;
	p = pb_toend_1(p,e);
	if (pb_skipto_2(&p, e, 16058)) {
		struct UninterpretedOption *prev = NULL;
		do {
			pb_string_t msg;
			p = pb_get_string(p + 2, e, &msg);
			struct UninterpretedOption *c = (struct UninterpretedOption*) pb_calloc(obj, sizeof(struct UninterpretedOption));
			if (!c || pb_get_UninterpretedOption(msg.c_str, msg.c_str + msg.len, obj, c)) {
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
void pb_term_OneofOptions(struct OneofOptions *m) {
	for (int i = 0; i < m->uninterpreted_option.len; i++) {
		pb_term_UninterpretedOption((struct UninterpretedOption*) m->uninterpreted_option.v[i]);
	}
}

int pb_get_EnumOptions(const char *p, const char *e, pb_buf_t *obj, struct EnumOptions *m) {
	(void) obj;
	if (pb_skipto_1(&p, e, 16)) {
		p = pb_get_bool(p + 1, e, (bool*)&m->allow_alias);
	}
	if (pb_skipto_1(&p, e, 24)) {
		p = pb_get_bool(p + 1, e, (bool*)&m->deprecated);
	}
	p = pb_toend_1(p,e);
	if (pb_skipto_2(&p, e, 16058)) {
		struct UninterpretedOption *prev = NULL;
		do {
			pb_string_t msg;
			p = pb_get_string(p + 2, e, &msg);
			struct UninterpretedOption *c = (struct UninterpretedOption*) pb_calloc(obj, sizeof(struct UninterpretedOption));
			if (!c || pb_get_UninterpretedOption(msg.c_str, msg.c_str + msg.len, obj, c)) {
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
void pb_term_EnumOptions(struct EnumOptions *m) {
	for (int i = 0; i < m->uninterpreted_option.len; i++) {
		pb_term_UninterpretedOption((struct UninterpretedOption*) m->uninterpreted_option.v[i]);
	}
}

int pb_get_EnumValueOptions(const char *p, const char *e, pb_buf_t *obj, struct EnumValueOptions *m) {
	(void) obj;
	if (pb_skipto_1(&p, e, 8)) {
		p = pb_get_bool(p + 1, e, (bool*)&m->deprecated);
	}
	p = pb_toend_1(p,e);
	if (pb_skipto_2(&p, e, 16058)) {
		struct UninterpretedOption *prev = NULL;
		do {
			pb_string_t msg;
			p = pb_get_string(p + 2, e, &msg);
			struct UninterpretedOption *c = (struct UninterpretedOption*) pb_calloc(obj, sizeof(struct UninterpretedOption));
			if (!c || pb_get_UninterpretedOption(msg.c_str, msg.c_str + msg.len, obj, c)) {
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
void pb_term_EnumValueOptions(struct EnumValueOptions *m) {
	for (int i = 0; i < m->uninterpreted_option.len; i++) {
		pb_term_UninterpretedOption((struct UninterpretedOption*) m->uninterpreted_option.v[i]);
	}
}

int pb_get_ServiceOptions(const char *p, const char *e, pb_buf_t *obj, struct ServiceOptions *m) {
	(void) obj;
	p = pb_toend_1(p,e);
	if (pb_skipto_2(&p, e, 648)) {
		p = pb_get_bool(p + 2, e, (bool*)&m->deprecated);
	}
	if (pb_skipto_2(&p, e, 16058)) {
		struct UninterpretedOption *prev = NULL;
		do {
			pb_string_t msg;
			p = pb_get_string(p + 2, e, &msg);
			struct UninterpretedOption *c = (struct UninterpretedOption*) pb_calloc(obj, sizeof(struct UninterpretedOption));
			if (!c || pb_get_UninterpretedOption(msg.c_str, msg.c_str + msg.len, obj, c)) {
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
void pb_term_ServiceOptions(struct ServiceOptions *m) {
	for (int i = 0; i < m->uninterpreted_option.len; i++) {
		pb_term_UninterpretedOption((struct UninterpretedOption*) m->uninterpreted_option.v[i]);
	}
}

int pb_get_MethodOptions(const char *p, const char *e, pb_buf_t *obj, struct MethodOptions *m) {
	(void) obj;
	p = pb_toend_1(p,e);
	if (pb_skipto_2(&p, e, 648)) {
		p = pb_get_bool(p + 2, e, (bool*)&m->deprecated);
	}
	if (pb_skipto_2(&p, e, 656)) {
		p = pb_get_u32(p + 2, e, (uint32_t*)&m->idempotency_level);
	}
	if (pb_skipto_2(&p, e, 16058)) {
		struct UninterpretedOption *prev = NULL;
		do {
			pb_string_t msg;
			p = pb_get_string(p + 2, e, &msg);
			struct UninterpretedOption *c = (struct UninterpretedOption*) pb_calloc(obj, sizeof(struct UninterpretedOption));
			if (!c || pb_get_UninterpretedOption(msg.c_str, msg.c_str + msg.len, obj, c)) {
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
void pb_term_MethodOptions(struct MethodOptions *m) {
	for (int i = 0; i < m->uninterpreted_option.len; i++) {
		pb_term_UninterpretedOption((struct UninterpretedOption*) m->uninterpreted_option.v[i]);
	}
}

int pb_get_UninterpretedOption(const char *p, const char *e, pb_buf_t *obj, struct UninterpretedOption *m) {
	(void) obj;
	if (pb_skipto_1(&p, e, 18)) {
		struct UninterpretedOption_NamePart *prev = NULL;
		do {
			pb_string_t msg;
			p = pb_get_string(p + 1, e, &msg);
			struct UninterpretedOption_NamePart *c = (struct UninterpretedOption_NamePart*) pb_calloc(obj, sizeof(struct UninterpretedOption_NamePart));
			if (!c || pb_get_UninterpretedOption_NamePart(msg.c_str, msg.c_str + msg.len, obj, c)) {
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
		p = pb_get_string(p + 1, e, (pb_string_t*)&m->identifier_value);
	}
	if (pb_skipto_1(&p, e, 32)) {
		p = pb_get_u64(p + 1, e, (uint64_t*)&m->positive_int_value);
	}
	if (pb_skipto_1(&p, e, 40)) {
		p = pb_get_u64(p + 1, e, (uint64_t*)&m->negative_int_value);
	}
	if (pb_skipto_1(&p, e, 53)) {
		p = pb_get_f64(p + 1, e, (union pb_f64*)&m->double_value);
	}
	if (pb_skipto_1(&p, e, 58)) {
		p = pb_get_bytes(p + 1, e, (pb_bytes_t*)&m->string_value);
	}
	if (pb_skipto_1(&p, e, 66)) {
		p = pb_get_string(p + 1, e, (pb_string_t*)&m->aggregate_value);
	}
	return 0;
}
void pb_term_UninterpretedOption(struct UninterpretedOption *m) {
	for (int i = 0; i < m->name.len; i++) {
		pb_term_UninterpretedOption_NamePart((struct UninterpretedOption_NamePart*) m->name.v[i]);
	}
	if (m->identifier_value.c_str) {
		((char*)m->identifier_value.c_str)[m->identifier_value.len] = '\0';
	} else {
		m->identifier_value.c_str = "";
	}
	if (m->aggregate_value.c_str) {
		((char*)m->aggregate_value.c_str)[m->aggregate_value.len] = '\0';
	} else {
		m->aggregate_value.c_str = "";
	}
}

int pb_get_UninterpretedOption_NamePart(const char *p, const char *e, pb_buf_t *obj, struct UninterpretedOption_NamePart *m) {
	(void) obj;
	if (pb_skipto_1(&p, e, 10)) {
		p = pb_get_string(p + 1, e, (pb_string_t*)&m->name_part);
	}
	if (pb_skipto_1(&p, e, 16)) {
		p = pb_get_bool(p + 1, e, (bool*)&m->is_extension);
	}
	return 0;
}
void pb_term_UninterpretedOption_NamePart(struct UninterpretedOption_NamePart *m) {
	if (m->name_part.c_str) {
		((char*)m->name_part.c_str)[m->name_part.len] = '\0';
	} else {
		m->name_part.c_str = "";
	}
}

int pb_get_SourceCodeInfo(const char *p, const char *e, pb_buf_t *obj, struct SourceCodeInfo *m) {
	(void) obj;
	if (pb_skipto_1(&p, e, 10)) {
		struct SourceCodeInfo_Location *prev = NULL;
		do {
			pb_string_t msg;
			p = pb_get_string(p + 1, e, &msg);
			struct SourceCodeInfo_Location *c = (struct SourceCodeInfo_Location*) pb_calloc(obj, sizeof(struct SourceCodeInfo_Location));
			if (!c || pb_get_SourceCodeInfo_Location(msg.c_str, msg.c_str + msg.len, obj, c)) {
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
void pb_term_SourceCodeInfo(struct SourceCodeInfo *m) {
	for (int i = 0; i < m->location.len; i++) {
		pb_term_SourceCodeInfo_Location((struct SourceCodeInfo_Location*) m->location.v[i]);
	}
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
		p = pb_get_string(p + 1, e, (pb_string_t*)&m->leading_comments);
	}
	if (pb_skipto_1(&p, e, 34)) {
		p = pb_get_string(p + 1, e, (pb_string_t*)&m->trailing_comments);
	}
	if (pb_skipto_1(&p, e, 50)) {
		m->leading_detached_comments.v = (pb_string_t*) obj->next;
		do {
			if (obj->next + m->leading_detached_comments.len * sizeof(pb_string_t) > obj->end) {
				return -1;
			}
			p = pb_get_string(p + 1, e, (pb_string_t*)&m->leading_detached_comments.v[m->leading_detached_comments.len]);
			m->leading_detached_comments.len++;
		} while (!pb_cmp_tag_1(p, e, 50));

		obj->next += m->leading_detached_comments.len * sizeof(pb_string_t);
	}
	return 0;
}
void pb_term_SourceCodeInfo_Location(struct SourceCodeInfo_Location *m) {
	if (m->leading_comments.c_str) {
		((char*)m->leading_comments.c_str)[m->leading_comments.len] = '\0';
	} else {
		m->leading_comments.c_str = "";
	}
	if (m->trailing_comments.c_str) {
		((char*)m->trailing_comments.c_str)[m->trailing_comments.len] = '\0';
	} else {
		m->trailing_comments.c_str = "";
	}
	for (int i = 0; i < m->leading_detached_comments.len; i++) {
		if (m->leading_detached_comments.v[i].c_str) {
			((char*)m->leading_detached_comments.v[i].c_str)[m->leading_detached_comments.v[i].len] = '\0';
		} else {
			((pb_string_t*)m->leading_detached_comments.v)[i].c_str = "";
		}
	}
}

int pb_get_GeneratedCodeInfo(const char *p, const char *e, pb_buf_t *obj, struct GeneratedCodeInfo *m) {
	(void) obj;
	if (pb_skipto_1(&p, e, 10)) {
		struct GeneratedCodeInfo_Annotation *prev = NULL;
		do {
			pb_string_t msg;
			p = pb_get_string(p + 1, e, &msg);
			struct GeneratedCodeInfo_Annotation *c = (struct GeneratedCodeInfo_Annotation*) pb_calloc(obj, sizeof(struct GeneratedCodeInfo_Annotation));
			if (!c || pb_get_GeneratedCodeInfo_Annotation(msg.c_str, msg.c_str + msg.len, obj, c)) {
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
void pb_term_GeneratedCodeInfo(struct GeneratedCodeInfo *m) {
	for (int i = 0; i < m->annotation.len; i++) {
		pb_term_GeneratedCodeInfo_Annotation((struct GeneratedCodeInfo_Annotation*) m->annotation.v[i]);
	}
}

int pb_get_GeneratedCodeInfo_Annotation(const char *p, const char *e, pb_buf_t *obj, struct GeneratedCodeInfo_Annotation *m) {
	(void) obj;
	if (pb_skipto_1(&p, e, 10)) {
		p = pb_get_packed_u32(p, e, obj, (uint32_t**)&m->path.v, &m->path.len);
	}
	if (pb_skipto_1(&p, e, 18)) {
		p = pb_get_string(p + 1, e, (pb_string_t*)&m->source_file);
	}
	if (pb_skipto_1(&p, e, 24)) {
		p = pb_get_u32(p + 1, e, (uint32_t*)&m->begin);
	}
	if (pb_skipto_1(&p, e, 32)) {
		p = pb_get_u32(p + 1, e, (uint32_t*)&m->end);
	}
	return 0;
}
void pb_term_GeneratedCodeInfo_Annotation(struct GeneratedCodeInfo_Annotation *m) {
	if (m->source_file.c_str) {
		((char*)m->source_file.c_str)[m->source_file.len] = '\0';
	} else {
		m->source_file.c_str = "";
	}
}
