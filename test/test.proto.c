#include "test/test.proto.h"

const char *pb_parse_TestMessage(const char *p, pb_buf_t *obj, struct TestMessage *m) {
	(void) obj;
	(void) m;
	if (!pb_parse_map(&p)) {
		return p;
	}
	for (;;) {
		pb_string_t key;
		switch (pb_parse_field(&p, &key, 3712286036) % 321) {
		case 0:
			return p;
		case 98:
			if (pb_cmp(key, "b")) {
				goto unknown;
			}
			p = pb_parse_bool(p, &m->b);
			continue;
		case 267:
			if (pb_cmp(key, "u32")) {
				goto unknown;
			}
			p = pb_parse_u32(p, &m->u32);
			continue;
		case 3:
			if (pb_cmp(key, "u64")) {
				goto unknown;
			}
			p = pb_parse_u64(p, &m->u64);
			continue;
		case 166:
			if (pb_cmp(key, "i32")) {
				goto unknown;
			}
			p = pb_parse_i32(p, &m->i32);
			continue;
		case 223:
			if (pb_cmp(key, "i64")) {
				goto unknown;
			}
			p = pb_parse_i64(p, &m->i64);
			continue;
		case 281:
			if (pb_cmp(key, "s32")) {
				goto unknown;
			}
			p = pb_parse_i32(p, &m->s32);
			continue;
		case 17:
			if (pb_cmp(key, "s64")) {
				goto unknown;
			}
			p = pb_parse_i64(p, &m->s64);
			continue;
		case 255:
			if (pb_cmp(key, "f32")) {
				goto unknown;
			}
			p = pb_parse_u32(p, &m->f32);
			continue;
		case 312:
			if (pb_cmp(key, "f64")) {
				goto unknown;
			}
			p = pb_parse_u64(p, &m->f64);
			continue;
		case 275:
			if (pb_cmp(key, "sf32")) {
				goto unknown;
			}
			p = pb_parse_i32(p, &m->sf32);
			continue;
		case 196:
			if (pb_cmp(key, "sf64")) {
				goto unknown;
			}
			p = pb_parse_i64(p, &m->sf64);
			continue;
		case 102:
			if (pb_cmp(key, "f")) {
				goto unknown;
			}
			p = pb_parse_float(p, &m->f);
			continue;
		case 100:
			if (pb_cmp(key, "d")) {
				goto unknown;
			}
			p = pb_parse_double(p, &m->d);
			continue;
		case 36:
			if (pb_cmp(key, "en")) {
				goto unknown;
			}
			p = pb_parse_TestEnum(p, &m->en);
			continue;
		case 128:
			if (pb_cmp(key, "by")) {
				goto unknown;
			}
			p = pb_parse_bytes(p, &m->by);
			continue;
		case 242:
			if (pb_cmp(key, "str")) {
				goto unknown;
			}
			p = pb_parse_string(p, &m->str);
			continue;
		case 159:
			if (pb_cmp(key, "rb")) {
				goto unknown;
			}
			p = pb_parse_array_bool(p, obj, &m->rb.v, &m->rb.len);
			continue;
		case 230:
			if (pb_cmp(key, "ru32")) {
				goto unknown;
			}
			p = pb_parse_array_u32(p, obj, &m->ru32.v, &m->ru32.len);
			continue;
		case 151:
			if (pb_cmp(key, "ru64")) {
				goto unknown;
			}
			p = pb_parse_array_u64(p, obj, &m->ru64.v, &m->ru64.len);
			continue;
		case 129:
			if (pb_cmp(key, "ri32")) {
				goto unknown;
			}
			p = pb_parse_array_i32(p, obj, &m->ri32.v, &m->ri32.len);
			continue;
		case 50:
			if (pb_cmp(key, "ri64")) {
				goto unknown;
			}
			p = pb_parse_array_i64(p, obj, &m->ri64.v, &m->ri64.len);
			continue;
		case 244:
			if (pb_cmp(key, "rs32")) {
				goto unknown;
			}
			p = pb_parse_array_i32(p, obj, &m->rs32.v, &m->rs32.len);
			continue;
		case 165:
			if (pb_cmp(key, "rs64")) {
				goto unknown;
			}
			p = pb_parse_array_i64(p, obj, &m->rs64.v, &m->rs64.len);
			continue;
		case 218:
			if (pb_cmp(key, "rf32")) {
				goto unknown;
			}
			p = pb_parse_array_u32(p, obj, &m->rf32.v, &m->rf32.len);
			continue;
		case 139:
			if (pb_cmp(key, "rf64")) {
				goto unknown;
			}
			p = pb_parse_array_u64(p, obj, &m->rf64.v, &m->rf64.len);
			continue;
		case 38:
			if (pb_cmp(key, "rsf32")) {
				goto unknown;
			}
			p = pb_parse_array_i32(p, obj, &m->rsf32.v, &m->rsf32.len);
			continue;
		case 280:
			if (pb_cmp(key, "rsf64")) {
				goto unknown;
			}
			p = pb_parse_array_i64(p, obj, &m->rsf64.v, &m->rsf64.len);
			continue;
		case 163:
			if (pb_cmp(key, "rf")) {
				goto unknown;
			}
			p = pb_parse_array_float(p, obj, &m->rf.v, &m->rf.len);
			continue;
		case 161:
			if (pb_cmp(key, "rd")) {
				goto unknown;
			}
			p = pb_parse_array_double(p, obj, &m->rd.v, &m->rd.len);
			continue;
		case 81:
			if (pb_cmp(key, "rby")) {
				goto unknown;
			}
			p = pb_parse_array_bytes(p, obj, &m->rby.v, &m->rby.len);
			continue;
		case 205:
			if (pb_cmp(key, "rstr")) {
				goto unknown;
			}
			p = pb_parse_array_string(p, obj, &m->rstr.v, &m->rstr.len);
			continue;
		case 125:
			if (pb_cmp(key, "ren")) {
				goto unknown;
			}
			if (pb_parse_array(&p)) {
				int n = 0;
				m->ren.v = (enum TestEnum*) obj->next;
				do {
					if (obj->next + n * sizeof(enum TestEnum) > obj->end) {return pb_errret;}
					p = pb_parse_TestEnum(p, (enum TestEnum*) &m->ren.v[n++]); 
				} while (pb_more_array(&p));
				obj->next += n * sizeof(enum TestEnum);
				m->ren.len = n;
			}
			continue;
		case 222:
			if (pb_cmp(key, "msg")) {
				goto unknown;
			}
			m->msg = (struct TestMessage*) pb_calloc(obj, sizeof(struct TestMessage));
			if (!m->msg) {return pb_errret;}
			p = pb_parse_TestMessage(p, obj, (struct TestMessage*) m->msg);
			continue;
		case 209:
			if (pb_cmp(key, "pod")) {
				goto unknown;
			}
			p = pb_parse_TestPod(p, &m->pod);
			continue;
		case 49:
			if (pb_cmp(key, "rmsg")) {
				goto unknown;
			}
			if (pb_parse_array(&p)) {
				struct TestMessage *prev = NULL;
				do {
					struct TestMessage *c = (struct TestMessage*) pb_calloc(obj, sizeof(struct TestMessage));
					if (!c) {return pb_errret;}
					p = pb_parse_TestMessage(p, obj, c);
					m->rmsg.len++;
					c->pb_hdr.prev = prev;
					prev = c;
				} while (pb_more_array(&p));

				m->rmsg.v = (const struct TestMessage**) pb_calloc(obj, m->rmsg.len * sizeof(struct TestMessage*));
				if (!m->rmsg.v) {return pb_errret;}

				for (int i = m->rmsg.len - 1; i >= 0; i--) {
					m->rmsg.v[i] = prev;
					prev = (struct TestMessage*) prev->pb_hdr.prev;
				}
			}
			continue;
		case 172:
			if (pb_cmp(key, "rpod")) {
				goto unknown;
			}
			if (pb_parse_array(&p)) {
				int n = 0;
				m->rpod.v = (struct TestPod*) obj->next;
				do {
					if (obj->next + n * sizeof(struct TestPod) > obj->end) {return pb_errret;}
					p = pb_parse_TestPod(p, (struct TestPod*) &m->rpod.v[n++]); 
				} while (pb_more_array(&p));
				obj->next += n * sizeof(struct TestPod);
				m->rpod.len = n;
			}
			continue;
		case 51:
			if (pb_cmp(key, "mb")) {
				goto unknown;
			}
			if (pb_parse_array(&p)) {
				int n = 0;
				m->mb.v = (struct TestMessage_MbEntry*) obj->next;
				do {
					if (obj->next + n * sizeof(struct TestMessage_MbEntry) > obj->end) {return pb_errret;}
					p = pb_parse_TestMessage_MbEntry(p, (struct TestMessage_MbEntry*) &m->mb.v[n++]); 
				} while (pb_more_array(&p));
				obj->next += n * sizeof(struct TestMessage_MbEntry);
				m->mb.len = n;
			}
			continue;
		case 315:
			if (pb_cmp(key, "mu32")) {
				goto unknown;
			}
			if (pb_parse_array(&p)) {
				int n = 0;
				m->mu32.v = (struct TestMessage_Mu32Entry*) obj->next;
				do {
					if (obj->next + n * sizeof(struct TestMessage_Mu32Entry) > obj->end) {return pb_errret;}
					p = pb_parse_TestMessage_Mu32Entry(p, (struct TestMessage_Mu32Entry*) &m->mu32.v[n++]); 
				} while (pb_more_array(&p));
				obj->next += n * sizeof(struct TestMessage_Mu32Entry);
				m->mu32.len = n;
			}
			continue;
		case 236:
			if (pb_cmp(key, "mu64")) {
				goto unknown;
			}
			if (pb_parse_array(&p)) {
				int n = 0;
				m->mu64.v = (struct TestMessage_Mu64Entry*) obj->next;
				do {
					if (obj->next + n * sizeof(struct TestMessage_Mu64Entry) > obj->end) {return pb_errret;}
					p = pb_parse_TestMessage_Mu64Entry(p, (struct TestMessage_Mu64Entry*) &m->mu64.v[n++]); 
				} while (pb_more_array(&p));
				obj->next += n * sizeof(struct TestMessage_Mu64Entry);
				m->mu64.len = n;
			}
			continue;
		case 214:
			if (pb_cmp(key, "mi32")) {
				goto unknown;
			}
			if (pb_parse_array(&p)) {
				int n = 0;
				m->mi32.v = (struct TestMessage_Mi32Entry*) obj->next;
				do {
					if (obj->next + n * sizeof(struct TestMessage_Mi32Entry) > obj->end) {return pb_errret;}
					p = pb_parse_TestMessage_Mi32Entry(p, (struct TestMessage_Mi32Entry*) &m->mi32.v[n++]); 
				} while (pb_more_array(&p));
				obj->next += n * sizeof(struct TestMessage_Mi32Entry);
				m->mi32.len = n;
			}
			continue;
		case 135:
			if (pb_cmp(key, "mi64")) {
				goto unknown;
			}
			if (pb_parse_array(&p)) {
				int n = 0;
				m->mi64.v = (struct TestMessage_Mi64Entry*) obj->next;
				do {
					if (obj->next + n * sizeof(struct TestMessage_Mi64Entry) > obj->end) {return pb_errret;}
					p = pb_parse_TestMessage_Mi64Entry(p, (struct TestMessage_Mi64Entry*) &m->mi64.v[n++]); 
				} while (pb_more_array(&p));
				obj->next += n * sizeof(struct TestMessage_Mi64Entry);
				m->mi64.len = n;
			}
			continue;
		case 8:
			if (pb_cmp(key, "ms32")) {
				goto unknown;
			}
			if (pb_parse_array(&p)) {
				int n = 0;
				m->ms32.v = (struct TestMessage_Ms32Entry*) obj->next;
				do {
					if (obj->next + n * sizeof(struct TestMessage_Ms32Entry) > obj->end) {return pb_errret;}
					p = pb_parse_TestMessage_Ms32Entry(p, (struct TestMessage_Ms32Entry*) &m->ms32.v[n++]); 
				} while (pb_more_array(&p));
				obj->next += n * sizeof(struct TestMessage_Ms32Entry);
				m->ms32.len = n;
			}
			continue;
		case 250:
			if (pb_cmp(key, "ms64")) {
				goto unknown;
			}
			if (pb_parse_array(&p)) {
				int n = 0;
				m->ms64.v = (struct TestMessage_Ms64Entry*) obj->next;
				do {
					if (obj->next + n * sizeof(struct TestMessage_Ms64Entry) > obj->end) {return pb_errret;}
					p = pb_parse_TestMessage_Ms64Entry(p, (struct TestMessage_Ms64Entry*) &m->ms64.v[n++]); 
				} while (pb_more_array(&p));
				obj->next += n * sizeof(struct TestMessage_Ms64Entry);
				m->ms64.len = n;
			}
			continue;
		case 303:
			if (pb_cmp(key, "mf32")) {
				goto unknown;
			}
			if (pb_parse_array(&p)) {
				int n = 0;
				m->mf32.v = (struct TestMessage_Mf32Entry*) obj->next;
				do {
					if (obj->next + n * sizeof(struct TestMessage_Mf32Entry) > obj->end) {return pb_errret;}
					p = pb_parse_TestMessage_Mf32Entry(p, (struct TestMessage_Mf32Entry*) &m->mf32.v[n++]); 
				} while (pb_more_array(&p));
				obj->next += n * sizeof(struct TestMessage_Mf32Entry);
				m->mf32.len = n;
			}
			continue;
		case 224:
			if (pb_cmp(key, "mf64")) {
				goto unknown;
			}
			if (pb_parse_array(&p)) {
				int n = 0;
				m->mf64.v = (struct TestMessage_Mf64Entry*) obj->next;
				do {
					if (obj->next + n * sizeof(struct TestMessage_Mf64Entry) > obj->end) {return pb_errret;}
					p = pb_parse_TestMessage_Mf64Entry(p, (struct TestMessage_Mf64Entry*) &m->mf64.v[n++]); 
				} while (pb_more_array(&p));
				obj->next += n * sizeof(struct TestMessage_Mf64Entry);
				m->mf64.len = n;
			}
			continue;
		case 276:
			if (pb_cmp(key, "msf32")) {
				goto unknown;
			}
			if (pb_parse_array(&p)) {
				int n = 0;
				m->msf32.v = (struct TestMessage_Msf32Entry*) obj->next;
				do {
					if (obj->next + n * sizeof(struct TestMessage_Msf32Entry) > obj->end) {return pb_errret;}
					p = pb_parse_TestMessage_Msf32Entry(p, (struct TestMessage_Msf32Entry*) &m->msf32.v[n++]); 
				} while (pb_more_array(&p));
				obj->next += n * sizeof(struct TestMessage_Msf32Entry);
				m->msf32.len = n;
			}
			continue;
		case 12:
			if (pb_cmp(key, "msf64")) {
				goto unknown;
			}
			if (pb_parse_array(&p)) {
				int n = 0;
				m->msf64.v = (struct TestMessage_Msf64Entry*) obj->next;
				do {
					if (obj->next + n * sizeof(struct TestMessage_Msf64Entry) > obj->end) {return pb_errret;}
					p = pb_parse_TestMessage_Msf64Entry(p, (struct TestMessage_Msf64Entry*) &m->msf64.v[n++]); 
				} while (pb_more_array(&p));
				obj->next += n * sizeof(struct TestMessage_Msf64Entry);
				m->msf64.len = n;
			}
			continue;
		case 55:
			if (pb_cmp(key, "mf")) {
				goto unknown;
			}
			if (pb_parse_array(&p)) {
				int n = 0;
				m->mf.v = (struct TestMessage_MfEntry*) obj->next;
				do {
					if (obj->next + n * sizeof(struct TestMessage_MfEntry) > obj->end) {return pb_errret;}
					p = pb_parse_TestMessage_MfEntry(p, (struct TestMessage_MfEntry*) &m->mf.v[n++]); 
				} while (pb_more_array(&p));
				obj->next += n * sizeof(struct TestMessage_MfEntry);
				m->mf.len = n;
			}
			continue;
		case 53:
			if (pb_cmp(key, "md")) {
				goto unknown;
			}
			if (pb_parse_array(&p)) {
				int n = 0;
				m->md.v = (struct TestMessage_MdEntry*) obj->next;
				do {
					if (obj->next + n * sizeof(struct TestMessage_MdEntry) > obj->end) {return pb_errret;}
					p = pb_parse_TestMessage_MdEntry(p, (struct TestMessage_MdEntry*) &m->md.v[n++]); 
				} while (pb_more_array(&p));
				obj->next += n * sizeof(struct TestMessage_MdEntry);
				m->md.len = n;
			}
			continue;
		case 184:
			if (pb_cmp(key, "mby")) {
				goto unknown;
			}
			if (pb_parse_array(&p)) {
				struct TestMessage_MbyEntry *prev = NULL;
				do {
					struct TestMessage_MbyEntry *c = (struct TestMessage_MbyEntry*) pb_calloc(obj, sizeof(struct TestMessage_MbyEntry));
					if (!c) {return pb_errret;}
					p = pb_parse_TestMessage_MbyEntry(p, obj, c);
					m->mby.len++;
					c->pb_hdr.prev = prev;
					prev = c;
				} while (pb_more_array(&p));

				m->mby.v = (const struct TestMessage_MbyEntry**) pb_calloc(obj, m->mby.len * sizeof(struct TestMessage_MbyEntry*));
				if (!m->mby.v) {return pb_errret;}

				for (int i = m->mby.len - 1; i >= 0; i--) {
					m->mby.v[i] = prev;
					prev = (struct TestMessage_MbyEntry*) prev->pb_hdr.prev;
				}
			}
			continue;
		case 290:
			if (pb_cmp(key, "mstr")) {
				goto unknown;
			}
			if (pb_parse_array(&p)) {
				struct TestMessage_MstrEntry *prev = NULL;
				do {
					struct TestMessage_MstrEntry *c = (struct TestMessage_MstrEntry*) pb_calloc(obj, sizeof(struct TestMessage_MstrEntry));
					if (!c) {return pb_errret;}
					p = pb_parse_TestMessage_MstrEntry(p, obj, c);
					m->mstr.len++;
					c->pb_hdr.prev = prev;
					prev = c;
				} while (pb_more_array(&p));

				m->mstr.v = (const struct TestMessage_MstrEntry**) pb_calloc(obj, m->mstr.len * sizeof(struct TestMessage_MstrEntry*));
				if (!m->mstr.v) {return pb_errret;}

				for (int i = m->mstr.len - 1; i >= 0; i--) {
					m->mstr.v[i] = prev;
					prev = (struct TestMessage_MstrEntry*) prev->pb_hdr.prev;
				}
			}
			continue;
		case 228:
			if (pb_cmp(key, "men")) {
				goto unknown;
			}
			if (pb_parse_array(&p)) {
				int n = 0;
				m->men.v = (struct TestMessage_MenEntry*) obj->next;
				do {
					if (obj->next + n * sizeof(struct TestMessage_MenEntry) > obj->end) {return pb_errret;}
					p = pb_parse_TestMessage_MenEntry(p, (struct TestMessage_MenEntry*) &m->men.v[n++]); 
				} while (pb_more_array(&p));
				obj->next += n * sizeof(struct TestMessage_MenEntry);
				m->men.len = n;
			}
			continue;
		case 134:
			if (pb_cmp(key, "mmsg")) {
				goto unknown;
			}
			if (pb_parse_array(&p)) {
				struct TestMessage_MmsgEntry *prev = NULL;
				do {
					struct TestMessage_MmsgEntry *c = (struct TestMessage_MmsgEntry*) pb_calloc(obj, sizeof(struct TestMessage_MmsgEntry));
					if (!c) {return pb_errret;}
					p = pb_parse_TestMessage_MmsgEntry(p, obj, c);
					m->mmsg.len++;
					c->pb_hdr.prev = prev;
					prev = c;
				} while (pb_more_array(&p));

				m->mmsg.v = (const struct TestMessage_MmsgEntry**) pb_calloc(obj, m->mmsg.len * sizeof(struct TestMessage_MmsgEntry*));
				if (!m->mmsg.v) {return pb_errret;}

				for (int i = m->mmsg.len - 1; i >= 0; i--) {
					m->mmsg.v[i] = prev;
					prev = (struct TestMessage_MmsgEntry*) prev->pb_hdr.prev;
				}
			}
			continue;
		case 257:
			if (pb_cmp(key, "mpod")) {
				goto unknown;
			}
			if (pb_parse_array(&p)) {
				int n = 0;
				m->mpod.v = (struct TestMessage_MpodEntry*) obj->next;
				do {
					if (obj->next + n * sizeof(struct TestMessage_MpodEntry) > obj->end) {return pb_errret;}
					p = pb_parse_TestMessage_MpodEntry(p, (struct TestMessage_MpodEntry*) &m->mpod.v[n++]); 
				} while (pb_more_array(&p));
				obj->next += n * sizeof(struct TestMessage_MpodEntry);
				m->mpod.len = n;
			}
			continue;
		default:
		unknown:
			p = pb_parse_skip(p);
		}
	}
}
int pb_print_TestMessage(pb_buf_t *a, struct TestMessage const *m) {
	char *p = pb_appendsz(a, 312);
	if (!p) {return -1;}
	*p++ = '{';
	if (m->b) {
		memcpy(p, "\"b\":", 4);
		p += 4;
		p = pb_print_bool(p, m->b);
	}
	if (m->u32) {
		memcpy(p, "\"u32\":", 6);
		p += 6;
		p = pb_print_u32(p, m->u32);
	}
	if (m->u64) {
		memcpy(p, "\"u64\":", 6);
		p += 6;
		p = pb_print_u64(p, m->u64);
	}
	if (m->i32) {
		memcpy(p, "\"i32\":", 6);
		p += 6;
		p = pb_print_i32(p, m->i32);
	}
	if (m->i64) {
		memcpy(p, "\"i64\":", 6);
		p += 6;
		p = pb_print_i64(p, m->i64);
	}
	if (m->s32) {
		memcpy(p, "\"s32\":", 6);
		p += 6;
		p = pb_print_i32(p, m->s32);
	}
	if (m->s64) {
		memcpy(p, "\"s64\":", 6);
		p += 6;
		p = pb_print_i64(p, m->s64);
	}
	if (m->f32) {
		memcpy(p, "\"f32\":", 6);
		p += 6;
		p = pb_print_u32(p, m->f32);
	}
	if (m->f64) {
		memcpy(p, "\"f64\":", 6);
		p += 6;
		p = pb_print_u64(p, m->f64);
	}
	if (m->sf32) {
		memcpy(p, "\"sf32\":", 7);
		p += 7;
		p = pb_print_i32(p, m->sf32);
	}
	if (m->sf64) {
		memcpy(p, "\"sf64\":", 7);
		p += 7;
		p = pb_print_i64(p, m->sf64);
	}
	if (m->f) {
		memcpy(p, "\"f\":", 4);
		p += 4;
		p = pb_print_float(p, m->f);
	}
	if (m->d) {
		memcpy(p, "\"d\":", 4);
		p += 4;
		p = pb_print_double(p, m->d);
	}
	if (m->en) {
		memcpy(p, "\"en\":", 5);
		p += 5;
		p = pb_print_TestEnum(p, m->en);
	}
	a->next = p;
	if (m->by.len) {
		if (pb_append(a, "\"by\":", 5)) {return -1;}
		if (pb_print_bytes(a, m->by)) {return -1;}
		a->next[-1] = ','; // replace trailing newline
	}
	if (m->str.len) {
		if (pb_append(a, "\"str\":", 6)) {return -1;}
		if (pb_print_string(a, m->str)) {return -1;}
		a->next[-1] = ','; // replace trailing newline
	}
	if (m->rb.len) {
		if (pb_append(a, "\"rb\":", 5)) {return -1;}
		char *mp = pb_appendsz(a, 3 /*[],*/ + m->rb.len * 6);
		if (!mp) {return -1;}
		*mp++ = '[';
		for (int i = 0; i < m->rb.len; i++) {
			mp = pb_print_bool(mp, m->rb.v[i]);
		}
		mp = pb_print_array_end_i(mp);
		a->next = mp;
	}
	if (m->ru32.len) {
		if (pb_append(a, "\"ru32\":", 7)) {return -1;}
		char *mp = pb_appendsz(a, 3 /*[],*/ + m->ru32.len * 12);
		if (!mp) {return -1;}
		*mp++ = '[';
		for (int i = 0; i < m->ru32.len; i++) {
			mp = pb_print_u32(mp, m->ru32.v[i]);
		}
		mp = pb_print_array_end_i(mp);
		a->next = mp;
	}
	if (m->ru64.len) {
		if (pb_append(a, "\"ru64\":", 7)) {return -1;}
		char *mp = pb_appendsz(a, 3 /*[],*/ + m->ru64.len * 24);
		if (!mp) {return -1;}
		*mp++ = '[';
		for (int i = 0; i < m->ru64.len; i++) {
			mp = pb_print_u64(mp, m->ru64.v[i]);
		}
		mp = pb_print_array_end_i(mp);
		a->next = mp;
	}
	if (m->ri32.len) {
		if (pb_append(a, "\"ri32\":", 7)) {return -1;}
		char *mp = pb_appendsz(a, 3 /*[],*/ + m->ri32.len * 12);
		if (!mp) {return -1;}
		*mp++ = '[';
		for (int i = 0; i < m->ri32.len; i++) {
			mp = pb_print_i32(mp, m->ri32.v[i]);
		}
		mp = pb_print_array_end_i(mp);
		a->next = mp;
	}
	if (m->ri64.len) {
		if (pb_append(a, "\"ri64\":", 7)) {return -1;}
		char *mp = pb_appendsz(a, 3 /*[],*/ + m->ri64.len * 24);
		if (!mp) {return -1;}
		*mp++ = '[';
		for (int i = 0; i < m->ri64.len; i++) {
			mp = pb_print_i64(mp, m->ri64.v[i]);
		}
		mp = pb_print_array_end_i(mp);
		a->next = mp;
	}
	if (m->rs32.len) {
		if (pb_append(a, "\"rs32\":", 7)) {return -1;}
		char *mp = pb_appendsz(a, 3 /*[],*/ + m->rs32.len * 12);
		if (!mp) {return -1;}
		*mp++ = '[';
		for (int i = 0; i < m->rs32.len; i++) {
			mp = pb_print_i32(mp, m->rs32.v[i]);
		}
		mp = pb_print_array_end_i(mp);
		a->next = mp;
	}
	if (m->rs64.len) {
		if (pb_append(a, "\"rs64\":", 7)) {return -1;}
		char *mp = pb_appendsz(a, 3 /*[],*/ + m->rs64.len * 24);
		if (!mp) {return -1;}
		*mp++ = '[';
		for (int i = 0; i < m->rs64.len; i++) {
			mp = pb_print_i64(mp, m->rs64.v[i]);
		}
		mp = pb_print_array_end_i(mp);
		a->next = mp;
	}
	if (m->rf32.len) {
		if (pb_append(a, "\"rf32\":", 7)) {return -1;}
		char *mp = pb_appendsz(a, 3 /*[],*/ + m->rf32.len * 12);
		if (!mp) {return -1;}
		*mp++ = '[';
		for (int i = 0; i < m->rf32.len; i++) {
			mp = pb_print_u32(mp, m->rf32.v[i]);
		}
		mp = pb_print_array_end_i(mp);
		a->next = mp;
	}
	if (m->rf64.len) {
		if (pb_append(a, "\"rf64\":", 7)) {return -1;}
		char *mp = pb_appendsz(a, 3 /*[],*/ + m->rf64.len * 24);
		if (!mp) {return -1;}
		*mp++ = '[';
		for (int i = 0; i < m->rf64.len; i++) {
			mp = pb_print_u64(mp, m->rf64.v[i]);
		}
		mp = pb_print_array_end_i(mp);
		a->next = mp;
	}
	if (m->rsf32.len) {
		if (pb_append(a, "\"rsf32\":", 8)) {return -1;}
		char *mp = pb_appendsz(a, 3 /*[],*/ + m->rsf32.len * 12);
		if (!mp) {return -1;}
		*mp++ = '[';
		for (int i = 0; i < m->rsf32.len; i++) {
			mp = pb_print_i32(mp, m->rsf32.v[i]);
		}
		mp = pb_print_array_end_i(mp);
		a->next = mp;
	}
	if (m->rsf64.len) {
		if (pb_append(a, "\"rsf64\":", 8)) {return -1;}
		char *mp = pb_appendsz(a, 3 /*[],*/ + m->rsf64.len * 24);
		if (!mp) {return -1;}
		*mp++ = '[';
		for (int i = 0; i < m->rsf64.len; i++) {
			mp = pb_print_i64(mp, m->rsf64.v[i]);
		}
		mp = pb_print_array_end_i(mp);
		a->next = mp;
	}
	if (m->rf.len) {
		if (pb_append(a, "\"rf\":", 5)) {return -1;}
		char *mp = pb_appendsz(a, 3 /*[],*/ + m->rf.len * 14);
		if (!mp) {return -1;}
		*mp++ = '[';
		for (int i = 0; i < m->rf.len; i++) {
			mp = pb_print_float(mp, m->rf.v[i]);
		}
		mp = pb_print_array_end_i(mp);
		a->next = mp;
	}
	if (m->rd.len) {
		if (pb_append(a, "\"rd\":", 5)) {return -1;}
		char *mp = pb_appendsz(a, 3 /*[],*/ + m->rd.len * 22);
		if (!mp) {return -1;}
		*mp++ = '[';
		for (int i = 0; i < m->rd.len; i++) {
			mp = pb_print_double(mp, m->rd.v[i]);
		}
		mp = pb_print_array_end_i(mp);
		a->next = mp;
	}
	if (m->rby.len) {
		if (pb_append(a, "\"rby\":", 6)) {return -1;}
		if (pb_append(a, "[", 1)) {return -1;}
		for (int i = 0; i < m->rby.len; i++) {
			if (pb_print_bytes(a, m->rby.v[i])) {return -1;}
			a->next[-1] = ','; // replace trailing newline
		}
		if (pb_print_array_end(a)) {return -1;}
	}
	if (m->rstr.len) {
		if (pb_append(a, "\"rstr\":", 7)) {return -1;}
		if (pb_append(a, "[", 1)) {return -1;}
		for (int i = 0; i < m->rstr.len; i++) {
			if (pb_print_string(a, m->rstr.v[i])) {return -1;}
			a->next[-1] = ','; // replace trailing newline
		}
		if (pb_print_array_end(a)) {return -1;}
	}
	if (m->ren.len) {
		if (pb_append(a, "\"ren\":", 6)) {return -1;}
		char *mp = pb_appendsz(a, 3 /*[],*/ + m->ren.len * 10);
		if (!mp) {return -1;}
		*mp++ = '[';
		for (int i = 0; i < m->ren.len; i++) {
			mp = pb_print_TestEnum(mp, m->ren.v[i]);
		}
		mp = pb_print_array_end_i(mp);
		a->next = mp;
	}
	if (m->msg) {
		if (pb_append(a, "\"msg\":", 6)) {return -1;}
		if (pb_print_TestMessage(a, m->msg)) {return -1;}
		a->next[-1] = ','; // replace trailing newline
	}
	if (pb_nonzero_TestPod(&m->pod)) {
		if (pb_append(a, "\"pod\":", 6)) {return -1;}
		if (pb_print_TestPod(a, &m->pod)) {return -1;}
		a->next[-1] = ','; // replace trailing newline
	}
	if (m->rmsg.len) {
		if (pb_append(a, "\"rmsg\":", 7)) {return -1;}
		if (pb_append(a, "[", 1)) {return -1;}
		for (int i = 0; i < m->rmsg.len; i++) {
			if (pb_print_TestMessage(a, m->rmsg.v[i])) {return -1;}
			a->next[-1] = ','; // replace trailing newline
		}
		if (pb_print_array_end(a)) {return -1;}
	}
	if (m->rpod.len) {
		if (pb_append(a, "\"rpod\":", 7)) {return -1;}
		if (pb_append(a, "[", 1)) {return -1;}
		for (int i = 0; i < m->rpod.len; i++) {
			if (pb_print_TestPod(a, &m->rpod.v[i])) {return -1;}
			a->next[-1] = ','; // replace trailing newline
		}
		if (pb_print_array_end(a)) {return -1;}
	}
	if (m->mb.len) {
		if (pb_append(a, "\"mb\":", 5)) {return -1;}
		if (pb_append(a, "[", 1)) {return -1;}
		for (int i = 0; i < m->mb.len; i++) {
			if (pb_print_TestMessage_MbEntry(a, &m->mb.v[i])) {return -1;}
			a->next[-1] = ','; // replace trailing newline
		}
		if (pb_print_array_end(a)) {return -1;}
	}
	if (m->mu32.len) {
		if (pb_append(a, "\"mu32\":", 7)) {return -1;}
		if (pb_append(a, "[", 1)) {return -1;}
		for (int i = 0; i < m->mu32.len; i++) {
			if (pb_print_TestMessage_Mu32Entry(a, &m->mu32.v[i])) {return -1;}
			a->next[-1] = ','; // replace trailing newline
		}
		if (pb_print_array_end(a)) {return -1;}
	}
	if (m->mu64.len) {
		if (pb_append(a, "\"mu64\":", 7)) {return -1;}
		if (pb_append(a, "[", 1)) {return -1;}
		for (int i = 0; i < m->mu64.len; i++) {
			if (pb_print_TestMessage_Mu64Entry(a, &m->mu64.v[i])) {return -1;}
			a->next[-1] = ','; // replace trailing newline
		}
		if (pb_print_array_end(a)) {return -1;}
	}
	if (m->mi32.len) {
		if (pb_append(a, "\"mi32\":", 7)) {return -1;}
		if (pb_append(a, "[", 1)) {return -1;}
		for (int i = 0; i < m->mi32.len; i++) {
			if (pb_print_TestMessage_Mi32Entry(a, &m->mi32.v[i])) {return -1;}
			a->next[-1] = ','; // replace trailing newline
		}
		if (pb_print_array_end(a)) {return -1;}
	}
	if (m->mi64.len) {
		if (pb_append(a, "\"mi64\":", 7)) {return -1;}
		if (pb_append(a, "[", 1)) {return -1;}
		for (int i = 0; i < m->mi64.len; i++) {
			if (pb_print_TestMessage_Mi64Entry(a, &m->mi64.v[i])) {return -1;}
			a->next[-1] = ','; // replace trailing newline
		}
		if (pb_print_array_end(a)) {return -1;}
	}
	if (m->ms32.len) {
		if (pb_append(a, "\"ms32\":", 7)) {return -1;}
		if (pb_append(a, "[", 1)) {return -1;}
		for (int i = 0; i < m->ms32.len; i++) {
			if (pb_print_TestMessage_Ms32Entry(a, &m->ms32.v[i])) {return -1;}
			a->next[-1] = ','; // replace trailing newline
		}
		if (pb_print_array_end(a)) {return -1;}
	}
	if (m->ms64.len) {
		if (pb_append(a, "\"ms64\":", 7)) {return -1;}
		if (pb_append(a, "[", 1)) {return -1;}
		for (int i = 0; i < m->ms64.len; i++) {
			if (pb_print_TestMessage_Ms64Entry(a, &m->ms64.v[i])) {return -1;}
			a->next[-1] = ','; // replace trailing newline
		}
		if (pb_print_array_end(a)) {return -1;}
	}
	if (m->mf32.len) {
		if (pb_append(a, "\"mf32\":", 7)) {return -1;}
		if (pb_append(a, "[", 1)) {return -1;}
		for (int i = 0; i < m->mf32.len; i++) {
			if (pb_print_TestMessage_Mf32Entry(a, &m->mf32.v[i])) {return -1;}
			a->next[-1] = ','; // replace trailing newline
		}
		if (pb_print_array_end(a)) {return -1;}
	}
	if (m->mf64.len) {
		if (pb_append(a, "\"mf64\":", 7)) {return -1;}
		if (pb_append(a, "[", 1)) {return -1;}
		for (int i = 0; i < m->mf64.len; i++) {
			if (pb_print_TestMessage_Mf64Entry(a, &m->mf64.v[i])) {return -1;}
			a->next[-1] = ','; // replace trailing newline
		}
		if (pb_print_array_end(a)) {return -1;}
	}
	if (m->msf32.len) {
		if (pb_append(a, "\"msf32\":", 8)) {return -1;}
		if (pb_append(a, "[", 1)) {return -1;}
		for (int i = 0; i < m->msf32.len; i++) {
			if (pb_print_TestMessage_Msf32Entry(a, &m->msf32.v[i])) {return -1;}
			a->next[-1] = ','; // replace trailing newline
		}
		if (pb_print_array_end(a)) {return -1;}
	}
	if (m->msf64.len) {
		if (pb_append(a, "\"msf64\":", 8)) {return -1;}
		if (pb_append(a, "[", 1)) {return -1;}
		for (int i = 0; i < m->msf64.len; i++) {
			if (pb_print_TestMessage_Msf64Entry(a, &m->msf64.v[i])) {return -1;}
			a->next[-1] = ','; // replace trailing newline
		}
		if (pb_print_array_end(a)) {return -1;}
	}
	if (m->mf.len) {
		if (pb_append(a, "\"mf\":", 5)) {return -1;}
		if (pb_append(a, "[", 1)) {return -1;}
		for (int i = 0; i < m->mf.len; i++) {
			if (pb_print_TestMessage_MfEntry(a, &m->mf.v[i])) {return -1;}
			a->next[-1] = ','; // replace trailing newline
		}
		if (pb_print_array_end(a)) {return -1;}
	}
	if (m->md.len) {
		if (pb_append(a, "\"md\":", 5)) {return -1;}
		if (pb_append(a, "[", 1)) {return -1;}
		for (int i = 0; i < m->md.len; i++) {
			if (pb_print_TestMessage_MdEntry(a, &m->md.v[i])) {return -1;}
			a->next[-1] = ','; // replace trailing newline
		}
		if (pb_print_array_end(a)) {return -1;}
	}
	if (m->mby.len) {
		if (pb_append(a, "\"mby\":", 6)) {return -1;}
		if (pb_append(a, "[", 1)) {return -1;}
		for (int i = 0; i < m->mby.len; i++) {
			if (pb_print_TestMessage_MbyEntry(a, m->mby.v[i])) {return -1;}
			a->next[-1] = ','; // replace trailing newline
		}
		if (pb_print_array_end(a)) {return -1;}
	}
	if (m->mstr.len) {
		if (pb_append(a, "\"mstr\":", 7)) {return -1;}
		if (pb_append(a, "[", 1)) {return -1;}
		for (int i = 0; i < m->mstr.len; i++) {
			if (pb_print_TestMessage_MstrEntry(a, m->mstr.v[i])) {return -1;}
			a->next[-1] = ','; // replace trailing newline
		}
		if (pb_print_array_end(a)) {return -1;}
	}
	if (m->men.len) {
		if (pb_append(a, "\"men\":", 6)) {return -1;}
		if (pb_append(a, "[", 1)) {return -1;}
		for (int i = 0; i < m->men.len; i++) {
			if (pb_print_TestMessage_MenEntry(a, &m->men.v[i])) {return -1;}
			a->next[-1] = ','; // replace trailing newline
		}
		if (pb_print_array_end(a)) {return -1;}
	}
	if (m->mmsg.len) {
		if (pb_append(a, "\"mmsg\":", 7)) {return -1;}
		if (pb_append(a, "[", 1)) {return -1;}
		for (int i = 0; i < m->mmsg.len; i++) {
			if (pb_print_TestMessage_MmsgEntry(a, m->mmsg.v[i])) {return -1;}
			a->next[-1] = ','; // replace trailing newline
		}
		if (pb_print_array_end(a)) {return -1;}
	}
	if (m->mpod.len) {
		if (pb_append(a, "\"mpod\":", 7)) {return -1;}
		if (pb_append(a, "[", 1)) {return -1;}
		for (int i = 0; i < m->mpod.len; i++) {
			if (pb_print_TestMessage_MpodEntry(a, &m->mpod.v[i])) {return -1;}
			a->next[-1] = ','; // replace trailing newline
		}
		if (pb_print_array_end(a)) {return -1;}
	}
	return pb_print_map_end(a);
}
int pb_get_TestMessage(const char *p, const char *e, pb_buf_t *obj, struct TestMessage *m) {
	(void) obj;
	if (pb_skipto_1(&p, e, 8)) {
		p = pb_get_bool(p + 1, e, (bool*)&m->b);
	}
	if (pb_skipto_1(&p, e, 16)) {
		p = pb_get_u32(p + 1, e, (uint32_t*)&m->i32);
	}
	if (pb_skipto_1(&p, e, 24)) {
		p = pb_get_s32(p + 1, e, (int32_t*)&m->s32);
	}
	if (pb_skipto_1(&p, e, 33)) {
		p = pb_get_f32(p + 1, e, (union pb_f32*)&m->sf32);
	}
	if (pb_skipto_1(&p, e, 40)) {
		p = pb_get_u32(p + 1, e, (uint32_t*)&m->u32);
	}
	if (pb_skipto_1(&p, e, 49)) {
		p = pb_get_f32(p + 1, e, (union pb_f32*)&m->f32);
	}
	if (pb_skipto_1(&p, e, 56)) {
		p = pb_get_u64(p + 1, e, (uint64_t*)&m->i64);
	}
	if (pb_skipto_1(&p, e, 69)) {
		p = pb_get_f64(p + 1, e, (union pb_f64*)&m->sf64);
	}
	if (pb_skipto_1(&p, e, 72)) {
		p = pb_get_s64(p + 1, e, (int64_t*)&m->s64);
	}
	if (pb_skipto_1(&p, e, 80)) {
		p = pb_get_u64(p + 1, e, (uint64_t*)&m->u64);
	}
	if (pb_skipto_1(&p, e, 93)) {
		p = pb_get_f64(p + 1, e, (union pb_f64*)&m->f64);
	}
	if (pb_skipto_1(&p, e, 97)) {
		p = pb_get_f32(p + 1, e, (union pb_f32*)&m->f);
	}
	if (pb_skipto_1(&p, e, 109)) {
		p = pb_get_f64(p + 1, e, (union pb_f64*)&m->d);
	}
	if (pb_skipto_1(&p, e, 114)) {
		p = pb_get_bytes(p + 1, e, (pb_bytes_t*)&m->by);
	}
	if (pb_skipto_1(&p, e, 122)) {
		p = pb_get_string(p + 1, e, (pb_string_t*)&m->str);
	}
	p = pb_toend_1(p,e);
	if (pb_skipto_2(&p, e, 384)) {
		p = pb_get_u32(p + 2, e, (uint32_t*)&m->en);
	}
	if (pb_skipto_2(&p, e, 394)) {
		pb_string_t msg;
		p = pb_get_string(p + 2, e, &msg);
		m->msg = (struct TestMessage*) pb_calloc(obj, sizeof(struct TestMessage));
		if (!m->msg || pb_get_TestMessage(msg.c_str, msg.c_str + msg.len, obj, (struct TestMessage*) m->msg)) {
			return -1;
		}
	}
	if (pb_skipto_2(&p, e, 402)) {
		pb_string_t msg;
		p = pb_get_string(p + 2, e, &msg);
		if (pb_get_TestPod(msg.c_str, msg.c_str + msg.len, &m->pod)) {
			return -1;
		}
	}
	if (pb_skipto_2(&p, e, 426)) {
		p = pb_get_packed_bool(p, e, obj, (bool**)&m->rb.v, &m->rb.len);
	}
	if (pb_skipto_2(&p, e, 434)) {
		p = pb_get_packed_u32(p, e, obj, (uint32_t**)&m->ri32.v, &m->ri32.len);
	}
	if (pb_skipto_2(&p, e, 442)) {
		p = pb_get_packed_s32(p, e, obj, (int32_t**)&m->rs32.v, &m->rs32.len);
	}
	if (pb_skipto_2(&p, e, 450)) {
		p = pb_get_packed_f32(p, e, obj, (union pb_f32**)&m->rsf32.v, &m->rsf32.len);
	}
	if (pb_skipto_2(&p, e, 458)) {
		p = pb_get_packed_u32(p, e, obj, (uint32_t**)&m->ru32.v, &m->ru32.len);
	}
	if (pb_skipto_2(&p, e, 466)) {
		p = pb_get_packed_f32(p, e, obj, (union pb_f32**)&m->rf32.v, &m->rf32.len);
	}
	if (pb_skipto_2(&p, e, 474)) {
		p = pb_get_packed_u64(p, e, obj, (uint64_t**)&m->ri64.v, &m->ri64.len);
	}
	if (pb_skipto_2(&p, e, 482)) {
		p = pb_get_packed_f64(p, e, obj, (union pb_f64**)&m->rsf64.v, &m->rsf64.len);
	}
	if (pb_skipto_2(&p, e, 490)) {
		p = pb_get_packed_s64(p, e, obj, (int64_t**)&m->rs64.v, &m->rs64.len);
	}
	if (pb_skipto_2(&p, e, 3474)) {
		p = pb_get_packed_u64(p, e, obj, (uint64_t**)&m->ru64.v, &m->ru64.len);
	}
	if (pb_skipto_2(&p, e, 3482)) {
		p = pb_get_packed_f64(p, e, obj, (union pb_f64**)&m->rf64.v, &m->rf64.len);
	}
	if (pb_skipto_2(&p, e, 3490)) {
		p = pb_get_packed_f32(p, e, obj, (union pb_f32**)&m->rf.v, &m->rf.len);
	}
	if (pb_skipto_2(&p, e, 3498)) {
		p = pb_get_packed_f64(p, e, obj, (union pb_f64**)&m->rd.v, &m->rd.len);
	}
	if (pb_skipto_2(&p, e, 3506)) {
		m->rby.v = (pb_bytes_t*) obj->next;
		do {
			if (obj->next + m->rby.len * sizeof(pb_bytes_t) > obj->end) {
				return -1;
			}
			p = pb_get_bytes(p + 2, e, (pb_bytes_t*)&m->rby.v[m->rby.len]);
			m->rby.len++;
		} while (!pb_cmp_tag_2(p, e, 3506));

		obj->next += m->rby.len * sizeof(pb_bytes_t);
	}
	if (pb_skipto_2(&p, e, 3514)) {
		m->rstr.v = (pb_string_t*) obj->next;
		do {
			if (obj->next + m->rstr.len * sizeof(pb_string_t) > obj->end) {
				return -1;
			}
			p = pb_get_string(p + 2, e, (pb_string_t*)&m->rstr.v[m->rstr.len]);
			m->rstr.len++;
		} while (!pb_cmp_tag_2(p, e, 3514));

		obj->next += m->rstr.len * sizeof(pb_string_t);
	}
	if (pb_skipto_2(&p, e, 3522)) {
		p = pb_get_packed_u32(p, e, obj, (uint32_t**)&m->ren.v, &m->ren.len);
	}
	if (pb_skipto_2(&p, e, 3530)) {
		struct TestMessage *prev = NULL;
		do {
			pb_string_t msg;
			p = pb_get_string(p + 2, e, &msg);
			struct TestMessage *c = (struct TestMessage*) pb_calloc(obj, sizeof(struct TestMessage));
			if (!c || pb_get_TestMessage(msg.c_str, msg.c_str + msg.len, obj, c)) {
				return -1;
			}
			m->rmsg.len++;
			c->pb_hdr.prev = prev;
			prev = c;
		} while (!pb_cmp_tag_2(p, e, 3530));

		m->rmsg.v = (const struct TestMessage**) pb_calloc(obj, m->rmsg.len * sizeof(struct TestMessage*));
		if (!m->rmsg.v) {
			return -1;
		}

		for (int i = m->rmsg.len - 1; i >= 0; i--) {
			m->rmsg.v[i] = prev;
			prev = (struct TestMessage*) prev->pb_hdr.prev;
		}
	}
	if (pb_skipto_2(&p, e, 3538)) {
		m->rpod.v = (struct TestPod*) obj->next;
		do {
			if (obj->next + m->rpod.len * sizeof(struct TestPod) > obj->end) {
				return -1;
			}
			pb_string_t msg;
			p = pb_get_string(p + 2, e, &msg);
			if (pb_get_TestPod(msg.c_str, msg.c_str + msg.len, (struct TestPod*) &m->rpod.v[m->rpod.len])) {
				return -1;
			}
			m->rpod.len++;
		} while (!pb_cmp_tag_2(p, e, 3538));

		obj->next += m->rpod.len * sizeof(struct TestPod);
	}
	if (pb_skipto_2(&p, e, 3562)) {
		m->mb.v = (struct TestMessage_MbEntry*) obj->next;
		do {
			if (obj->next + m->mb.len * sizeof(struct TestMessage_MbEntry) > obj->end) {
				return -1;
			}
			pb_string_t msg;
			p = pb_get_string(p + 2, e, &msg);
			if (pb_get_TestMessage_MbEntry(msg.c_str, msg.c_str + msg.len, (struct TestMessage_MbEntry*) &m->mb.v[m->mb.len])) {
				return -1;
			}
			m->mb.len++;
		} while (!pb_cmp_tag_2(p, e, 3562));

		obj->next += m->mb.len * sizeof(struct TestMessage_MbEntry);
	}
	if (pb_skipto_2(&p, e, 3570)) {
		m->mi32.v = (struct TestMessage_Mi32Entry*) obj->next;
		do {
			if (obj->next + m->mi32.len * sizeof(struct TestMessage_Mi32Entry) > obj->end) {
				return -1;
			}
			pb_string_t msg;
			p = pb_get_string(p + 2, e, &msg);
			if (pb_get_TestMessage_Mi32Entry(msg.c_str, msg.c_str + msg.len, (struct TestMessage_Mi32Entry*) &m->mi32.v[m->mi32.len])) {
				return -1;
			}
			m->mi32.len++;
		} while (!pb_cmp_tag_2(p, e, 3570));

		obj->next += m->mi32.len * sizeof(struct TestMessage_Mi32Entry);
	}
	if (pb_skipto_2(&p, e, 3578)) {
		m->ms32.v = (struct TestMessage_Ms32Entry*) obj->next;
		do {
			if (obj->next + m->ms32.len * sizeof(struct TestMessage_Ms32Entry) > obj->end) {
				return -1;
			}
			pb_string_t msg;
			p = pb_get_string(p + 2, e, &msg);
			if (pb_get_TestMessage_Ms32Entry(msg.c_str, msg.c_str + msg.len, (struct TestMessage_Ms32Entry*) &m->ms32.v[m->ms32.len])) {
				return -1;
			}
			m->ms32.len++;
		} while (!pb_cmp_tag_2(p, e, 3578));

		obj->next += m->ms32.len * sizeof(struct TestMessage_Ms32Entry);
	}
	if (pb_skipto_2(&p, e, 3714)) {
		m->msf32.v = (struct TestMessage_Msf32Entry*) obj->next;
		do {
			if (obj->next + m->msf32.len * sizeof(struct TestMessage_Msf32Entry) > obj->end) {
				return -1;
			}
			pb_string_t msg;
			p = pb_get_string(p + 2, e, &msg);
			if (pb_get_TestMessage_Msf32Entry(msg.c_str, msg.c_str + msg.len, (struct TestMessage_Msf32Entry*) &m->msf32.v[m->msf32.len])) {
				return -1;
			}
			m->msf32.len++;
		} while (!pb_cmp_tag_2(p, e, 3714));

		obj->next += m->msf32.len * sizeof(struct TestMessage_Msf32Entry);
	}
	if (pb_skipto_2(&p, e, 3722)) {
		m->mu32.v = (struct TestMessage_Mu32Entry*) obj->next;
		do {
			if (obj->next + m->mu32.len * sizeof(struct TestMessage_Mu32Entry) > obj->end) {
				return -1;
			}
			pb_string_t msg;
			p = pb_get_string(p + 2, e, &msg);
			if (pb_get_TestMessage_Mu32Entry(msg.c_str, msg.c_str + msg.len, (struct TestMessage_Mu32Entry*) &m->mu32.v[m->mu32.len])) {
				return -1;
			}
			m->mu32.len++;
		} while (!pb_cmp_tag_2(p, e, 3722));

		obj->next += m->mu32.len * sizeof(struct TestMessage_Mu32Entry);
	}
	if (pb_skipto_2(&p, e, 3730)) {
		m->mf32.v = (struct TestMessage_Mf32Entry*) obj->next;
		do {
			if (obj->next + m->mf32.len * sizeof(struct TestMessage_Mf32Entry) > obj->end) {
				return -1;
			}
			pb_string_t msg;
			p = pb_get_string(p + 2, e, &msg);
			if (pb_get_TestMessage_Mf32Entry(msg.c_str, msg.c_str + msg.len, (struct TestMessage_Mf32Entry*) &m->mf32.v[m->mf32.len])) {
				return -1;
			}
			m->mf32.len++;
		} while (!pb_cmp_tag_2(p, e, 3730));

		obj->next += m->mf32.len * sizeof(struct TestMessage_Mf32Entry);
	}
	if (pb_skipto_2(&p, e, 3738)) {
		m->mi64.v = (struct TestMessage_Mi64Entry*) obj->next;
		do {
			if (obj->next + m->mi64.len * sizeof(struct TestMessage_Mi64Entry) > obj->end) {
				return -1;
			}
			pb_string_t msg;
			p = pb_get_string(p + 2, e, &msg);
			if (pb_get_TestMessage_Mi64Entry(msg.c_str, msg.c_str + msg.len, (struct TestMessage_Mi64Entry*) &m->mi64.v[m->mi64.len])) {
				return -1;
			}
			m->mi64.len++;
		} while (!pb_cmp_tag_2(p, e, 3738));

		obj->next += m->mi64.len * sizeof(struct TestMessage_Mi64Entry);
	}
	if (pb_skipto_2(&p, e, 3746)) {
		m->msf64.v = (struct TestMessage_Msf64Entry*) obj->next;
		do {
			if (obj->next + m->msf64.len * sizeof(struct TestMessage_Msf64Entry) > obj->end) {
				return -1;
			}
			pb_string_t msg;
			p = pb_get_string(p + 2, e, &msg);
			if (pb_get_TestMessage_Msf64Entry(msg.c_str, msg.c_str + msg.len, (struct TestMessage_Msf64Entry*) &m->msf64.v[m->msf64.len])) {
				return -1;
			}
			m->msf64.len++;
		} while (!pb_cmp_tag_2(p, e, 3746));

		obj->next += m->msf64.len * sizeof(struct TestMessage_Msf64Entry);
	}
	if (pb_skipto_2(&p, e, 3754)) {
		m->ms64.v = (struct TestMessage_Ms64Entry*) obj->next;
		do {
			if (obj->next + m->ms64.len * sizeof(struct TestMessage_Ms64Entry) > obj->end) {
				return -1;
			}
			pb_string_t msg;
			p = pb_get_string(p + 2, e, &msg);
			if (pb_get_TestMessage_Ms64Entry(msg.c_str, msg.c_str + msg.len, (struct TestMessage_Ms64Entry*) &m->ms64.v[m->ms64.len])) {
				return -1;
			}
			m->ms64.len++;
		} while (!pb_cmp_tag_2(p, e, 3754));

		obj->next += m->ms64.len * sizeof(struct TestMessage_Ms64Entry);
	}
	p = pb_toend_2(p,e);
	if (pb_skipto_3(&p, e, 101010)) {
		m->mu64.v = (struct TestMessage_Mu64Entry*) obj->next;
		do {
			if (obj->next + m->mu64.len * sizeof(struct TestMessage_Mu64Entry) > obj->end) {
				return -1;
			}
			pb_string_t msg;
			p = pb_get_string(p + 3, e, &msg);
			if (pb_get_TestMessage_Mu64Entry(msg.c_str, msg.c_str + msg.len, (struct TestMessage_Mu64Entry*) &m->mu64.v[m->mu64.len])) {
				return -1;
			}
			m->mu64.len++;
		} while (!pb_cmp_tag_3(p, e, 101010));

		obj->next += m->mu64.len * sizeof(struct TestMessage_Mu64Entry);
	}
	if (pb_skipto_3(&p, e, 101018)) {
		m->mf64.v = (struct TestMessage_Mf64Entry*) obj->next;
		do {
			if (obj->next + m->mf64.len * sizeof(struct TestMessage_Mf64Entry) > obj->end) {
				return -1;
			}
			pb_string_t msg;
			p = pb_get_string(p + 3, e, &msg);
			if (pb_get_TestMessage_Mf64Entry(msg.c_str, msg.c_str + msg.len, (struct TestMessage_Mf64Entry*) &m->mf64.v[m->mf64.len])) {
				return -1;
			}
			m->mf64.len++;
		} while (!pb_cmp_tag_3(p, e, 101018));

		obj->next += m->mf64.len * sizeof(struct TestMessage_Mf64Entry);
	}
	if (pb_skipto_3(&p, e, 101026)) {
		m->mf.v = (struct TestMessage_MfEntry*) obj->next;
		do {
			if (obj->next + m->mf.len * sizeof(struct TestMessage_MfEntry) > obj->end) {
				return -1;
			}
			pb_string_t msg;
			p = pb_get_string(p + 3, e, &msg);
			if (pb_get_TestMessage_MfEntry(msg.c_str, msg.c_str + msg.len, (struct TestMessage_MfEntry*) &m->mf.v[m->mf.len])) {
				return -1;
			}
			m->mf.len++;
		} while (!pb_cmp_tag_3(p, e, 101026));

		obj->next += m->mf.len * sizeof(struct TestMessage_MfEntry);
	}
	if (pb_skipto_3(&p, e, 101034)) {
		m->md.v = (struct TestMessage_MdEntry*) obj->next;
		do {
			if (obj->next + m->md.len * sizeof(struct TestMessage_MdEntry) > obj->end) {
				return -1;
			}
			pb_string_t msg;
			p = pb_get_string(p + 3, e, &msg);
			if (pb_get_TestMessage_MdEntry(msg.c_str, msg.c_str + msg.len, (struct TestMessage_MdEntry*) &m->md.v[m->md.len])) {
				return -1;
			}
			m->md.len++;
		} while (!pb_cmp_tag_3(p, e, 101034));

		obj->next += m->md.len * sizeof(struct TestMessage_MdEntry);
	}
	if (pb_skipto_3(&p, e, 101042)) {
		struct TestMessage_MbyEntry *prev = NULL;
		do {
			pb_string_t msg;
			p = pb_get_string(p + 3, e, &msg);
			struct TestMessage_MbyEntry *c = (struct TestMessage_MbyEntry*) pb_calloc(obj, sizeof(struct TestMessage_MbyEntry));
			if (!c || pb_get_TestMessage_MbyEntry(msg.c_str, msg.c_str + msg.len, obj, c)) {
				return -1;
			}
			m->mby.len++;
			c->pb_hdr.prev = prev;
			prev = c;
		} while (!pb_cmp_tag_3(p, e, 101042));

		m->mby.v = (const struct TestMessage_MbyEntry**) pb_calloc(obj, m->mby.len * sizeof(struct TestMessage_MbyEntry*));
		if (!m->mby.v) {
			return -1;
		}

		for (int i = m->mby.len - 1; i >= 0; i--) {
			m->mby.v[i] = prev;
			prev = (struct TestMessage_MbyEntry*) prev->pb_hdr.prev;
		}
	}
	if (pb_skipto_3(&p, e, 101050)) {
		struct TestMessage_MstrEntry *prev = NULL;
		do {
			pb_string_t msg;
			p = pb_get_string(p + 3, e, &msg);
			struct TestMessage_MstrEntry *c = (struct TestMessage_MstrEntry*) pb_calloc(obj, sizeof(struct TestMessage_MstrEntry));
			if (!c || pb_get_TestMessage_MstrEntry(msg.c_str, msg.c_str + msg.len, obj, c)) {
				return -1;
			}
			m->mstr.len++;
			c->pb_hdr.prev = prev;
			prev = c;
		} while (!pb_cmp_tag_3(p, e, 101050));

		m->mstr.v = (const struct TestMessage_MstrEntry**) pb_calloc(obj, m->mstr.len * sizeof(struct TestMessage_MstrEntry*));
		if (!m->mstr.v) {
			return -1;
		}

		for (int i = m->mstr.len - 1; i >= 0; i--) {
			m->mstr.v[i] = prev;
			prev = (struct TestMessage_MstrEntry*) prev->pb_hdr.prev;
		}
	}
	if (pb_skipto_3(&p, e, 101058)) {
		m->men.v = (struct TestMessage_MenEntry*) obj->next;
		do {
			if (obj->next + m->men.len * sizeof(struct TestMessage_MenEntry) > obj->end) {
				return -1;
			}
			pb_string_t msg;
			p = pb_get_string(p + 3, e, &msg);
			if (pb_get_TestMessage_MenEntry(msg.c_str, msg.c_str + msg.len, (struct TestMessage_MenEntry*) &m->men.v[m->men.len])) {
				return -1;
			}
			m->men.len++;
		} while (!pb_cmp_tag_3(p, e, 101058));

		obj->next += m->men.len * sizeof(struct TestMessage_MenEntry);
	}
	if (pb_skipto_3(&p, e, 101066)) {
		struct TestMessage_MmsgEntry *prev = NULL;
		do {
			pb_string_t msg;
			p = pb_get_string(p + 3, e, &msg);
			struct TestMessage_MmsgEntry *c = (struct TestMessage_MmsgEntry*) pb_calloc(obj, sizeof(struct TestMessage_MmsgEntry));
			if (!c || pb_get_TestMessage_MmsgEntry(msg.c_str, msg.c_str + msg.len, obj, c)) {
				return -1;
			}
			m->mmsg.len++;
			c->pb_hdr.prev = prev;
			prev = c;
		} while (!pb_cmp_tag_3(p, e, 101066));

		m->mmsg.v = (const struct TestMessage_MmsgEntry**) pb_calloc(obj, m->mmsg.len * sizeof(struct TestMessage_MmsgEntry*));
		if (!m->mmsg.v) {
			return -1;
		}

		for (int i = m->mmsg.len - 1; i >= 0; i--) {
			m->mmsg.v[i] = prev;
			prev = (struct TestMessage_MmsgEntry*) prev->pb_hdr.prev;
		}
	}
	if (pb_skipto_3(&p, e, 101074)) {
		m->mpod.v = (struct TestMessage_MpodEntry*) obj->next;
		do {
			if (obj->next + m->mpod.len * sizeof(struct TestMessage_MpodEntry) > obj->end) {
				return -1;
			}
			pb_string_t msg;
			p = pb_get_string(p + 3, e, &msg);
			if (pb_get_TestMessage_MpodEntry(msg.c_str, msg.c_str + msg.len, (struct TestMessage_MpodEntry*) &m->mpod.v[m->mpod.len])) {
				return -1;
			}
			m->mpod.len++;
		} while (!pb_cmp_tag_3(p, e, 101074));

		obj->next += m->mpod.len * sizeof(struct TestMessage_MpodEntry);
	}
	return 0;
}
void pb_term_TestMessage(struct TestMessage *m) {
	if (m->str.c_str) {
		((char*)m->str.c_str)[m->str.len] = '\0';
	} else {
		m->str.c_str = "";
	}
	if (m->msg) {
		pb_term_TestMessage((struct TestMessage*) m->msg);
	}
	pb_term_TestPod((struct TestPod*) &m->pod);
	for (int i = 0; i < m->rstr.len; i++) {
		if (m->rstr.v[i].c_str) {
			((char*)m->rstr.v[i].c_str)[m->rstr.v[i].len] = '\0';
		} else {
			((pb_string_t*)m->rstr.v)[i].c_str = "";
		}
	}
	for (int i = 0; i < m->rmsg.len; i++) {
		pb_term_TestMessage((struct TestMessage*) m->rmsg.v[i]);
	}
	for (int i = 0; i < m->rpod.len; i++) {
		pb_term_TestPod((struct TestPod*) &m->rpod.v[i]);
	}
	for (int i = 0; i < m->mb.len; i++) {
		pb_term_TestMessage_MbEntry((struct TestMessage_MbEntry*) &m->mb.v[i]);
	}
	for (int i = 0; i < m->mi32.len; i++) {
		pb_term_TestMessage_Mi32Entry((struct TestMessage_Mi32Entry*) &m->mi32.v[i]);
	}
	for (int i = 0; i < m->ms32.len; i++) {
		pb_term_TestMessage_Ms32Entry((struct TestMessage_Ms32Entry*) &m->ms32.v[i]);
	}
	for (int i = 0; i < m->msf32.len; i++) {
		pb_term_TestMessage_Msf32Entry((struct TestMessage_Msf32Entry*) &m->msf32.v[i]);
	}
	for (int i = 0; i < m->mu32.len; i++) {
		pb_term_TestMessage_Mu32Entry((struct TestMessage_Mu32Entry*) &m->mu32.v[i]);
	}
	for (int i = 0; i < m->mf32.len; i++) {
		pb_term_TestMessage_Mf32Entry((struct TestMessage_Mf32Entry*) &m->mf32.v[i]);
	}
	for (int i = 0; i < m->mi64.len; i++) {
		pb_term_TestMessage_Mi64Entry((struct TestMessage_Mi64Entry*) &m->mi64.v[i]);
	}
	for (int i = 0; i < m->msf64.len; i++) {
		pb_term_TestMessage_Msf64Entry((struct TestMessage_Msf64Entry*) &m->msf64.v[i]);
	}
	for (int i = 0; i < m->ms64.len; i++) {
		pb_term_TestMessage_Ms64Entry((struct TestMessage_Ms64Entry*) &m->ms64.v[i]);
	}
	for (int i = 0; i < m->mu64.len; i++) {
		pb_term_TestMessage_Mu64Entry((struct TestMessage_Mu64Entry*) &m->mu64.v[i]);
	}
	for (int i = 0; i < m->mf64.len; i++) {
		pb_term_TestMessage_Mf64Entry((struct TestMessage_Mf64Entry*) &m->mf64.v[i]);
	}
	for (int i = 0; i < m->mf.len; i++) {
		pb_term_TestMessage_MfEntry((struct TestMessage_MfEntry*) &m->mf.v[i]);
	}
	for (int i = 0; i < m->md.len; i++) {
		pb_term_TestMessage_MdEntry((struct TestMessage_MdEntry*) &m->md.v[i]);
	}
	for (int i = 0; i < m->mby.len; i++) {
		pb_term_TestMessage_MbyEntry((struct TestMessage_MbyEntry*) m->mby.v[i]);
	}
	for (int i = 0; i < m->mstr.len; i++) {
		pb_term_TestMessage_MstrEntry((struct TestMessage_MstrEntry*) m->mstr.v[i]);
	}
	for (int i = 0; i < m->men.len; i++) {
		pb_term_TestMessage_MenEntry((struct TestMessage_MenEntry*) &m->men.v[i]);
	}
	for (int i = 0; i < m->mmsg.len; i++) {
		pb_term_TestMessage_MmsgEntry((struct TestMessage_MmsgEntry*) m->mmsg.v[i]);
	}
	for (int i = 0; i < m->mpod.len; i++) {
		pb_term_TestMessage_MpodEntry((struct TestMessage_MpodEntry*) &m->mpod.v[i]);
	}
}
char *pb_encode_TestMessage(char *p, struct TestMessage const *m) {
	if (m->b) {
		p = pb_put_tag_1(p, 8);
		p = pb_put_bool(p, *(bool*)& m->b);
	}
	if (m->i32) {
		p = pb_put_tag_1(p, 16);
		p = pb_put_u32(p, *(uint32_t*)& m->i32);
	}
	if (m->s32) {
		p = pb_put_tag_1(p, 24);
		p = pb_put_s32(p, *(int32_t*)& m->s32);
	}
	if (m->sf32) {
		p = pb_put_tag_1(p, 33);
		p = pb_put_f32(p, *(union pb_f32*)& m->sf32);
	}
	if (m->u32) {
		p = pb_put_tag_1(p, 40);
		p = pb_put_u32(p, *(uint32_t*)& m->u32);
	}
	if (m->f32) {
		p = pb_put_tag_1(p, 49);
		p = pb_put_f32(p, *(union pb_f32*)& m->f32);
	}
	if (m->i64) {
		p = pb_put_tag_1(p, 56);
		p = pb_put_u64(p, *(uint64_t*)& m->i64);
	}
	if (m->sf64) {
		p = pb_put_tag_1(p, 69);
		p = pb_put_f64(p, *(union pb_f64*)& m->sf64);
	}
	if (m->s64) {
		p = pb_put_tag_1(p, 72);
		p = pb_put_s64(p, *(int64_t*)& m->s64);
	}
	if (m->u64) {
		p = pb_put_tag_1(p, 80);
		p = pb_put_u64(p, *(uint64_t*)& m->u64);
	}
	if (m->f64) {
		p = pb_put_tag_1(p, 93);
		p = pb_put_f64(p, *(union pb_f64*)& m->f64);
	}
	if (m->f) {
		p = pb_put_tag_1(p, 97);
		p = pb_put_f32(p, *(union pb_f32*)& m->f);
	}
	if (m->d) {
		p = pb_put_tag_1(p, 109);
		p = pb_put_f64(p, *(union pb_f64*)& m->d);
	}
	if (m->by.len) {
		p = pb_put_tag_1(p, 114);
		p = pb_put_bytes(p, *(pb_bytes_t*)& m->by);
	}
	if (m->str.len) {
		p = pb_put_tag_1(p, 122);
		p = pb_put_string(p, *(pb_string_t*)& m->str);
	}
	if (m->en) {
		p = pb_put_tag_2(p, 384);
		p = pb_put_u32(p, *(uint32_t*)& m->en);
	}
	if (m->msg) {
		p = pb_put_tag_2(p, 394);
		int szlen = pb_u32_size(m->msg->pb_hdr.maxsz);
		char *start = p+szlen;
		p = pb_encode_TestMessage(start, m->msg);
		pb_put_finish(p, start, szlen);
	}
	if (pb_nonzero_TestPod(&m->pod)) {
		p = pb_put_tag_2(p, 402);
		char *start = p+1;
		p = pb_encode_TestPod(start, &m->pod);
		start[-1] = (char) (p - start);
	}
	if (m->rb.len) {
		p = pb_put_tag_2(p, 426);
		p = pb_put_packed_bool(p, *(bool**)& m->rb.v, m->rb.len);
	}
	if (m->ri32.len) {
		p = pb_put_tag_2(p, 434);
		p = pb_put_packed_u32(p, *(uint32_t**)& m->ri32.v, m->ri32.len);
	}
	if (m->rs32.len) {
		p = pb_put_tag_2(p, 442);
		p = pb_put_packed_s32(p, *(int32_t**)& m->rs32.v, m->rs32.len);
	}
	if (m->rsf32.len) {
		p = pb_put_tag_2(p, 450);
		p = pb_put_packed_f32(p, *(union pb_f32**)& m->rsf32.v, m->rsf32.len);
	}
	if (m->ru32.len) {
		p = pb_put_tag_2(p, 458);
		p = pb_put_packed_u32(p, *(uint32_t**)& m->ru32.v, m->ru32.len);
	}
	if (m->rf32.len) {
		p = pb_put_tag_2(p, 466);
		p = pb_put_packed_f32(p, *(union pb_f32**)& m->rf32.v, m->rf32.len);
	}
	if (m->ri64.len) {
		p = pb_put_tag_2(p, 474);
		p = pb_put_packed_u64(p, *(uint64_t**)& m->ri64.v, m->ri64.len);
	}
	if (m->rsf64.len) {
		p = pb_put_tag_2(p, 482);
		p = pb_put_packed_f64(p, *(union pb_f64**)& m->rsf64.v, m->rsf64.len);
	}
	if (m->rs64.len) {
		p = pb_put_tag_2(p, 490);
		p = pb_put_packed_s64(p, *(int64_t**)& m->rs64.v, m->rs64.len);
	}
	if (m->ru64.len) {
		p = pb_put_tag_2(p, 3474);
		p = pb_put_packed_u64(p, *(uint64_t**)& m->ru64.v, m->ru64.len);
	}
	if (m->rf64.len) {
		p = pb_put_tag_2(p, 3482);
		p = pb_put_packed_f64(p, *(union pb_f64**)& m->rf64.v, m->rf64.len);
	}
	if (m->rf.len) {
		p = pb_put_tag_2(p, 3490);
		p = pb_put_packed_f32(p, *(union pb_f32**)& m->rf.v, m->rf.len);
	}
	if (m->rd.len) {
		p = pb_put_tag_2(p, 3498);
		p = pb_put_packed_f64(p, *(union pb_f64**)& m->rd.v, m->rd.len);
	}
	if (m->rby.len) {
		for (int i = 0; i < m->rby.len; i++) {
			p = pb_put_tag_2(p, 3506);
			p = pb_put_bytes(p, *(pb_bytes_t*)& m->rby.v[i]);
		}
	}
	if (m->rstr.len) {
		for (int i = 0; i < m->rstr.len; i++) {
			p = pb_put_tag_2(p, 3514);
			p = pb_put_string(p, *(pb_string_t*)& m->rstr.v[i]);
		}
	}
	if (m->ren.len) {
		p = pb_put_tag_2(p, 3522);
		p = pb_put_packed_u32(p, *(uint32_t**)& m->ren.v, m->ren.len);
	}
	if (m->rmsg.len) {
		for (int i = 0; i < m->rmsg.len; i++) {
			p = pb_put_tag_2(p, 3530);
			int szlen = pb_u32_size(m->rmsg.v[i]->pb_hdr.maxsz);
			char *start = p+szlen;
			p = pb_encode_TestMessage(start, m->rmsg.v[i]);
			pb_put_finish(p, start, szlen);
		}
	}
	if (m->rpod.len) {
		for (int i = 0; i < m->rpod.len; i++) {
			p = pb_put_tag_2(p, 3538);
			char *start = p+1;
			p = pb_encode_TestPod(start, &m->rpod.v[i]);
			start[-1] = (char) (p - start);
		}
	}
	if (m->mb.len) {
		for (int i = 0; i < m->mb.len; i++) {
			p = pb_put_tag_2(p, 3562);
			char *start = p+1;
			p = pb_encode_TestMessage_MbEntry(start, &m->mb.v[i]);
			start[-1] = (char) (p - start);
		}
	}
	if (m->mi32.len) {
		for (int i = 0; i < m->mi32.len; i++) {
			p = pb_put_tag_2(p, 3570);
			char *start = p+1;
			p = pb_encode_TestMessage_Mi32Entry(start, &m->mi32.v[i]);
			start[-1] = (char) (p - start);
		}
	}
	if (m->ms32.len) {
		for (int i = 0; i < m->ms32.len; i++) {
			p = pb_put_tag_2(p, 3578);
			char *start = p+1;
			p = pb_encode_TestMessage_Ms32Entry(start, &m->ms32.v[i]);
			start[-1] = (char) (p - start);
		}
	}
	if (m->msf32.len) {
		for (int i = 0; i < m->msf32.len; i++) {
			p = pb_put_tag_2(p, 3714);
			char *start = p+1;
			p = pb_encode_TestMessage_Msf32Entry(start, &m->msf32.v[i]);
			start[-1] = (char) (p - start);
		}
	}
	if (m->mu32.len) {
		for (int i = 0; i < m->mu32.len; i++) {
			p = pb_put_tag_2(p, 3722);
			char *start = p+1;
			p = pb_encode_TestMessage_Mu32Entry(start, &m->mu32.v[i]);
			start[-1] = (char) (p - start);
		}
	}
	if (m->mf32.len) {
		for (int i = 0; i < m->mf32.len; i++) {
			p = pb_put_tag_2(p, 3730);
			char *start = p+1;
			p = pb_encode_TestMessage_Mf32Entry(start, &m->mf32.v[i]);
			start[-1] = (char) (p - start);
		}
	}
	if (m->mi64.len) {
		for (int i = 0; i < m->mi64.len; i++) {
			p = pb_put_tag_2(p, 3738);
			char *start = p+1;
			p = pb_encode_TestMessage_Mi64Entry(start, &m->mi64.v[i]);
			start[-1] = (char) (p - start);
		}
	}
	if (m->msf64.len) {
		for (int i = 0; i < m->msf64.len; i++) {
			p = pb_put_tag_2(p, 3746);
			char *start = p+1;
			p = pb_encode_TestMessage_Msf64Entry(start, &m->msf64.v[i]);
			start[-1] = (char) (p - start);
		}
	}
	if (m->ms64.len) {
		for (int i = 0; i < m->ms64.len; i++) {
			p = pb_put_tag_2(p, 3754);
			char *start = p+1;
			p = pb_encode_TestMessage_Ms64Entry(start, &m->ms64.v[i]);
			start[-1] = (char) (p - start);
		}
	}
	if (m->mu64.len) {
		for (int i = 0; i < m->mu64.len; i++) {
			p = pb_put_tag_3(p, 101010);
			char *start = p+1;
			p = pb_encode_TestMessage_Mu64Entry(start, &m->mu64.v[i]);
			start[-1] = (char) (p - start);
		}
	}
	if (m->mf64.len) {
		for (int i = 0; i < m->mf64.len; i++) {
			p = pb_put_tag_3(p, 101018);
			char *start = p+1;
			p = pb_encode_TestMessage_Mf64Entry(start, &m->mf64.v[i]);
			start[-1] = (char) (p - start);
		}
	}
	if (m->mf.len) {
		for (int i = 0; i < m->mf.len; i++) {
			p = pb_put_tag_3(p, 101026);
			char *start = p+1;
			p = pb_encode_TestMessage_MfEntry(start, &m->mf.v[i]);
			start[-1] = (char) (p - start);
		}
	}
	if (m->md.len) {
		for (int i = 0; i < m->md.len; i++) {
			p = pb_put_tag_3(p, 101034);
			char *start = p+1;
			p = pb_encode_TestMessage_MdEntry(start, &m->md.v[i]);
			start[-1] = (char) (p - start);
		}
	}
	if (m->mby.len) {
		for (int i = 0; i < m->mby.len; i++) {
			p = pb_put_tag_3(p, 101042);
			int szlen = pb_u32_size(m->mby.v[i]->pb_hdr.maxsz);
			char *start = p+szlen;
			p = pb_encode_TestMessage_MbyEntry(start, m->mby.v[i]);
			pb_put_finish(p, start, szlen);
		}
	}
	if (m->mstr.len) {
		for (int i = 0; i < m->mstr.len; i++) {
			p = pb_put_tag_3(p, 101050);
			int szlen = pb_u32_size(m->mstr.v[i]->pb_hdr.maxsz);
			char *start = p+szlen;
			p = pb_encode_TestMessage_MstrEntry(start, m->mstr.v[i]);
			pb_put_finish(p, start, szlen);
		}
	}
	if (m->men.len) {
		for (int i = 0; i < m->men.len; i++) {
			p = pb_put_tag_3(p, 101058);
			char *start = p+1;
			p = pb_encode_TestMessage_MenEntry(start, &m->men.v[i]);
			start[-1] = (char) (p - start);
		}
	}
	if (m->mmsg.len) {
		for (int i = 0; i < m->mmsg.len; i++) {
			p = pb_put_tag_3(p, 101066);
			int szlen = pb_u32_size(m->mmsg.v[i]->pb_hdr.maxsz);
			char *start = p+szlen;
			p = pb_encode_TestMessage_MmsgEntry(start, m->mmsg.v[i]);
			pb_put_finish(p, start, szlen);
		}
	}
	if (m->mpod.len) {
		for (int i = 0; i < m->mpod.len; i++) {
			p = pb_put_tag_3(p, 101074);
			char *start = p+1;
			p = pb_encode_TestMessage_MpodEntry(start, &m->mpod.v[i]);
			start[-1] = (char) (p - start);
		}
	}
	return p;
}
int pb_maxsz_TestMessage(struct TestMessage const *m) {
	int n = 0;
	if (m->b) {
		n += 1 + 1;
	}
	if (m->i32) {
		n += 1 + 5;
	}
	if (m->s32) {
		n += 1 + 5;
	}
	if (m->sf32) {
		n += 1 + 4;
	}
	if (m->u32) {
		n += 1 + 5;
	}
	if (m->f32) {
		n += 1 + 4;
	}
	if (m->i64) {
		n += 1 + 10;
	}
	if (m->sf64) {
		n += 1 + 8;
	}
	if (m->s64) {
		n += 1 + 10;
	}
	if (m->u64) {
		n += 1 + 10;
	}
	if (m->f64) {
		n += 1 + 8;
	}
	if (m->f) {
		n += 1 + 4;
	}
	if (m->d) {
		n += 1 + 8;
	}
	if (m->by.len) {
		n += 1 + 5 + m->by.len;
	}
	if (m->str.len) {
		n += 1 + 5 + m->str.len;
	}
	if (m->en) {
		n += 2 + 5;
	}
	if (m->msg) {
		n += 2 + pb_maxsz_TestMessage(m->msg);
	}
	if (pb_nonzero_TestPod(&m->pod)) {
		n += 2 + 13;
	}
	if (m->rb.len) {
		n += 2 + (1 * m->rb.len);
	}
	if (m->ri32.len) {
		n += 2 + (5 * m->ri32.len);
	}
	if (m->rs32.len) {
		n += 2 + (5 * m->rs32.len);
	}
	if (m->rsf32.len) {
		n += 2 + (4 * m->rsf32.len);
	}
	if (m->ru32.len) {
		n += 2 + (5 * m->ru32.len);
	}
	if (m->rf32.len) {
		n += 2 + (4 * m->rf32.len);
	}
	if (m->ri64.len) {
		n += 2 + (10 * m->ri64.len);
	}
	if (m->rsf64.len) {
		n += 2 + (8 * m->rsf64.len);
	}
	if (m->rs64.len) {
		n += 2 + (10 * m->rs64.len);
	}
	if (m->ru64.len) {
		n += 2 + (10 * m->ru64.len);
	}
	if (m->rf64.len) {
		n += 2 + (8 * m->rf64.len);
	}
	if (m->rf.len) {
		n += 2 + (4 * m->rf.len);
	}
	if (m->rd.len) {
		n += 2 + (8 * m->rd.len);
	}
	if (m->rby.len) {
		for (int i = 0; i < m->rby.len; i++) {
			n += 2 + 5 + m->rby.v[i].len;
		}
	}
	if (m->rstr.len) {
		for (int i = 0; i < m->rstr.len; i++) {
			n += 2 + 5 + m->rstr.v[i].len;
		}
	}
	if (m->ren.len) {
		n += 2 + (5 * m->ren.len);
	}
	if (m->rmsg.len) {
		for (int i = 0; i < m->rmsg.len; i++) {
			n += 2 + pb_maxsz_TestMessage(m->rmsg.v[i]);
		}
	}
	if (m->rpod.len) {
		n += (2 + 13) * m->rpod.len;
	}
	if (m->mb.len) {
		n += (2 + 9) * m->mb.len;
	}
	if (m->mi32.len) {
		n += (2 + 13) * m->mi32.len;
	}
	if (m->ms32.len) {
		n += (2 + 13) * m->ms32.len;
	}
	if (m->msf32.len) {
		n += (2 + 12) * m->msf32.len;
	}
	if (m->mu32.len) {
		n += (2 + 13) * m->mu32.len;
	}
	if (m->mf32.len) {
		n += (2 + 12) * m->mf32.len;
	}
	if (m->mi64.len) {
		n += (2 + 18) * m->mi64.len;
	}
	if (m->msf64.len) {
		n += (2 + 16) * m->msf64.len;
	}
	if (m->ms64.len) {
		n += (2 + 18) * m->ms64.len;
	}
	if (m->mu64.len) {
		n += (3 + 18) * m->mu64.len;
	}
	if (m->mf64.len) {
		n += (3 + 16) * m->mf64.len;
	}
	if (m->mf.len) {
		n += (3 + 12) * m->mf.len;
	}
	if (m->md.len) {
		n += (3 + 16) * m->md.len;
	}
	if (m->mby.len) {
		for (int i = 0; i < m->mby.len; i++) {
			n += 3 + pb_maxsz_TestMessage_MbyEntry(m->mby.v[i]);
		}
	}
	if (m->mstr.len) {
		for (int i = 0; i < m->mstr.len; i++) {
			n += 3 + pb_maxsz_TestMessage_MstrEntry(m->mstr.v[i]);
		}
	}
	if (m->men.len) {
		n += (3 + 13) * m->men.len;
	}
	if (m->mmsg.len) {
		for (int i = 0; i < m->mmsg.len; i++) {
			n += 3 + pb_maxsz_TestMessage_MmsgEntry(m->mmsg.v[i]);
		}
	}
	if (m->mpod.len) {
		n += (3 + 21) * m->mpod.len;
	}
	n += pb_u32_size(n);
	((union pb_msg*) m)->maxsz = n;
	return n;
}

