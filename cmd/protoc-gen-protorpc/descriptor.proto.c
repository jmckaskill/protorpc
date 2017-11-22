#include "protorpc/cmd/protoc-gen-protorpc/descriptor.proto.h"

int pb_get_FileDescriptorSet(const char *p, const char *e, pb_buf_t *obj, struct FileDescriptorSet *m) {
	(void) obj;
	if (pb_skipto_1(&p, e, 10)) {
		struct FileDescriptorProto *prev = NULL;
		do {
			struct pb_string msg;
			p = pb_get_string(p + 1, e, &msg);
			struct FileDescriptorProto *c = (struct FileDescriptorProto*) pb_calloc(obj, sizeof(struct FileDescriptorProto));
			if (!c || pb_get_FileDescriptorProto(msg.p, msg.p + msg.len, obj, c)) {
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
			if (!c || pb_get_DescriptorProto(msg.p, msg.p + msg.len, obj, c)) {
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
			if (!c || pb_get_EnumDescriptorProto(msg.p, msg.p + msg.len, obj, c)) {
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
			if (!c || pb_get_ServiceDescriptorProto(msg.p, msg.p + msg.len, obj, c)) {
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
			if (!c || pb_get_FieldDescriptorProto(msg.p, msg.p + msg.len, obj, c)) {
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
		if (!m->options || pb_get_FileOptions(msg.p, msg.p + msg.len, obj, (struct FileOptions*) m->options)) {
			return -1;
		}
	}
	if (pb_skipto_1(&p, e, 74)) {
		struct pb_string msg;
		p = pb_get_string(p + 1, e, &msg);
		m->source_code_info = (struct SourceCodeInfo*) pb_calloc(obj, sizeof(struct SourceCodeInfo));
		if (!m->source_code_info || pb_get_SourceCodeInfo(msg.p, msg.p + msg.len, obj, (struct SourceCodeInfo*) m->source_code_info)) {
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
			if (!c || pb_get_FieldDescriptorProto(msg.p, msg.p + msg.len, obj, c)) {
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
			if (!c || pb_get_DescriptorProto(msg.p, msg.p + msg.len, obj, c)) {
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
			if (!c || pb_get_EnumDescriptorProto(msg.p, msg.p + msg.len, obj, c)) {
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
		m->extension_range.v = (struct DescriptorProto_ExtensionRange*) obj->next;
		do {
			if (obj->next + m->extension_range.len * sizeof(struct DescriptorProto_ExtensionRange) > obj->end) {
				return -1;
			}
			struct pb_string msg;
			p = pb_get_string(p + 1, e, &msg);
			if (pb_get_DescriptorProto_ExtensionRange(msg.p, msg.p + msg.len, (struct DescriptorProto_ExtensionRange*) &m->extension_range.v[m->extension_range.len])) {
				return -1;
			}
			m->extension_range.len++;
		} while (!pb_cmp_tag_1(p, e, 42));

		obj->next += m->extension_range.len * sizeof(struct DescriptorProto_ExtensionRange);
	}
	if (pb_skipto_1(&p, e, 50)) {
		struct FieldDescriptorProto *prev = NULL;
		do {
			struct pb_string msg;
			p = pb_get_string(p + 1, e, &msg);
			struct FieldDescriptorProto *c = (struct FieldDescriptorProto*) pb_calloc(obj, sizeof(struct FieldDescriptorProto));
			if (!c || pb_get_FieldDescriptorProto(msg.p, msg.p + msg.len, obj, c)) {
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
		if (!m->options || pb_get_MessageOptions(msg.p, msg.p + msg.len, obj, (struct MessageOptions*) m->options)) {
			return -1;
		}
	}
	if (pb_skipto_1(&p, e, 66)) {
		struct OneofDescriptorProto *prev = NULL;
		do {
			struct pb_string msg;
			p = pb_get_string(p + 1, e, &msg);
			struct OneofDescriptorProto *c = (struct OneofDescriptorProto*) pb_calloc(obj, sizeof(struct OneofDescriptorProto));
			if (!c || pb_get_OneofDescriptorProto(msg.p, msg.p + msg.len, obj, c)) {
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
			if (pb_get_DescriptorProto_ReservedRange(msg.p, msg.p + msg.len, (struct DescriptorProto_ReservedRange*) &m->reserved_range.v[m->reserved_range.len])) {
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

int pb_get_DescriptorProto_ExtensionRange(const char *p, const char *e, struct DescriptorProto_ExtensionRange *m) {
	if (pb_skipto_1(&p, e, 8)) {
		p = pb_get_u32(p + 1, e, (uint32_t*)&m->start);
	}
	if (pb_skipto_1(&p, e, 16)) {
		p = pb_get_u32(p + 1, e, (uint32_t*)&m->end);
	}
	return 0;
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
		if (!m->options || pb_get_FieldOptions(msg.p, msg.p + msg.len, obj, (struct FieldOptions*) m->options)) {
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

int pb_get_OneofDescriptorProto(const char *p, const char *e, pb_buf_t *obj, struct OneofDescriptorProto *m) {
	(void) obj;
	if (pb_skipto_1(&p, e, 10)) {
		p = pb_get_string(p + 1, e, (struct pb_string*)&m->name);
	}
	if (pb_skipto_1(&p, e, 18)) {
		struct pb_string msg;
		p = pb_get_string(p + 1, e, &msg);
		m->options = (struct OneofOptions*) pb_calloc(obj, sizeof(struct OneofOptions));
		if (!m->options || pb_get_OneofOptions(msg.p, msg.p + msg.len, obj, (struct OneofOptions*) m->options)) {
			return -1;
		}
	}
	return 0;
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
			if (!c || pb_get_EnumValueDescriptorProto(msg.p, msg.p + msg.len, obj, c)) {
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
		if (!m->options || pb_get_EnumOptions(msg.p, msg.p + msg.len, obj, (struct EnumOptions*) m->options)) {
			return -1;
		}
	}
	return 0;
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
		if (!m->options || pb_get_EnumValueOptions(msg.p, msg.p + msg.len, obj, (struct EnumValueOptions*) m->options)) {
			return -1;
		}
	}
	return 0;
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
			if (!c || pb_get_MethodDescriptorProto(msg.p, msg.p + msg.len, obj, c)) {
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
		if (!m->options || pb_get_ServiceOptions(msg.p, msg.p + msg.len, obj, (struct ServiceOptions*) m->options)) {
			return -1;
		}
	}
	return 0;
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
		if (!m->options || pb_get_MethodOptions(msg.p, msg.p + msg.len, obj, (struct MethodOptions*) m->options)) {
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
	if (pb_skipto_2(&p, e, 16058)) {
		struct UninterpretedOption *prev = NULL;
		do {
			struct pb_string msg;
			p = pb_get_string(p + 2, e, &msg);
			struct UninterpretedOption *c = (struct UninterpretedOption*) pb_calloc(obj, sizeof(struct UninterpretedOption));
			if (!c || pb_get_UninterpretedOption(msg.p, msg.p + msg.len, obj, c)) {
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
			if (!c || pb_get_UninterpretedOption(msg.p, msg.p + msg.len, obj, c)) {
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
			if (!c || pb_get_UninterpretedOption(msg.p, msg.p + msg.len, obj, c)) {
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
	p = pb_toend_2(p,e);
	if (pb_skipto_3(&p, e, 1619328)) {
		p = pb_get_u32(p + 3, e, &m->array);
	}
	return 0;
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
			if (!c || pb_get_UninterpretedOption(msg.p, msg.p + msg.len, obj, c)) {
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
			if (!c || pb_get_UninterpretedOption(msg.p, msg.p + msg.len, obj, c)) {
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
			if (!c || pb_get_UninterpretedOption(msg.p, msg.p + msg.len, obj, c)) {
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
			if (!c || pb_get_UninterpretedOption(msg.p, msg.p + msg.len, obj, c)) {
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

int pb_get_MethodOptions(const char *p, const char *e, pb_buf_t *obj, struct MethodOptions *m) {
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
			if (!c || pb_get_UninterpretedOption(msg.p, msg.p + msg.len, obj, c)) {
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
	if (pb_skipto_2(&p, e, 16064)) {
		p = pb_get_bool(p + 2, e, &m->binary);
	}
	return 0;
}

int pb_get_UninterpretedOption(const char *p, const char *e, pb_buf_t *obj, struct UninterpretedOption *m) {
	(void) obj;
	if (pb_skipto_1(&p, e, 18)) {
		struct UninterpretedOption_NamePart *prev = NULL;
		do {
			struct pb_string msg;
			p = pb_get_string(p + 1, e, &msg);
			struct UninterpretedOption_NamePart *c = (struct UninterpretedOption_NamePart*) pb_calloc(obj, sizeof(struct UninterpretedOption_NamePart));
			if (!c || pb_get_UninterpretedOption_NamePart(msg.p, msg.p + msg.len, obj, c)) {
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

int pb_get_SourceCodeInfo(const char *p, const char *e, pb_buf_t *obj, struct SourceCodeInfo *m) {
	(void) obj;
	if (pb_skipto_1(&p, e, 10)) {
		struct SourceCodeInfo_Location *prev = NULL;
		do {
			struct pb_string msg;
			p = pb_get_string(p + 1, e, &msg);
			struct SourceCodeInfo_Location *c = (struct SourceCodeInfo_Location*) pb_calloc(obj, sizeof(struct SourceCodeInfo_Location));
			if (!c || pb_get_SourceCodeInfo_Location(msg.p, msg.p + msg.len, obj, c)) {
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

int pb_get_GeneratedCodeInfo(const char *p, const char *e, pb_buf_t *obj, struct GeneratedCodeInfo *m) {
	(void) obj;
	if (pb_skipto_1(&p, e, 10)) {
		struct GeneratedCodeInfo_Annotation *prev = NULL;
		do {
			struct pb_string msg;
			p = pb_get_string(p + 1, e, &msg);
			struct GeneratedCodeInfo_Annotation *c = (struct GeneratedCodeInfo_Annotation*) pb_calloc(obj, sizeof(struct GeneratedCodeInfo_Annotation));
			if (!c || pb_get_GeneratedCodeInfo_Annotation(msg.p, msg.p + msg.len, obj, c)) {
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
