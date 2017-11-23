#include "protoc-gen-protorpc.h"

static bool is_empty(const struct type *t) {
	return !strcmp(t->name.buf, ".google.protobuf.Empty");
}

void do_server(str_t *o, const struct type *t, int stage) {
	static str_t name = STR_INIT;

	switch (stage) {
	case 0:
		// declarations in the header file
		str_add(o, EOL);
		for (int i = 0; i < t->svc->method.len; i++) {
			const struct MethodDescriptorProto *m = t->svc->method.v[i];
			const struct type *in = get_input_type(m);
			const struct type *out = get_output_type(m);

			if (m->client_streaming) {
				fprintf(stderr, "WARNING: client streaming not supported\n");
				continue;
			}

			str_setstr(&name, &t->c_type);
			str_addch(&name, '_');
			str_addstr(&name, m->name);

			if (m->server_streaming) {
				str_addf(o, "struct rpc_publisher *g_rpc_%s;" EOL, name.buf);
				str_addf(o, "void publish_%s(%s const *msg);" EOL, name.buf, out->c_type.buf);
			} else {
				str_addf(o, "int cgi_%s(int sts", name.buf);
				if (!is_empty(out)) {
					str_addf(o, ", %s const *msg", out->c_type.buf);
				}
				str_add(o, ");" EOL);

				str_addf(o, "extern int rpc_%s(pb_alloc_t *obj", name.buf);
				if (!is_empty(out)) {
					str_addf(o, ", %s *out", out->c_type.buf);
				}
				if (!is_empty(in)) {
					str_addf(o, ", %s const *in", in->c_type.buf);
				}
				str_add(o, ");" EOL);
				str_addf(o, "static inline int pb_rpc_%s(pb_alloc_t *obj, str_t *resp, char *body, int bodysz);" EOL, name.buf);
			}
		}
		str_add(o, EOL);
		str_addf(o, "static inline void register_%s(struct rpc_server *s);" EOL, t->c_type.buf);
		break;

	case 1:
		// inline definitions in the header file
		for (int i = 0; i < t->svc->method.len; i++) {
			const struct MethodDescriptorProto *m = t->svc->method.v[i];
			const struct type *in = get_input_type(m);
			const struct type *out = get_output_type(m);

			str_setstr(&name, &t->c_type);
			str_addch(&name, '_');
			str_addstr(&name, m->name);

			if (!m->client_streaming && !m->server_streaming) {
				str_addf(o, "static inline int pb_rpc_%s(pb_alloc_t *obj, str_t *resp, char *body, int bodysz) {" EOL, name.buf);
				if (is_empty(in)) {
					str_add(o, "\t(void) body; (void) bodysz;" EOL);
				} else {
					str_addf(o, "\t%s in = {0};" EOL, in->c_type.buf);
					if (m->options && m->options->binary) {
						if (in->pod_message) {
							str_add(o, "(void) obj;" EOL);
							str_addf(o, "\tpb_decode_%s(body, body + bodysz, &in);" EOL, in->proto_suffix.buf);
						} else {
							str_addf(o, "\tpb_decode_%s(body, body + bodysz, obj, &in);" EOL, in->proto_suffix.buf);
						}
					} else {
						str_add(o, "\t(void) bodysz;" EOL);
						if (in->pod_message) {
							str_add(o, "\t(void) obj;" EOL);
							str_addf(o, "\tif (pb_parse_%s(body, &in) == pb_errret) {" EOL, in->json_suffix.buf);
						} else {
							str_addf(o, "\tif (pb_parse_%s(body, obj, &in) == pb_errret) {" EOL, in->json_suffix.buf);
						}
						str_add(o, "\t\treturn 400;" EOL);
						str_add(o, "\t}" EOL);
					}
				}
				if (!is_empty(out)) {
					str_addf(o, "\t%s out = {0};" EOL, out->c_type.buf);
				}
				str_addf(o, "\tint sts = rpc_%s(obj", name.buf);
				if (!is_empty(out)) {
					str_add(o, ", &out");
				}
				if (!is_empty(in)) {
					str_add(o, ", &in");
				}
				str_add(o, ");" EOL);
				if (is_empty(out)) {
					str_add(o, "\t(void) resp;" EOL);
				} else if (m->options && m->options->binary) {
					str_addf(o, "\tstr_grow(resp, resp->len + pb_maxsz_%s(&out));" EOL, out->proto_suffix.buf);
					str_addf(o, "\tstr_setend(resp, pb_encode_%s(resp->buf + resp->len, &out));" EOL, out->proto_suffix.buf);
				} else {
					str_addf(o, "\tpb_print_%s(resp, &out);" EOL, out->json_suffix.buf);
				}
				str_add(o, "\treturn sts;" EOL);
				str_add(o, "}" EOL);
			}
		}
		str_addf(o, "static inline void register_%s(struct rpc_server *s) {" EOL, t->c_type.buf);
		for (int i = 0; i < t->svc->method.len; i++) {
			const struct MethodDescriptorProto *m = t->svc->method.v[i];
			if (m->client_streaming) {
				continue;
			}

			str_setstr(&name, &t->c_type);
			str_addch(&name, '_');
			str_addstr(&name, m->name);

			// proto_type includes a leading dot which we don't want in the path
			if (m->server_streaming) {
				str_addf(o, "\tg_rpc_%s = rpc_publisher_new();" EOL, name.buf);
				str_addf(o, "\trpc_handle_stream(s, \"/api/%s/%.*s\", g_rpc_%s);" EOL, t->proto_type.buf+1, STRF(m->name), name.buf);
			} else {
				str_addf(o, "\trpc_handle_post(s, \"/api/%s/%.*s\", &pb_rpc_%s);" EOL, t->proto_type.buf+1, STRF(m->name), name.buf);
			}
		}
		str_add(o, "}" EOL);
		break;
	case 2:
		// source file
		str_add(o, EOL);

		for (int i = 0; i < t->svc->method.len; i++) {
			const struct MethodDescriptorProto *m = t->svc->method.v[i];
			const struct type *out = get_output_type(m);

			if (m->client_streaming) {
				continue;
			}

			str_setstr(&name, &t->c_type);
			str_addch(&name, '_');
			str_addstr(&name, m->name);

			if (m->server_streaming) {
				str_addf(o, "struct rpc_publisher *g_rpc_%s;" EOL, name.buf);
				str_addf(o, "void publish_%s(%s const *msg) {" EOL, name.buf, out->c_type.buf);
				str_addf(o, "\tstr_t *s = rpc_publish_start(g_rpc_%s);" EOL, name.buf);
				str_add(o, "\tif (msg) {" EOL);
				str_addf(o, "\t\tpb_print_%s(s, msg);" EOL, out->json_suffix.buf);
				str_add(o, "\t}" EOL);
				str_addf(o, "\trpc_publish_finish(g_rpc_%s);" EOL, name.buf);
				str_add(o, "}" EOL);
			} else {
				str_addf(o, "int cgi_%s(int sts", name.buf);
				if (!is_empty(out)) {
					str_addf(o, ", %s const *msg", out->c_type.buf);
				}
				str_add(o, ") {" EOL);
				str_add(o, "\tstr_t out = STR_INIT;" EOL);
				str_add(o, "\tset_binary(stdout);" EOL);
				if (!is_empty(out)) {
					str_add(o, "\tif (msg) {" EOL);
					str_addf(o, "\t\tpb_print_%s(&out, msg);" EOL, out->json_suffix.buf);
					str_add(o, "\t}" EOL);
				}
				str_add(o, "\tprintf(\"HTTP/1.1 %d \\r\\nContent-Length:%u\\r\\n\\r\\n\", sts, out.len);" EOL);
				str_add(o, "\tfwrite(out.buf, 1, out.len, stdout);" EOL);
				str_add(o, "\tfclose(stdout);" EOL);
				str_add(o, "\tstr_destroy(&out);" EOL);
				str_add(o, "\treturn sts;" EOL);
				str_add(o, "}" EOL);
			}
		}
		str_add(o, EOL);
		break;
	}
}