bool pb_nonzero_TestMessage_MbEntry(struct TestMessage_MbEntry const *m) {
	return false
	    || m->key
	    || m->value;
}
const char *pb_parse_TestMessage_MbEntry(const char *p, struct TestMessage_MbEntry *m) {
	(void) m;
	if (!pb_parse_map(&p)) {
		return p;
	}
	for (;;) {
		pb_string_t key;
		switch (pb_parse_field(&p, &key, 4) % 5) {
		case 0:
			return p;
		case 2:
			if (pb_cmp(key, "key")) {
				goto unknown;
			}
			p = pb_parse_u32(p, &m->key);
			continue;
		case 3:
			if (pb_cmp(key, "value")) {
				goto unknown;
			}
			p = pb_parse_bool(p, &m->value);
			continue;
		default:
		unknown:
			p = pb_parse_skip(p);
		}
	}
}
int pb_print_TestMessage_MbEntry(pb_buf_t *a, struct TestMessage_MbEntry const *m) {
	char *p = pb_appendsz(a, 33);
	if (!p) {return -1;}
	*p++ = '{';
	if (m->key) {
		memcpy(p, "\"key\":", 6);
		p += 6;
		p = pb_print_u32(p, m->key);
	}
	if (m->value) {
		memcpy(p, "\"value\":", 8);
		p += 8;
		p = pb_print_bool(p, m->value);
	}
	a->next = p;
	return pb_print_map_end(a);
}
int pb_get_TestMessage_MbEntry(const char *p, const char *e, struct TestMessage_MbEntry *m) {
	if (pb_skipto_1(&p, e, 8)) {
		p = pb_get_u32(p + 1, e, (uint32_t*)&m->key);
	}
	if (pb_skipto_1(&p, e, 16)) {
		p = pb_get_bool(p + 1, e, (bool*)&m->value);
	}
	return 0;
}
void pb_term_TestMessage_MbEntry(struct TestMessage_MbEntry *m) {
}
char *pb_encode_TestMessage_MbEntry(char *p, struct TestMessage_MbEntry const *m) {
	if (m->key) {
		p = pb_put_tag_1(p, 8);
		p = pb_put_u32(p, *(uint32_t*)& m->key);
	}
	if (m->value) {
		p = pb_put_tag_1(p, 16);
		p = pb_put_bool(p, *(bool*)& m->value);
	}
	return p;
}
int pb_maxsz_TestMessage_MbEntry(struct TestMessage_MbEntry const *m) {
	int n = 0;
	if (m->key) {
		n += 1 + 5;
	}
	if (m->value) {
		n += 1 + 1;
	}
	n += pb_u32_size(n);
	((union pb_msg*) m)->maxsz = n;
	return n;
}

bool pb_nonzero_TestMessage_Mu32Entry(struct TestMessage_Mu32Entry const *m) {
	return false
	    || m->key
	    || m->value;
}
const char *pb_parse_TestMessage_Mu32Entry(const char *p, struct TestMessage_Mu32Entry *m) {
	(void) m;
	if (!pb_parse_map(&p)) {
		return p;
	}
	for (;;) {
		pb_string_t key;
		switch (pb_parse_field(&p, &key, 4) % 5) {
		case 0:
			return p;
		case 2:
			if (pb_cmp(key, "key")) {
				goto unknown;
			}
			p = pb_parse_u32(p, &m->key);
			continue;
		case 3:
			if (pb_cmp(key, "value")) {
				goto unknown;
			}
			p = pb_parse_u32(p, &m->value);
			continue;
		default:
		unknown:
			p = pb_parse_skip(p);
		}
	}
}
int pb_print_TestMessage_Mu32Entry(pb_buf_t *a, struct TestMessage_Mu32Entry const *m) {
	char *p = pb_appendsz(a, 39);
	if (!p) {return -1;}
	*p++ = '{';
	if (m->key) {
		memcpy(p, "\"key\":", 6);
		p += 6;
		p = pb_print_u32(p, m->key);
	}
	if (m->value) {
		memcpy(p, "\"value\":", 8);
		p += 8;
		p = pb_print_u32(p, m->value);
	}
	a->next = p;
	return pb_print_map_end(a);
}
int pb_get_TestMessage_Mu32Entry(const char *p, const char *e, struct TestMessage_Mu32Entry *m) {
	if (pb_skipto_1(&p, e, 8)) {
		p = pb_get_u32(p + 1, e, (uint32_t*)&m->key);
	}
	if (pb_skipto_1(&p, e, 16)) {
		p = pb_get_u32(p + 1, e, (uint32_t*)&m->value);
	}
	return 0;
}
void pb_term_TestMessage_Mu32Entry(struct TestMessage_Mu32Entry *m) {
}
char *pb_encode_TestMessage_Mu32Entry(char *p, struct TestMessage_Mu32Entry const *m) {
	if (m->key) {
		p = pb_put_tag_1(p, 8);
		p = pb_put_u32(p, *(uint32_t*)& m->key);
	}
	if (m->value) {
		p = pb_put_tag_1(p, 16);
		p = pb_put_u32(p, *(uint32_t*)& m->value);
	}
	return p;
}
int pb_maxsz_TestMessage_Mu32Entry(struct TestMessage_Mu32Entry const *m) {
	int n = 0;
	if (m->key) {
		n += 1 + 5;
	}
	if (m->value) {
		n += 1 + 5;
	}
	n += pb_u32_size(n);
	((union pb_msg*) m)->maxsz = n;
	return n;
}