#if 0
void do_client(str_t *o, const type *t, bool define) {
	if (!define) {
		o << endl;
		o << "namespace " << t->svc->name << " {" << endl;
		for (int i = 0; i < t->svc->method.size(); i++) {
			const MethodDescriptorProto *m = t->svc->method[i];
			const type *in = get_input_type(m);
			const type *out = get_output_type(m);
			const char *streamtype = (m->options && m->options->binary) ? "rpc_proto_stream<" : "rpc_json_stream<";

			if (m->client_streaming || (m->server_streaming && is_empty(in))) {
				o << "inline int " << m->name << "(rpc_client *client, " << streamtype << get_relative_type(out, t);
				if (!is_empty(in)) {
					o << ", " << get_relative_type(in, t);
				}
				o << "> *stream) {" << endl;
				o << "\treturn client->connect_stream(\"/" << t->proto_type.substr(1) << "/" << m->name << "\", " << (t->proto_type.size() + m->name.size() + 1) << ", (rpc_stream_base*) stream);" << endl;
				o << "}" << endl;
			} else if (m->server_streaming) {
				o << "int " << m->name << "(rpc_client *client, " << streamtype << get_relative_type(out, t) << "> *stream";
				if (!is_empty(in)) {
					o << ", " << get_relative_type(in, t) << " const *msg";
				}
				o << ");" << endl;
			} else {
				o << "int " << m->name << "(rpc_client *client";
				if (!is_empty(out)) {
					o << ", pb_objalloc *obj, " << get_relative_type(out, t) << " *resp";
				}
				if (!is_empty(in)) {
					o << ", " << get_relative_type(in, t) << " const *req";
				}
				o << ");" << endl;
			}
		}
		o << "}" << endl;
		o << endl;
	} else {
		for (int i = 0; i < t->svc->method.size(); i++) {
			const MethodDescriptorProto *m = t->svc->method[i];
			const type *in = get_input_type(m);
			const type *out = get_output_type(m);
			const char *reqtype = (m->options && m->options->binary) ? "proto" : "json";
			std::string path = "/";
			path += t->proto_type.substr(1);
			path += "/";
			path += m->name;

			if (m->client_streaming || (m->server_streaming && is_empty(in))) {
				// inlined
			} else if (m->server_streaming) {
				o << "int " << t->cpp_type << "::" << t->svc->name << "::" << m->name << "(rpc_client *client, rpc_" << reqtype << "_stream<" << out->cpp_type << "> *stream, " << in->cpp_type << " const *msg) {" << endl;
				o << "\tint err = client->connect_stream(\"" << path << "\", " << path.size() << ", stream);" << endl;
				o << "\tif (err) {return err;}" << endl;
				o << "\treturn stream->send(msg);" << endl;
				o << "}" << endl;
			} else {
				o << "int " << t->cpp_type << "::" << t->svc->name << "::" << m->name << "(rpc_client *client";
				if (!is_empty(out)) {
					o << ", pb_objalloc *obj, " << out->cpp_type << " *resp";
				}
				if (!is_empty(in)) {
					o << ", " << in->cpp_type << " const *req";
				}
				o << ") {" << endl;
				o << "\tpb_bytes body;" << endl;
				if (m->options && m->options->binary) {
					if (is_empty(in)) {
						o << "\tuint8_t *p = client->start_proto_request(\"" << path << "\", " << path.size() << ", 0);" << endl;
					} else {
						o << "\tuint8_t *p = client->start_proto_request(\"" << path << "\", " << path.size() << ", pb_calc_maxsz(req));" << endl;
						o << "\tp = pb_encode(p, req);" << endl;
					}
					o << "\tint err = client->send_proto_request(&body, p);" << endl;
				} else {
					if (is_empty(in)) {
						o << "\tclient->start_json_request(\"" << path << "\", " << path.size() << ");" << endl;
					} else {
						o << "\tpb_buffer *b = client->start_json_request(\"" << path << "\", " << path.size() << ");" << endl;
						o << "\tpb_print_msg(b, req);" << endl;
					}
					o << "\tint err = client->send_json_request(&body);" << endl;
				}
				if (is_empty(out)) {
					o << "\treturn err;" << endl;
				} else if (m->options && m->options->binary) {
					o << "\tif (!err) {" << endl;
					o << "\t\tpb_decode(&body[0], &body[body.size()], obj, resp);" << endl;
					o << "\t}" << endl;
					o << "\treturn err;" << endl;
				} else {
					o << "\tif (err) {" << endl;
					o << "\t\treturn err;" << endl;
					o << "\t}" << endl;
					o << "\treturn (pb_parse_msg(&body[0], obj, resp) == pb_errret) ? -1 : 0;" << endl;
				}
				o << "}" << endl;
			}
		}
		o << endl;
	}
}
#endif