bool pb_nonzero_TestMessage_Mu64Entry(struct TestMessage_Mu64Entry const *m) {
	return false
	    || m->key
	    || m->value;
}
const char *pb_parse_TestMessage_Mu64Entry(const char *p, struct TestMessage_Mu64Entry *m) {
	(void) m;
	if (!pb_parse_map(&p)) {
		return p;
	}
	for (;;) {
		pb_string_t key;
		switch (pb_parse_field(&p, &key, 4) % 5) {
		case 0:
			return p;
		case 2:
			if (pb_cmp(key, "key")) {
				goto unknown;
			}
			p = pb_parse_u32(p, &m->key);
			continue;
		case 3:
			if (pb_cmp(key, "value")) {
				goto unknown;
			}
			p = pb_parse_u64(p, &m->value);
			continue;
		default:
		unknown:
			p = pb_parse_skip(p);
		}
	}
}
int pb_print_TestMessage_Mu64Entry(pb_buf_t *a, struct TestMessage_Mu64Entry const *m) {
	char *p = pb_appendsz(a, 51);
	if (!p) {return -1;}
	*p++ = '{';
	if (m->key) {
		memcpy(p, "\"key\":", 6);
		p += 6;
		p = pb_print_u32(p, m->key);
	}
	if (m->value) {
		memcpy(p, "\"value\":", 8);
		p += 8;
		p = pb_print_u64(p, m->value);
	}
	a->next = p;
	return pb_print_map_end(a);
}
int pb_get_TestMessage_Mu64Entry(const char *p, const char *e, struct TestMessage_Mu64Entry *m) {
	if (pb_skipto_1(&p, e, 8)) {
		p = pb_get_u32(p + 1, e, (uint32_t*)&m->key);
	}
	if (pb_skipto_1(&p, e, 16)) {
		p = pb_get_u64(p + 1, e, (uint64_t*)&m->value);
	}
	return 0;
}
void pb_term_TestMessage_Mu64Entry(struct TestMessage_Mu64Entry *m) {
}
char *pb_encode_TestMessage_Mu64Entry(char *p, struct TestMessage_Mu64Entry const *m) {
	if (m->key) {
		p = pb_put_tag_1(p, 8);
		p = pb_put_u32(p, *(uint32_t*)& m->key);
	}
	if (m->value) {
		p = pb_put_tag_1(p, 16);
		p = pb_put_u64(p, *(uint64_t*)& m->value);
	}
	return p;
}
int pb_maxsz_TestMessage_Mu64Entry(struct TestMessage_Mu64Entry const *m) {
	int n = 0;
	if (m->key) {
		n += 1 + 5;
	}
	if (m->value) {
		n += 1 + 10;
	}
	n += pb_u32_size(n);
	((union pb_msg*) m)->maxsz = n;
	return n;
}

bool pb_nonzero_TestMessage_Mi32Entry(struct TestMessage_Mi32Entry const *m) {
	return false
	    || m->key
	    || m->value;
}
const char *pb_parse_TestMessage_Mi32Entry(const char *p, struct TestMessage_Mi32Entry *m) {
	(void) m;
	if (!pb_parse_map(&p)) {
		return p;
	}
	for (;;) {
		pb_string_t key;
		switch (pb_parse_field(&p, &key, 4) % 5) {
		case 0:
			return p;
		case 2:
			if (pb_cmp(key, "key")) {
				goto unknown;
			}
			p = pb_parse_u32(p, &m->key);
			continue;
		case 3:
			if (pb_cmp(key, "value")) {
				goto unknown;
			}
			p = pb_parse_i32(p, &m->value);
			continue;
		default:
		unknown:
			p = pb_parse_skip(p);
		}
	}
}
int pb_print_TestMessage_Mi32Entry(pb_buf_t *a, struct TestMessage_Mi32Entry const *m) {
	char *p = pb_appendsz(a, 39);
	if (!p) {return -1;}
	*p++ = '{';
	if (m->key) {
		memcpy(p, "\"key\":", 6);
		p += 6;
		p = pb_print_u32(p, m->key);
	}
	if (m->value) {
		memcpy(p, "\"value\":", 8);
		p += 8;
		p = pb_print_i32(p, m->value);
	}
	a->next = p;
	return pb_print_map_end(a);
}
int pb_get_TestMessage_Mi32Entry(const char *p, const char *e, struct TestMessage_Mi32Entry *m) {
	if (pb_skipto_1(&p, e, 8)) {
		p = pb_get_u32(p + 1, e, (uint32_t*)&m->key);
	}
	if (pb_skipto_1(&p, e, 16)) {
		p = pb_get_u32(p + 1, e, (uint32_t*)&m->value);
	}
	return 0;
}
void pb_term_TestMessage_Mi32Entry(struct TestMessage_Mi32Entry *m) {
}
char *pb_encode_TestMessage_Mi32Entry(char *p, struct TestMessage_Mi32Entry const *m) {
	if (m->key) {
		p = pb_put_tag_1(p, 8);
		p = pb_put_u32(p, *(uint32_t*)& m->key);
	}
	if (m->value) {
		p = pb_put_tag_1(p, 16);
		p = pb_put_u32(p, *(uint32_t*)& m->value);
	}
	return p;
}
int pb_maxsz_TestMessage_Mi32Entry(struct TestMessage_Mi32Entry const *m) {
	int n = 0;
	if (m->key) {
		n += 1 + 5;
	}
	if (m->value) {
		n += 1 + 5;
	}
	n += pb_u32_size(n);
	((union pb_msg*) m)->maxsz = n;
	return n;
}

bool pb_nonzero_TestMessage_Mi64Entry(struct TestMessage_Mi64Entry const *m) {
	return false
	    || m->key
	    || m->value;
}
const char *pb_parse_TestMessage_Mi64Entry(const char *p, struct TestMessage_Mi64Entry *m) {
	(void) m;
	if (!pb_parse_map(&p)) {
		return p;
	}
	for (;;) {
		pb_string_t key;
		switch (pb_parse_field(&p, &key, 4) % 5) {
		case 0:
			return p;
		case 2:
			if (pb_cmp(key, "key")) {
				goto unknown;
			}
			p = pb_parse_u32(p, &m->key);
			continue;
		case 3:
			if (pb_cmp(key, "value")) {
				goto unknown;
			}
			p = pb_parse_i64(p, &m->value);
			continue;
		default:
		unknown:
			p = pb_parse_skip(p);
		}
	}
}
int pb_print_TestMessage_Mi64Entry(pb_buf_t *a, struct TestMessage_Mi64Entry const *m) {
	char *p = pb_appendsz(a, 51);
	if (!p) {return -1;}
	*p++ = '{';
	if (m->key) {
		memcpy(p, "\"key\":", 6);
		p += 6;
		p = pb_print_u32(p, m->key);
	}
	if (m->value) {
		memcpy(p, "\"value\":", 8);
		p += 8;
		p = pb_print_i64(p, m->value);
	}
	a->next = p;
	return pb_print_map_end(a);
}
int pb_get_TestMessage_Mi64Entry(const char *p, const char *e, struct TestMessage_Mi64Entry *m) {
	if (pb_skipto_1(&p, e, 8)) {
		p = pb_get_u32(p + 1, e, (uint32_t*)&m->key);
	}
	if (pb_skipto_1(&p, e, 16)) {
		p = pb_get_u64(p + 1, e, (uint64_t*)&m->value);
	}
	return 0;
}
void pb_term_TestMessage_Mi64Entry(struct TestMessage_Mi64Entry *m) {
}
char *pb_encode_TestMessage_Mi64Entry(char *p, struct TestMessage_Mi64Entry const *m) {
	if (m->key) {
		p = pb_put_tag_1(p, 8);
		p = pb_put_u32(p, *(uint32_t*)& m->key);
	}
	if (m->value) {
		p = pb_put_tag_1(p, 16);
		p = pb_put_u64(p, *(uint64_t*)& m->value);
	}
	return p;
}
int pb_maxsz_TestMessage_Mi64Entry(struct TestMessage_Mi64Entry const *m) {
	int n = 0;
	if (m->key) {
		n += 1 + 5;
	}
	if (m->value) {
		n += 1 + 10;
	}
	n += pb_u32_size(n);
	((union pb_msg*) m)->maxsz = n;
	return n;
}

bool pb_nonzero_TestMessage_Ms32Entry(struct TestMessage_Ms32Entry const *m) {
	return false
	    || m->key
	    || m->value;
}
const char *pb_parse_TestMessage_Ms32Entry(const char *p, struct TestMessage_Ms32Entry *m) {
	(void) m;
	if (!pb_parse_map(&p)) {
		return p;
	}
	for (;;) {
		pb_string_t key;
		switch (pb_parse_field(&p, &key, 4) % 5) {
		case 0:
			return p;
		case 2:
			if (pb_cmp(key, "key")) {
				goto unknown;
			}
			p = pb_parse_u32(p, &m->key);
			continue;
		case 3:
			if (pb_cmp(key, "value")) {
				goto unknown;
			}
			p = pb_parse_i32(p, &m->value);
			continue;
		default:
		unknown:
			p = pb_parse_skip(p);
		}
	}
}
int pb_print_TestMessage_Ms32Entry(pb_buf_t *a, struct TestMessage_Ms32Entry const *m) {
	char *p = pb_appendsz(a, 39);
	if (!p) {return -1;}
	*p++ = '{';
	if (m->key) {
		memcpy(p, "\"key\":", 6);
		p += 6;
		p = pb_print_u32(p, m->key);
	}
	if (m->value) {
		memcpy(p, "\"value\":", 8);
		p += 8;
		p = pb_print_i32(p, m->value);
	}
	a->next = p;
	return pb_print_map_end(a);
}
int pb_get_TestMessage_Ms32Entry(const char *p, const char *e, struct TestMessage_Ms32Entry *m) {
	if (pb_skipto_1(&p, e, 8)) {
		p = pb_get_u32(p + 1, e, (uint32_t*)&m->key);
	}
	if (pb_skipto_1(&p, e, 16)) {
		p = pb_get_s32(p + 1, e, (int32_t*)&m->value);
	}
	return 0;
}
void pb_term_TestMessage_Ms32Entry(struct TestMessage_Ms32Entry *m) {
}
char *pb_encode_TestMessage_Ms32Entry(char *p, struct TestMessage_Ms32Entry const *m) {
	if (m->key) {
		p = pb_put_tag_1(p, 8);
		p = pb_put_u32(p, *(uint32_t*)& m->key);
	}
	if (m->value) {
		p = pb_put_tag_1(p, 16);
		p = pb_put_s32(p, *(int32_t*)& m->value);
	}
	return p;
}
int pb_maxsz_TestMessage_Ms32Entry(struct TestMessage_Ms32Entry const *m) {
	int n = 0;
	if (m->key) {
		n += 1 + 5;
	}
	if (m->value) {
		n += 1 + 5;
	}
	n += pb_u32_size(n);
	((union pb_msg*) m)->maxsz = n;
	return n;
}

bool pb_nonzero_TestMessage_Ms64Entry(struct TestMessage_Ms64Entry const *m) {
	return false
	    || m->key
	    || m->value;
}
const char *pb_parse_TestMessage_Ms64Entry(const char *p, struct TestMessage_Ms64Entry *m) {
	(void) m;
	if (!pb_parse_map(&p)) {
		return p;
	}
	for (;;) {
		pb_string_t key;
		switch (pb_parse_field(&p, &key, 4) % 5) {
		case 0:
			return p;
		case 2:
			if (pb_cmp(key, "key")) {
				goto unknown;
			}
			p = pb_parse_u32(p, &m->key);
			continue;
		case 3:
			if (pb_cmp(key, "value")) {
				goto unknown;
			}
			p = pb_parse_i64(p, &m->value);
			continue;
		default:
		unknown:
			p = pb_parse_skip(p);
		}
	}
}
int pb_print_TestMessage_Ms64Entry(pb_buf_t *a, struct TestMessage_Ms64Entry const *m) {
	char *p = pb_appendsz(a, 51);
	if (!p) {return -1;}
	*p++ = '{';
	if (m->key) {
		memcpy(p, "\"key\":", 6);
		p += 6;
		p = pb_print_u32(p, m->key);
	}
	if (m->value) {
		memcpy(p, "\"value\":", 8);
		p += 8;
		p = pb_print_i64(p, m->value);
	}
	a->next = p;
	return pb_print_map_end(a);
}
int pb_get_TestMessage_Ms64Entry(const char *p, const char *e, struct TestMessage_Ms64Entry *m) {
	if (pb_skipto_1(&p, e, 8)) {
		p = pb_get_u32(p + 1, e, (uint32_t*)&m->key);
	}
	if (pb_skipto_1(&p, e, 16)) {
		p = pb_get_s64(p + 1, e, (int64_t*)&m->value);
	}
	return 0;
}
void pb_term_TestMessage_Ms64Entry(struct TestMessage_Ms64Entry *m) {
}
char *pb_encode_TestMessage_Ms64Entry(char *p, struct TestMessage_Ms64Entry const *m) {
	if (m->key) {
		p = pb_put_tag_1(p, 8);
		p = pb_put_u32(p, *(uint32_t*)& m->key);
	}
	if (m->value) {
		p = pb_put_tag_1(p, 16);
		p = pb_put_s64(p, *(int64_t*)& m->value);
	}
	return p;
}
int pb_maxsz_TestMessage_Ms64Entry(struct TestMessage_Ms64Entry const *m) {
	int n = 0;
	if (m->key) {
		n += 1 + 5;
	}
	if (m->value) {
		n += 1 + 10;
	}
	n += pb_u32_size(n);
	((union pb_msg*) m)->maxsz = n;
	return n;
}

bool pb_nonzero_TestMessage_Mf32Entry(struct TestMessage_Mf32Entry const *m) {
	return false
	    || m->key
	    || m->value;
}
const char *pb_parse_TestMessage_Mf32Entry(const char *p, struct TestMessage_Mf32Entry *m) {
	(void) m;
	if (!pb_parse_map(&p)) {
		return p;
	}
	for (;;) {
		pb_string_t key;
		switch (pb_parse_field(&p, &key, 4) % 5) {
		case 0:
			return p;
		case 2:
			if (pb_cmp(key, "key")) {
				goto unknown;
			}
			p = pb_parse_u32(p, &m->key);
			continue;
		case 3:
			if (pb_cmp(key, "value")) {
				goto unknown;
			}
			p = pb_parse_u32(p, &m->value);
			continue;
		default:
		unknown:
			p = pb_parse_skip(p);
		}
	}
}
int pb_print_TestMessage_Mf32Entry(pb_buf_t *a, struct TestMessage_Mf32Entry const *m) {
	char *p = pb_appendsz(a, 39);
	if (!p) {return -1;}
	*p++ = '{';
	if (m->key) {
		memcpy(p, "\"key\":", 6);
		p += 6;
		p = pb_print_u32(p, m->key);
	}
	if (m->value) {
		memcpy(p, "\"value\":", 8);
		p += 8;
		p = pb_print_u32(p, m->value);
	}
	a->next = p;
	return pb_print_map_end(a);
}
int pb_get_TestMessage_Mf32Entry(const char *p, const char *e, struct TestMessage_Mf32Entry *m) {
	if (pb_skipto_1(&p, e, 8)) {
		p = pb_get_u32(p + 1, e, (uint32_t*)&m->key);
	}
	if (pb_skipto_1(&p, e, 17)) {
		p = pb_get_f32(p + 1, e, (union pb_f32*)&m->value);
	}
	return 0;
}
void pb_term_TestMessage_Mf32Entry(struct TestMessage_Mf32Entry *m) {
}
char *pb_encode_TestMessage_Mf32Entry(char *p, struct TestMessage_Mf32Entry const *m) {
	if (m->key) {
		p = pb_put_tag_1(p, 8);
		p = pb_put_u32(p, *(uint32_t*)& m->key);
	}
	if (m->value) {
		p = pb_put_tag_1(p, 17);
		p = pb_put_f32(p, *(union pb_f32*)& m->value);
	}
	return p;
}
int pb_maxsz_TestMessage_Mf32Entry(struct TestMessage_Mf32Entry const *m) {
	int n = 0;
	if (m->key) {
		n += 1 + 5;
	}
	if (m->value) {
		n += 1 + 4;
	}
	n += pb_u32_size(n);
	((union pb_msg*) m)->maxsz = n;
	return n;
}

bool pb_nonzero_TestMessage_Mf64Entry(struct TestMessage_Mf64Entry const *m) {
	return false
	    || m->key
	    || m->value;
}
const char *pb_parse_TestMessage_Mf64Entry(const char *p, struct TestMessage_Mf64Entry *m) {
	(void) m;
	if (!pb_parse_map(&p)) {
		return p;
	}
	for (;;) {
		pb_string_t key;
		switch (pb_parse_field(&p, &key, 4) % 5) {
		case 0:
			return p;
		case 2:
			if (pb_cmp(key, "key")) {
				goto unknown;
			}
			p = pb_parse_u32(p, &m->key);
			continue;
		case 3:
			if (pb_cmp(key, "value")) {
				goto unknown;
			}
			p = pb_parse_u64(p, &m->value);
			continue;
		default:
		unknown:
			p = pb_parse_skip(p);
		}
	}
}
int pb_print_TestMessage_Mf64Entry(pb_buf_t *a, struct TestMessage_Mf64Entry const *m) {
	char *p = pb_appendsz(a, 51);
	if (!p) {return -1;}
	*p++ = '{';
	if (m->key) {
		memcpy(p, "\"key\":", 6);
		p += 6;
		p = pb_print_u32(p, m->key);
	}
	if (m->value) {
		memcpy(p, "\"value\":", 8);
		p += 8;
		p = pb_print_u64(p, m->value);
	}
	a->next = p;
	return pb_print_map_end(a);
}
int pb_get_TestMessage_Mf64Entry(const char *p, const char *e, struct TestMessage_Mf64Entry *m) {
	if (pb_skipto_1(&p, e, 8)) {
		p = pb_get_u32(p + 1, e, (uint32_t*)&m->key);
	}
	if (pb_skipto_1(&p, e, 21)) {
		p = pb_get_f64(p + 1, e, (union pb_f64*)&m->value);
	}
	return 0;
}
void pb_term_TestMessage_Mf64Entry(struct TestMessage_Mf64Entry *m) {
}
char *pb_encode_TestMessage_Mf64Entry(char *p, struct TestMessage_Mf64Entry const *m) {
	if (m->key) {
		p = pb_put_tag_1(p, 8);
		p = pb_put_u32(p, *(uint32_t*)& m->key);
	}
	if (m->value) {
		p = pb_put_tag_1(p, 21);
		p = pb_put_f64(p, *(union pb_f64*)& m->value);
	}
	return p;
}
int pb_maxsz_TestMessage_Mf64Entry(struct TestMessage_Mf64Entry const *m) {
	int n = 0;
	if (m->key) {
		n += 1 + 5;
	}
	if (m->value) {
		n += 1 + 8;
	}
	n += pb_u32_size(n);
	((union pb_msg*) m)->maxsz = n;
	return n;
}

bool pb_nonzero_TestMessage_Msf32Entry(struct TestMessage_Msf32Entry const *m) {
	return false
	    || m->key
	    || m->value;
}
const char *pb_parse_TestMessage_Msf32Entry(const char *p, struct TestMessage_Msf32Entry *m) {
	(void) m;
	if (!pb_parse_map(&p)) {
		return p;
	}
	for (;;) {
		pb_string_t key;
		switch (pb_parse_field(&p, &key, 4) % 5) {
		case 0:
			return p;
		case 2:
			if (pb_cmp(key, "key")) {
				goto unknown;
			}
			p = pb_parse_u32(p, &m->key);
			continue;
		case 3:
			if (pb_cmp(key, "value")) {
				goto unknown;
			}
			p = pb_parse_i32(p, &m->value);
			continue;
		default:
		unknown:
			p = pb_parse_skip(p);
		}
	}
}
int pb_print_TestMessage_Msf32Entry(pb_buf_t *a, struct TestMessage_Msf32Entry const *m) {
	char *p = pb_appendsz(a, 39);
	if (!p) {return -1;}
	*p++ = '{';
	if (m->key) {
		memcpy(p, "\"key\":", 6);
		p += 6;
		p = pb_print_u32(p, m->key);
	}
	if (m->value) {
		memcpy(p, "\"value\":", 8);
		p += 8;
		p = pb_print_i32(p, m->value);
	}
	a->next = p;
	return pb_print_map_end(a);
}
int pb_get_TestMessage_Msf32Entry(const char *p, const char *e, struct TestMessage_Msf32Entry *m) {
	if (pb_skipto_1(&p, e, 8)) {
		p = pb_get_u32(p + 1, e, (uint32_t*)&m->key);
	}
	if (pb_skipto_1(&p, e, 17)) {
		p = pb_get_f32(p + 1, e, (union pb_f32*)&m->value);
	}
	return 0;
}
void pb_term_TestMessage_Msf32Entry(struct TestMessage_Msf32Entry *m) {
}
char *pb_encode_TestMessage_Msf32Entry(char *p, struct TestMessage_Msf32Entry const *m) {
	if (m->key) {
		p = pb_put_tag_1(p, 8);
		p = pb_put_u32(p, *(uint32_t*)& m->key);
	}
	if (m->value) {
		p = pb_put_tag_1(p, 17);
		p = pb_put_f32(p, *(union pb_f32*)& m->value);
	}
	return p;
}
int pb_maxsz_TestMessage_Msf32Entry(struct TestMessage_Msf32Entry const *m) {
	int n = 0;
	if (m->key) {
		n += 1 + 5;
	}
	if (m->value) {
		n += 1 + 4;
	}
	n += pb_u32_size(n);
	((union pb_msg*) m)->maxsz = n;
	return n;
}

bool pb_nonzero_TestMessage_Msf64Entry(struct TestMessage_Msf64Entry const *m) {
	return false
	    || m->key
	    || m->value;
}
const char *pb_parse_TestMessage_Msf64Entry(const char *p, struct TestMessage_Msf64Entry *m) {
	(void) m;
	if (!pb_parse_map(&p)) {
		return p;
	}
	for (;;) {
		pb_string_t key;
		switch (pb_parse_field(&p, &key, 4) % 5) {
		case 0:
			return p;
		case 2:
			if (pb_cmp(key, "key")) {
				goto unknown;
			}
			p = pb_parse_u32(p, &m->key);
			continue;
		case 3:
			if (pb_cmp(key, "value")) {
				goto unknown;
			}
			p = pb_parse_i64(p, &m->value);
			continue;
		default:
		unknown:
			p = pb_parse_skip(p);
		}
	}
}
int pb_print_TestMessage_Msf64Entry(pb_buf_t *a, struct TestMessage_Msf64Entry const *m) {
	char *p = pb_appendsz(a, 51);
	if (!p) {return -1;}
	*p++ = '{';
	if (m->key) {
		memcpy(p, "\"key\":", 6);
		p += 6;
		p = pb_print_u32(p, m->key);
	}
	if (m->value) {
		memcpy(p, "\"value\":", 8);
		p += 8;
		p = pb_print_i64(p, m->value);
	}
	a->next = p;
	return pb_print_map_end(a);
}
int pb_get_TestMessage_Msf64Entry(const char *p, const char *e, struct TestMessage_Msf64Entry *m) {
	if (pb_skipto_1(&p, e, 8)) {
		p = pb_get_u32(p + 1, e, (uint32_t*)&m->key);
	}
	if (pb_skipto_1(&p, e, 21)) {
		p = pb_get_f64(p + 1, e, (union pb_f64*)&m->value);
	}
	return 0;
}
void pb_term_TestMessage_Msf64Entry(struct TestMessage_Msf64Entry *m) {
}
char *pb_encode_TestMessage_Msf64Entry(char *p, struct TestMessage_Msf64Entry const *m) {
	if (m->key) {
		p = pb_put_tag_1(p, 8);
		p = pb_put_u32(p, *(uint32_t*)& m->key);
	}
	if (m->value) {
		p = pb_put_tag_1(p, 21);
		p = pb_put_f64(p, *(union pb_f64*)& m->value);
	}
	return p;
}
int pb_maxsz_TestMessage_Msf64Entry(struct TestMessage_Msf64Entry const *m) {
	int n = 0;
	if (m->key) {
		n += 1 + 5;
	}
	if (m->value) {
		n += 1 + 8;
	}
	n += pb_u32_size(n);
	((union pb_msg*) m)->maxsz = n;
	return n;
}

bool pb_nonzero_TestMessage_MfEntry(struct TestMessage_MfEntry const *m) {
	return false
	    || m->key
	    || m->value;
}
const char *pb_parse_TestMessage_MfEntry(const char *p, struct TestMessage_MfEntry *m) {
	(void) m;
	if (!pb_parse_map(&p)) {
		return p;
	}
	for (;;) {
		pb_string_t key;
		switch (pb_parse_field(&p, &key, 4) % 5) {
		case 0:
			return p;
		case 2:
			if (pb_cmp(key, "key")) {
				goto unknown;
			}
			p = pb_parse_u32(p, &m->key);
			continue;
		case 3:
			if (pb_cmp(key, "value")) {
				goto unknown;
			}
			p = pb_parse_float(p, &m->value);
			continue;
		default:
		unknown:
			p = pb_parse_skip(p);
		}
	}
}
int pb_print_TestMessage_MfEntry(pb_buf_t *a, struct TestMessage_MfEntry const *m) {
	char *p = pb_appendsz(a, 41);
	if (!p) {return -1;}
	*p++ = '{';
	if (m->key) {
		memcpy(p, "\"key\":", 6);
		p += 6;
		p = pb_print_u32(p, m->key);
	}
	if (m->value) {
		memcpy(p, "\"value\":", 8);
		p += 8;
		p = pb_print_float(p, m->value);
	}
	a->next = p;
	return pb_print_map_end(a);
}
int pb_get_TestMessage_MfEntry(const char *p, const char *e, struct TestMessage_MfEntry *m) {
	if (pb_skipto_1(&p, e, 8)) {
		p = pb_get_u32(p + 1, e, (uint32_t*)&m->key);
	}
	if (pb_skipto_1(&p, e, 17)) {
		p = pb_get_f32(p + 1, e, (union pb_f32*)&m->value);
	}
	return 0;
}
void pb_term_TestMessage_MfEntry(struct TestMessage_MfEntry *m) {
}
char *pb_encode_TestMessage_MfEntry(char *p, struct TestMessage_MfEntry const *m) {
	if (m->key) {
		p = pb_put_tag_1(p, 8);
		p = pb_put_u32(p, *(uint32_t*)& m->key);
	}
	if (m->value) {
		p = pb_put_tag_1(p, 17);
		p = pb_put_f32(p, *(union pb_f32*)& m->value);
	}
	return p;
}
int pb_maxsz_TestMessage_MfEntry(struct TestMessage_MfEntry const *m) {
	int n = 0;
	if (m->key) {
		n += 1 + 5;
	}
	if (m->value) {
		n += 1 + 4;
	}
	n += pb_u32_size(n);
	((union pb_msg*) m)->maxsz = n;
	return n;
}

bool pb_nonzero_TestMessage_MdEntry(struct TestMessage_MdEntry const *m) {
	return false
	    || m->key
	    || m->value;
}
const char *pb_parse_TestMessage_MdEntry(const char *p, struct TestMessage_MdEntry *m) {
	(void) m;
	if (!pb_parse_map(&p)) {
		return p;
	}
	for (;;) {
		pb_string_t key;
		switch (pb_parse_field(&p, &key, 4) % 5) {
		case 0:
			return p;
		case 2:
			if (pb_cmp(key, "key")) {
				goto unknown;
			}
			p = pb_parse_u32(p, &m->key);
			continue;
		case 3:
			if (pb_cmp(key, "value")) {
				goto unknown;
			}
			p = pb_parse_double(p, &m->value);
			continue;
		default:
		unknown:
			p = pb_parse_skip(p);
		}
	}
}
int pb_print_TestMessage_MdEntry(pb_buf_t *a, struct TestMessage_MdEntry const *m) {
	char *p = pb_appendsz(a, 49);
	if (!p) {return -1;}
	*p++ = '{';
	if (m->key) {
		memcpy(p, "\"key\":", 6);
		p += 6;
		p = pb_print_u32(p, m->key);
	}
	if (m->value) {
		memcpy(p, "\"value\":", 8);
		p += 8;
		p = pb_print_double(p, m->value);
	}
	a->next = p;
	return pb_print_map_end(a);
}
int pb_get_TestMessage_MdEntry(const char *p, const char *e, struct TestMessage_MdEntry *m) {
	if (pb_skipto_1(&p, e, 8)) {
		p = pb_get_u32(p + 1, e, (uint32_t*)&m->key);
	}
	if (pb_skipto_1(&p, e, 21)) {
		p = pb_get_f64(p + 1, e, (union pb_f64*)&m->value);
	}
	return 0;
}
void pb_term_TestMessage_MdEntry(struct TestMessage_MdEntry *m) {
}
char *pb_encode_TestMessage_MdEntry(char *p, struct TestMessage_MdEntry const *m) {
	if (m->key) {
		p = pb_put_tag_1(p, 8);
		p = pb_put_u32(p, *(uint32_t*)& m->key);
	}
	if (m->value) {
		p = pb_put_tag_1(p, 21);
		p = pb_put_f64(p, *(union pb_f64*)& m->value);
	}
	return p;
}
int pb_maxsz_TestMessage_MdEntry(struct TestMessage_MdEntry const *m) {
	int n = 0;
	if (m->key) {
		n += 1 + 5;
	}
	if (m->value) {
		n += 1 + 8;
	}
	n += pb_u32_size(n);
	((union pb_msg*) m)->maxsz = n;
	return n;
}

const char *pb_parse_TestMessage_MbyEntry(const char *p, pb_buf_t *obj, struct TestMessage_MbyEntry *m) {
	(void) obj;
	(void) m;
	if (!pb_parse_map(&p)) {
		return p;
	}
	for (;;) {
		pb_string_t key;
		switch (pb_parse_field(&p, &key, 4) % 5) {
		case 0:
			return p;
		case 2:
			if (pb_cmp(key, "key")) {
				goto unknown;
			}
			p = pb_parse_u32(p, &m->key);
			continue;
		case 3:
			if (pb_cmp(key, "value")) {
				goto unknown;
			}
			p = pb_parse_bytes(p, &m->value);
			continue;
		default:
		unknown:
			p = pb_parse_skip(p);
		}
	}
}
int pb_print_TestMessage_MbyEntry(pb_buf_t *a, struct TestMessage_MbyEntry const *m) {
	char *p = pb_appendsz(a, 19);
	if (!p) {return -1;}
	*p++ = '{';
	if (m->key) {
		memcpy(p, "\"key\":", 6);
		p += 6;
		p = pb_print_u32(p, m->key);
	}
	a->next = p;
	if (m->value.len) {
		if (pb_append(a, "\"value\":", 8)) {return -1;}
		if (pb_print_bytes(a, m->value)) {return -1;}
		a->next[-1] = ','; // replace trailing newline
	}
	return pb_print_map_end(a);
}
int pb_get_TestMessage_MbyEntry(const char *p, const char *e, pb_buf_t *obj, struct TestMessage_MbyEntry *m) {
	(void) obj;
	if (pb_skipto_1(&p, e, 8)) {
		p = pb_get_u32(p + 1, e, (uint32_t*)&m->key);
	}
	if (pb_skipto_1(&p, e, 18)) {
		p = pb_get_bytes(p + 1, e, (pb_bytes_t*)&m->value);
	}
	return 0;
}
void pb_term_TestMessage_MbyEntry(struct TestMessage_MbyEntry *m) {
}
char *pb_encode_TestMessage_MbyEntry(char *p, struct TestMessage_MbyEntry const *m) {
	if (m->key) {
		p = pb_put_tag_1(p, 8);
		p = pb_put_u32(p, *(uint32_t*)& m->key);
	}
	if (m->value.len) {
		p = pb_put_tag_1(p, 18);
		p = pb_put_bytes(p, *(pb_bytes_t*)& m->value);
	}
	return p;
}
int pb_maxsz_TestMessage_MbyEntry(struct TestMessage_MbyEntry const *m) {
	int n = 0;
	if (m->key) {
		n += 1 + 5;
	}
	if (m->value.len) {
		n += 1 + 5 + m->value.len;
	}
	n += pb_u32_size(n);
	((union pb_msg*) m)->maxsz = n;
	return n;
}

const char *pb_parse_TestMessage_MstrEntry(const char *p, pb_buf_t *obj, struct TestMessage_MstrEntry *m) {
	(void) obj;
	(void) m;
	if (!pb_parse_map(&p)) {
		return p;
	}
	for (;;) {
		pb_string_t key;
		switch (pb_parse_field(&p, &key, 4) % 5) {
		case 0:
			return p;
		case 2:
			if (pb_cmp(key, "key")) {
				goto unknown;
			}
			p = pb_parse_u32(p, &m->key);
			continue;
		case 3:
			if (pb_cmp(key, "value")) {
				goto unknown;
			}
			p = pb_parse_string(p, &m->value);
			continue;
		default:
		unknown:
			p = pb_parse_skip(p);
		}
	}
}
int pb_print_TestMessage_MstrEntry(pb_buf_t *a, struct TestMessage_MstrEntry const *m) {
	char *p = pb_appendsz(a, 19);
	if (!p) {return -1;}
	*p++ = '{';
	if (m->key) {
		memcpy(p, "\"key\":", 6);
		p += 6;
		p = pb_print_u32(p, m->key);
	}
	a->next = p;
	if (m->value.len) {
		if (pb_append(a, "\"value\":", 8)) {return -1;}
		if (pb_print_string(a, m->value)) {return -1;}
		a->next[-1] = ','; // replace trailing newline
	}
	return pb_print_map_end(a);
}
int pb_get_TestMessage_MstrEntry(const char *p, const char *e, pb_buf_t *obj, struct TestMessage_MstrEntry *m) {
	(void) obj;
	if (pb_skipto_1(&p, e, 8)) {
		p = pb_get_u32(p + 1, e, (uint32_t*)&m->key);
	}
	if (pb_skipto_1(&p, e, 18)) {
		p = pb_get_string(p + 1, e, (pb_string_t*)&m->value);
	}
	return 0;
}
void pb_term_TestMessage_MstrEntry(struct TestMessage_MstrEntry *m) {
	if (m->value.c_str) {
		((char*)m->value.c_str)[m->value.len] = '\0';
	} else {
		m->value.c_str = "";
	}
}
char *pb_encode_TestMessage_MstrEntry(char *p, struct TestMessage_MstrEntry const *m) {
	if (m->key) {
		p = pb_put_tag_1(p, 8);
		p = pb_put_u32(p, *(uint32_t*)& m->key);
	}
	if (m->value.len) {
		p = pb_put_tag_1(p, 18);
		p = pb_put_string(p, *(pb_string_t*)& m->value);
	}
	return p;
}
int pb_maxsz_TestMessage_MstrEntry(struct TestMessage_MstrEntry const *m) {
	int n = 0;
	if (m->key) {
		n += 1 + 5;
	}
	if (m->value.len) {
		n += 1 + 5 + m->value.len;
	}
	n += pb_u32_size(n);
	((union pb_msg*) m)->maxsz = n;
	return n;
}

bool pb_nonzero_TestMessage_MenEntry(struct TestMessage_MenEntry const *m) {
	return false
	    || m->key
	    || m->value;
}
const char *pb_parse_TestMessage_MenEntry(const char *p, struct TestMessage_MenEntry *m) {
	(void) m;
	if (!pb_parse_map(&p)) {
		return p;
	}
	for (;;) {
		pb_string_t key;
		switch (pb_parse_field(&p, &key, 4) % 5) {
		case 0:
			return p;
		case 2:
			if (pb_cmp(key, "key")) {
				goto unknown;
			}
			p = pb_parse_u32(p, &m->key);
			continue;
		case 3:
			if (pb_cmp(key, "value")) {
				goto unknown;
			}
			p = pb_parse_TestEnum(p, &m->value);
			continue;
		default:
		unknown:
			p = pb_parse_skip(p);
		}
	}
}
int pb_print_TestMessage_MenEntry(pb_buf_t *a, struct TestMessage_MenEntry const *m) {
	char *p = pb_appendsz(a, 37);
	if (!p) {return -1;}
	*p++ = '{';
	if (m->key) {
		memcpy(p, "\"key\":", 6);
		p += 6;
		p = pb_print_u32(p, m->key);
	}
	if (m->value) {
		memcpy(p, "\"value\":", 8);
		p += 8;
		p = pb_print_TestEnum(p, m->value);
	}
	a->next = p;
	return pb_print_map_end(a);
}
int pb_get_TestMessage_MenEntry(const char *p, const char *e, struct TestMessage_MenEntry *m) {
	if (pb_skipto_1(&p, e, 8)) {
		p = pb_get_u32(p + 1, e, (uint32_t*)&m->key);
	}
	if (pb_skipto_1(&p, e, 16)) {
		p = pb_get_u32(p + 1, e, (uint32_t*)&m->value);
	}
	return 0;
}
void pb_term_TestMessage_MenEntry(struct TestMessage_MenEntry *m) {
}
char *pb_encode_TestMessage_MenEntry(char *p, struct TestMessage_MenEntry const *m) {
	if (m->key) {
		p = pb_put_tag_1(p, 8);
		p = pb_put_u32(p, *(uint32_t*)& m->key);
	}
	if (m->value) {
		p = pb_put_tag_1(p, 16);
		p = pb_put_u32(p, *(uint32_t*)& m->value);
	}
	return p;
}
int pb_maxsz_TestMessage_MenEntry(struct TestMessage_MenEntry const *m) {
	int n = 0;
	if (m->key) {
		n += 1 + 5;
	}
	if (m->value) {
		n += 1 + 5;
	}
	n += pb_u32_size(n);
	((union pb_msg*) m)->maxsz = n;
	return n;
}

const char *pb_parse_TestMessage_MmsgEntry(const char *p, pb_buf_t *obj, struct TestMessage_MmsgEntry *m) {
	(void) obj;
	(void) m;
	if (!pb_parse_map(&p)) {
		return p;
	}
	for (;;) {
		pb_string_t key;
		switch (pb_parse_field(&p, &key, 4) % 5) {
		case 0:
			return p;
		case 2:
			if (pb_cmp(key, "key")) {
				goto unknown;
			}
			p = pb_parse_u32(p, &m->key);
			continue;
		case 3:
			if (pb_cmp(key, "value")) {
				goto unknown;
			}
			m->value = (struct TestMessage*) pb_calloc(obj, sizeof(struct TestMessage));
			if (!m->value) {return pb_errret;}
			p = pb_parse_TestMessage(p, obj, (struct TestMessage*) m->value);
			continue;
		default:
		unknown:
			p = pb_parse_skip(p);
		}
	}
}
int pb_print_TestMessage_MmsgEntry(pb_buf_t *a, struct TestMessage_MmsgEntry const *m) {
	char *p = pb_appendsz(a, 19);
	if (!p) {return -1;}
	*p++ = '{';
	if (m->key) {
		memcpy(p, "\"key\":", 6);
		p += 6;
		p = pb_print_u32(p, m->key);
	}
	a->next = p;
	if (m->value) {
		if (pb_append(a, "\"value\":", 8)) {return -1;}
		if (pb_print_TestMessage(a, m->value)) {return -1;}
		a->next[-1] = ','; // replace trailing newline
	}
	return pb_print_map_end(a);
}
int pb_get_TestMessage_MmsgEntry(const char *p, const char *e, pb_buf_t *obj, struct TestMessage_MmsgEntry *m) {
	(void) obj;
	if (pb_skipto_1(&p, e, 8)) {
		p = pb_get_u32(p + 1, e, (uint32_t*)&m->key);
	}
	if (pb_skipto_1(&p, e, 18)) {
		pb_string_t msg;
		p = pb_get_string(p + 1, e, &msg);
		m->value = (struct TestMessage*) pb_calloc(obj, sizeof(struct TestMessage));
		if (!m->value || pb_get_TestMessage(msg.c_str, msg.c_str + msg.len, obj, (struct TestMessage*) m->value)) {
			return -1;
		}
	}
	return 0;
}
void pb_term_TestMessage_MmsgEntry(struct TestMessage_MmsgEntry *m) {
	if (m->value) {
		pb_term_TestMessage((struct TestMessage*) m->value);
	}
}
char *pb_encode_TestMessage_MmsgEntry(char *p, struct TestMessage_MmsgEntry const *m) {
	if (m->key) {
		p = pb_put_tag_1(p, 8);
		p = pb_put_u32(p, *(uint32_t*)& m->key);
	}
	if (m->value) {
		p = pb_put_tag_1(p, 18);
		int szlen = pb_u32_size(m->value->pb_hdr.maxsz);
		char *start = p+szlen;
		p = pb_encode_TestMessage(start, m->value);
		pb_put_finish(p, start, szlen);
	}
	return p;
}
int pb_maxsz_TestMessage_MmsgEntry(struct TestMessage_MmsgEntry const *m) {
	int n = 0;
	if (m->key) {
		n += 1 + 5;
	}
	if (m->value) {
		n += 1 + pb_maxsz_TestMessage(m->value);
	}
	n += pb_u32_size(n);
	((union pb_msg*) m)->maxsz = n;
	return n;
}

bool pb_nonzero_TestMessage_MpodEntry(struct TestMessage_MpodEntry const *m) {
	return false
	    || m->key
	    || pb_nonzero_TestPod(&m->value);
}
const char *pb_parse_TestMessage_MpodEntry(const char *p, struct TestMessage_MpodEntry *m) {
	(void) m;
	if (!pb_parse_map(&p)) {
		return p;
	}
	for (;;) {
		pb_string_t key;
		switch (pb_parse_field(&p, &key, 4) % 5) {
		case 0:
			return p;
		case 2:
			if (pb_cmp(key, "key")) {
				goto unknown;
			}
			p = pb_parse_u32(p, &m->key);
			continue;
		case 3:
			if (pb_cmp(key, "value")) {
				goto unknown;
			}
			p = pb_parse_TestPod(p, &m->value);
			continue;
		default:
		unknown:
			p = pb_parse_skip(p);
		}
	}
}
int pb_print_TestMessage_MpodEntry(pb_buf_t *a, struct TestMessage_MpodEntry const *m) {
	char *p = pb_appendsz(a, 19);
	if (!p) {return -1;}
	*p++ = '{';
	if (m->key) {
		memcpy(p, "\"key\":", 6);
		p += 6;
		p = pb_print_u32(p, m->key);
	}
	a->next = p;
	if (pb_nonzero_TestPod(&m->value)) {
		if (pb_append(a, "\"value\":", 8)) {return -1;}
		if (pb_print_TestPod(a, &m->value)) {return -1;}
		a->next[-1] = ','; // replace trailing newline
	}
	return pb_print_map_end(a);
}
int pb_get_TestMessage_MpodEntry(const char *p, const char *e, struct TestMessage_MpodEntry *m) {
	if (pb_skipto_1(&p, e, 8)) {
		p = pb_get_u32(p + 1, e, (uint32_t*)&m->key);
	}
	if (pb_skipto_1(&p, e, 18)) {
		pb_string_t msg;
		p = pb_get_string(p + 1, e, &msg);
		if (pb_get_TestPod(msg.c_str, msg.c_str + msg.len, &m->value)) {
			return -1;
		}
	}
	return 0;
}
void pb_term_TestMessage_MpodEntry(struct TestMessage_MpodEntry *m) {
	pb_term_TestPod((struct TestPod*) &m->value);
}
char *pb_encode_TestMessage_MpodEntry(char *p, struct TestMessage_MpodEntry const *m) {
	if (m->key) {
		p = pb_put_tag_1(p, 8);
		p = pb_put_u32(p, *(uint32_t*)& m->key);
	}
	if (pb_nonzero_TestPod(&m->value)) {
		p = pb_put_tag_1(p, 18);
		char *start = p+1;
		p = pb_encode_TestPod(start, &m->value);
		start[-1] = (char) (p - start);
	}
	return p;
}
int pb_maxsz_TestMessage_MpodEntry(struct TestMessage_MpodEntry const *m) {
	int n = 0;
	if (m->key) {
		n += 1 + 5;
	}
	if (pb_nonzero_TestPod(&m->value)) {
		n += 1 + 13;
	}
	n += pb_u32_size(n);
	((union pb_msg*) m)->maxsz = n;
	return n;
}

bool pb_nonzero_TestPod(struct TestPod const *m) {
	return false
	    || m->foo_type
	    || m->foo.u;
}
const char *pb_parse_TestPod(const char *p, struct TestPod *m) {
	(void) m;
	if (!pb_parse_map(&p)) {
		return p;
	}
	for (;;) {
		pb_string_t key;
		switch (pb_parse_field(&p, &key, 121) % 8) {
		case 0:
			return p;
		case 5:
			if (pb_cmp(key, "u")) {
				goto unknown;
			}
			m->foo_type = TESTPOD_U;
			p = pb_parse_u32(p, &m->foo.u);
			continue;
		case 1:
			if (pb_cmp(key, "i")) {
				goto unknown;
			}
			m->foo_type = TESTPOD_I;
			p = pb_parse_i32(p, &m->foo.i);
			continue;
		default:
		unknown:
			p = pb_parse_skip(p);
		}
	}
}
int pb_print_TestPod(pb_buf_t *a, struct TestPod const *m) {
	char *p = pb_appendsz(a, 33);
	if (!p) {return -1;}
	*p++ = '{';
	if(m->foo_type == TESTPOD_U) {
		memcpy(p, "\"u\":", 4);
		p += 4;
		p = pb_print_u32(p, m->foo.u);
	}
	if(m->foo_type == TESTPOD_I) {
		memcpy(p, "\"i\":", 4);
		p += 4;
		p = pb_print_i32(p, m->foo.i);
	}
	a->next = p;
	return pb_print_map_end(a);
}
int pb_get_TestPod(const char *p, const char *e, struct TestPod *m) {
	if (pb_skipto_1(&p, e, 8)) {
		m->foo_type = TESTPOD_U;
		p = pb_get_u32(p + 1, e, (uint32_t*)&m->foo.u);
	}
	if (pb_skipto_1(&p, e, 16)) {
		m->foo_type = TESTPOD_I;
		p = pb_get_u32(p + 1, e, (uint32_t*)&m->foo.i);
	}
	return 0;
}
void pb_term_TestPod(struct TestPod *m) {
}
char *pb_encode_TestPod(char *p, struct TestPod const *m) {
	if(m->foo_type == TESTPOD_U) {
		p = pb_put_tag_1(p, 8);
		p = pb_put_u32(p, *(uint32_t*)& m->foo.u);
	}
	if(m->foo_type == TESTPOD_I) {
		p = pb_put_tag_1(p, 16);
		p = pb_put_u32(p, *(uint32_t*)& m->foo.i);
	}
	return p;
}
int pb_maxsz_TestPod(struct TestPod const *m) {
	int n = 0;
	if(m->foo_type == TESTPOD_U) {
		n += 1 + 5;
	}
	if(m->foo_type == TESTPOD_I) {
		n += 1 + 5;
	}
	n += pb_u32_size(n);
	((union pb_msg*) m)->maxsz = n;
	return n;
}

char *pb_print_TestEnum(char *p, enum TestEnum v){
	switch ((int32_t) v) {
	case 0:
		memcpy(p, "\"ENUM_A\",", 9);
		return p + 9;
	case 1:
		memcpy(p, "\"ENUM_B\",", 9);
		return p + 9;
	case 2:
		memcpy(p, "\"ENUM_C\",", 9);
		return p + 9;
	default:
		memcpy(p, "\"\",", 3);
		return p + 3;
	}
}
const char *pb_parse_TestEnum(const char *p, enum TestEnum *v) {
	pb_string_t val;
	*v = (enum TestEnum) 0;
	switch (pb_parse_enum(&p, &val, 4) % 6) {
	case 1:
		if (!pb_cmp(val, "ENUM_A")) {
			*v = (enum TestEnum) 0;
		}
		break;
	case 2:
		if (!pb_cmp(val, "ENUM_B")) {
			*v = (enum TestEnum) 1;
		}
		break;
	case 3:
		if (!pb_cmp(val, "ENUM_C")) {
			*v = (enum TestEnum) 2;
		}
		break;
	}
	return p;
}
const char *rpc_TestService(struct TestService* rpc, struct pr_http *h, pb_string_t body, pb_buf_t *resp) {
	pb_string_t path = {h->name.len, h->name.c_str};
	switch (pr_hash_path(path, 13) % 5) {
	case 2:
		if(pb_cmp(path, "/TestService/Test")) {
			return pr_not_found;
		} else {
			struct TestMessage in;
			struct TestMessage out;
			if (pb_parse_TestMessage((char*)body.c_str, &h->request_objects, &in) == pb_errret) {
				return pr_parse_error;
			}
			const char *ret = rpc->Test(rpc, h, NULL, &out);
			if (pb_print_TestMessage(resp, &out)) {
				return pr_print_error;
			}
			return ret;
		}
	default:
		return pr_not_found;
	}
}
