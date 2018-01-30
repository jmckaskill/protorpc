#include "test.proto.h"

char *pb_parse_TestMessage(char *p, pb_buf_t *obj, struct TestMessage *m) {
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
	char *p = (char*) pb_alloc(a, 312, 1);
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
		char *mp = (char*) pb_alloc(a, 3 /*[],*/ + m->rb.len * 6, 1);
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
		char *mp = (char*) pb_alloc(a, 3 /*[],*/ + m->ru32.len * 12, 1);
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
		char *mp = (char*) pb_alloc(a, 3 /*[],*/ + m->ru64.len * 24, 1);
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
		char *mp = (char*) pb_alloc(a, 3 /*[],*/ + m->ri32.len * 12, 1);
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
		char *mp = (char*) pb_alloc(a, 3 /*[],*/ + m->ri64.len * 24, 1);
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
		char *mp = (char*) pb_alloc(a, 3 /*[],*/ + m->rs32.len * 12, 1);
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
		char *mp = (char*) pb_alloc(a, 3 /*[],*/ + m->rs64.len * 24, 1);
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
		char *mp = (char*) pb_alloc(a, 3 /*[],*/ + m->rf32.len * 12, 1);
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
		char *mp = (char*) pb_alloc(a, 3 /*[],*/ + m->rf64.len * 24, 1);
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
		char *mp = (char*) pb_alloc(a, 3 /*[],*/ + m->rsf32.len * 12, 1);
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
		char *mp = (char*) pb_alloc(a, 3 /*[],*/ + m->rsf64.len * 24, 1);
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
		char *mp = (char*) pb_alloc(a, 3 /*[],*/ + m->rf.len * 14, 1);
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
		char *mp = (char*) pb_alloc(a, 3 /*[],*/ + m->rd.len * 22, 1);
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
		char *mp = (char*) pb_alloc(a, 3 /*[],*/ + m->ren.len * 10, 1);
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
static const struct proto_field fields_TestMessage[] = {
	{PROTO_BOOL, offsetof(struct TestMessage, b), 8, NULL, -1},
	{PROTO_I32, offsetof(struct TestMessage, i32), 16, NULL, -1},
	{PROTO_S32, offsetof(struct TestMessage, s32), 24, NULL, -1},
	{PROTO_SF32, offsetof(struct TestMessage, sf32), 37, NULL, -1},
	{PROTO_U32, offsetof(struct TestMessage, u32), 40, NULL, -1},
	{PROTO_F32, offsetof(struct TestMessage, f32), 53, NULL, -1},
	{PROTO_I64, offsetof(struct TestMessage, i64), 56, NULL, -1},
	{PROTO_SF64, offsetof(struct TestMessage, sf64), 65, NULL, -1},
	{PROTO_S64, offsetof(struct TestMessage, s64), 72, NULL, -1},
	{PROTO_U64, offsetof(struct TestMessage, u64), 80, NULL, -1},
	{PROTO_F64, offsetof(struct TestMessage, f64), 89, NULL, -1},
	{PROTO_FLOAT, offsetof(struct TestMessage, f), 101, NULL, -1},
	{PROTO_DOUBLE, offsetof(struct TestMessage, d), 105, NULL, -1},
	{PROTO_BYTES, offsetof(struct TestMessage, by), 114, NULL, -1},
	{PROTO_STRING, offsetof(struct TestMessage, str), 122, NULL, -1},
	{PROTO_ENUM, offsetof(struct TestMessage, en), 128, NULL, -1},
	{PROTO_MESSAGE, offsetof(struct TestMessage, msg), 138, &pb_type_TestMessage, -1},
	{PROTO_POD, offsetof(struct TestMessage, pod), 146, &pb_type_TestPod, -1},
	{PROTO_LIST_BOOL, offsetof(struct TestMessage, rb), 170, NULL, -1},
	{PROTO_LIST_I32, offsetof(struct TestMessage, ri32), 178, NULL, -1},
	{PROTO_LIST_S32, offsetof(struct TestMessage, rs32), 186, NULL, -1},
	{PROTO_LIST_SF32, offsetof(struct TestMessage, rsf32), 194, NULL, -1},
	{PROTO_LIST_U32, offsetof(struct TestMessage, ru32), 202, NULL, -1},
	{PROTO_LIST_F32, offsetof(struct TestMessage, rf32), 210, NULL, -1},
	{PROTO_LIST_I64, offsetof(struct TestMessage, ri64), 218, NULL, -1},
	{PROTO_LIST_SF64, offsetof(struct TestMessage, rsf64), 226, NULL, -1},
	{PROTO_LIST_S64, offsetof(struct TestMessage, rs64), 234, NULL, -1},
	{PROTO_LIST_U64, offsetof(struct TestMessage, ru64), 1682, NULL, -1},
	{PROTO_LIST_F64, offsetof(struct TestMessage, rf64), 1690, NULL, -1},
	{PROTO_LIST_FLOAT, offsetof(struct TestMessage, rf), 1698, NULL, -1},
	{PROTO_LIST_DOUBLE, offsetof(struct TestMessage, rd), 1706, NULL, -1},
	{PROTO_LIST_BYTES, offsetof(struct TestMessage, rby), 1714, NULL, -1},
	{PROTO_LIST_STRING, offsetof(struct TestMessage, rstr), 1722, NULL, -1},
	{PROTO_LIST_ENUM, offsetof(struct TestMessage, ren), 1730, NULL, -1},
	{PROTO_LIST_MESSAGE, offsetof(struct TestMessage, rmsg), 1738, &pb_type_TestMessage, -1},
	{PROTO_LIST_POD, offsetof(struct TestMessage, rpod), 1746, &pb_type_TestPod, -1},
	{PROTO_LIST_POD, offsetof(struct TestMessage, mb), 1770, &pb_type_TestMessage_MbEntry, -1},
	{PROTO_LIST_POD, offsetof(struct TestMessage, mi32), 1778, &pb_type_TestMessage_Mi32Entry, -1},
	{PROTO_LIST_POD, offsetof(struct TestMessage, ms32), 1786, &pb_type_TestMessage_Ms32Entry, -1},
	{PROTO_LIST_POD, offsetof(struct TestMessage, msf32), 1794, &pb_type_TestMessage_Msf32Entry, -1},
	{PROTO_LIST_POD, offsetof(struct TestMessage, mu32), 1802, &pb_type_TestMessage_Mu32Entry, -1},
	{PROTO_LIST_POD, offsetof(struct TestMessage, mf32), 1810, &pb_type_TestMessage_Mf32Entry, -1},
	{PROTO_LIST_POD, offsetof(struct TestMessage, mi64), 1818, &pb_type_TestMessage_Mi64Entry, -1},
	{PROTO_LIST_POD, offsetof(struct TestMessage, msf64), 1826, &pb_type_TestMessage_Msf64Entry, -1},
	{PROTO_LIST_POD, offsetof(struct TestMessage, ms64), 1834, &pb_type_TestMessage_Ms64Entry, -1},
	{PROTO_LIST_POD, offsetof(struct TestMessage, mu64), 17682, &pb_type_TestMessage_Mu64Entry, -1},
	{PROTO_LIST_POD, offsetof(struct TestMessage, mf64), 17690, &pb_type_TestMessage_Mf64Entry, -1},
	{PROTO_LIST_POD, offsetof(struct TestMessage, mf), 17698, &pb_type_TestMessage_MfEntry, -1},
	{PROTO_LIST_POD, offsetof(struct TestMessage, md), 17706, &pb_type_TestMessage_MdEntry, -1},
	{PROTO_LIST_MESSAGE, offsetof(struct TestMessage, mby), 17714, &pb_type_TestMessage_MbyEntry, -1},
	{PROTO_LIST_MESSAGE, offsetof(struct TestMessage, mstr), 17722, &pb_type_TestMessage_MstrEntry, -1},
	{PROTO_LIST_POD, offsetof(struct TestMessage, men), 17730, &pb_type_TestMessage_MenEntry, -1},
	{PROTO_LIST_MESSAGE, offsetof(struct TestMessage, mmsg), 17738, &pb_type_TestMessage_MmsgEntry, -1},
	{PROTO_LIST_POD, offsetof(struct TestMessage, mpod), 17746, &pb_type_TestMessage_MpodEntry, -1}
};
const struct proto_message pb_type_TestMessage = {
	sizeof(struct TestMessage),
	sizeof(fields_TestMessage) / sizeof(struct proto_field),
	fields_TestMessage
};

bool pb_nonzero_TestMessage_MbEntry(struct TestMessage_MbEntry const *m) {
	return false
	    || m->key
	    || m->value;
}
char *pb_parse_TestMessage_MbEntry(char *p, struct TestMessage_MbEntry *m) {
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
	char *p = (char*) pb_alloc(a, 33, 1);
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
static const struct proto_field fields_TestMessage_MbEntry[] = {
	{PROTO_U32, offsetof(struct TestMessage_MbEntry, key), 8, NULL, -1},
	{PROTO_BOOL, offsetof(struct TestMessage_MbEntry, value), 16, NULL, -1}
};
const struct proto_message pb_type_TestMessage_MbEntry = {
	sizeof(struct TestMessage_MbEntry),
	sizeof(fields_TestMessage_MbEntry) / sizeof(struct proto_field),
	fields_TestMessage_MbEntry
};

bool pb_nonzero_TestMessage_Mu32Entry(struct TestMessage_Mu32Entry const *m) {
	return false
	    || m->key
	    || m->value;
}
char *pb_parse_TestMessage_Mu32Entry(char *p, struct TestMessage_Mu32Entry *m) {
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
	char *p = (char*) pb_alloc(a, 39, 1);
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
static const struct proto_field fields_TestMessage_Mu32Entry[] = {
	{PROTO_U32, offsetof(struct TestMessage_Mu32Entry, key), 8, NULL, -1},
	{PROTO_U32, offsetof(struct TestMessage_Mu32Entry, value), 16, NULL, -1}
};
const struct proto_message pb_type_TestMessage_Mu32Entry = {
	sizeof(struct TestMessage_Mu32Entry),
	sizeof(fields_TestMessage_Mu32Entry) / sizeof(struct proto_field),
	fields_TestMessage_Mu32Entry
};

bool pb_nonzero_TestMessage_Mu64Entry(struct TestMessage_Mu64Entry const *m) {
	return false
	    || m->key
	    || m->value;
}
char *pb_parse_TestMessage_Mu64Entry(char *p, struct TestMessage_Mu64Entry *m) {
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
	char *p = (char*) pb_alloc(a, 51, 1);
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
static const struct proto_field fields_TestMessage_Mu64Entry[] = {
	{PROTO_U32, offsetof(struct TestMessage_Mu64Entry, key), 8, NULL, -1},
	{PROTO_U64, offsetof(struct TestMessage_Mu64Entry, value), 16, NULL, -1}
};
const struct proto_message pb_type_TestMessage_Mu64Entry = {
	sizeof(struct TestMessage_Mu64Entry),
	sizeof(fields_TestMessage_Mu64Entry) / sizeof(struct proto_field),
	fields_TestMessage_Mu64Entry
};

bool pb_nonzero_TestMessage_Mi32Entry(struct TestMessage_Mi32Entry const *m) {
	return false
	    || m->key
	    || m->value;
}
char *pb_parse_TestMessage_Mi32Entry(char *p, struct TestMessage_Mi32Entry *m) {
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
	char *p = (char*) pb_alloc(a, 39, 1);
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
static const struct proto_field fields_TestMessage_Mi32Entry[] = {
	{PROTO_U32, offsetof(struct TestMessage_Mi32Entry, key), 8, NULL, -1},
	{PROTO_I32, offsetof(struct TestMessage_Mi32Entry, value), 16, NULL, -1}
};
const struct proto_message pb_type_TestMessage_Mi32Entry = {
	sizeof(struct TestMessage_Mi32Entry),
	sizeof(fields_TestMessage_Mi32Entry) / sizeof(struct proto_field),
	fields_TestMessage_Mi32Entry
};

bool pb_nonzero_TestMessage_Mi64Entry(struct TestMessage_Mi64Entry const *m) {
	return false
	    || m->key
	    || m->value;
}
char *pb_parse_TestMessage_Mi64Entry(char *p, struct TestMessage_Mi64Entry *m) {
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
	char *p = (char*) pb_alloc(a, 51, 1);
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
static const struct proto_field fields_TestMessage_Mi64Entry[] = {
	{PROTO_U32, offsetof(struct TestMessage_Mi64Entry, key), 8, NULL, -1},
	{PROTO_I64, offsetof(struct TestMessage_Mi64Entry, value), 16, NULL, -1}
};
const struct proto_message pb_type_TestMessage_Mi64Entry = {
	sizeof(struct TestMessage_Mi64Entry),
	sizeof(fields_TestMessage_Mi64Entry) / sizeof(struct proto_field),
	fields_TestMessage_Mi64Entry
};

bool pb_nonzero_TestMessage_Ms32Entry(struct TestMessage_Ms32Entry const *m) {
	return false
	    || m->key
	    || m->value;
}
char *pb_parse_TestMessage_Ms32Entry(char *p, struct TestMessage_Ms32Entry *m) {
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
	char *p = (char*) pb_alloc(a, 39, 1);
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
static const struct proto_field fields_TestMessage_Ms32Entry[] = {
	{PROTO_U32, offsetof(struct TestMessage_Ms32Entry, key), 8, NULL, -1},
	{PROTO_S32, offsetof(struct TestMessage_Ms32Entry, value), 16, NULL, -1}
};
const struct proto_message pb_type_TestMessage_Ms32Entry = {
	sizeof(struct TestMessage_Ms32Entry),
	sizeof(fields_TestMessage_Ms32Entry) / sizeof(struct proto_field),
	fields_TestMessage_Ms32Entry
};

bool pb_nonzero_TestMessage_Ms64Entry(struct TestMessage_Ms64Entry const *m) {
	return false
	    || m->key
	    || m->value;
}
char *pb_parse_TestMessage_Ms64Entry(char *p, struct TestMessage_Ms64Entry *m) {
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
	char *p = (char*) pb_alloc(a, 51, 1);
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
static const struct proto_field fields_TestMessage_Ms64Entry[] = {
	{PROTO_U32, offsetof(struct TestMessage_Ms64Entry, key), 8, NULL, -1},
	{PROTO_S64, offsetof(struct TestMessage_Ms64Entry, value), 16, NULL, -1}
};
const struct proto_message pb_type_TestMessage_Ms64Entry = {
	sizeof(struct TestMessage_Ms64Entry),
	sizeof(fields_TestMessage_Ms64Entry) / sizeof(struct proto_field),
	fields_TestMessage_Ms64Entry
};

bool pb_nonzero_TestMessage_Mf32Entry(struct TestMessage_Mf32Entry const *m) {
	return false
	    || m->key
	    || m->value;
}
char *pb_parse_TestMessage_Mf32Entry(char *p, struct TestMessage_Mf32Entry *m) {
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
	char *p = (char*) pb_alloc(a, 39, 1);
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
static const struct proto_field fields_TestMessage_Mf32Entry[] = {
	{PROTO_U32, offsetof(struct TestMessage_Mf32Entry, key), 8, NULL, -1},
	{PROTO_F32, offsetof(struct TestMessage_Mf32Entry, value), 21, NULL, -1}
};
const struct proto_message pb_type_TestMessage_Mf32Entry = {
	sizeof(struct TestMessage_Mf32Entry),
	sizeof(fields_TestMessage_Mf32Entry) / sizeof(struct proto_field),
	fields_TestMessage_Mf32Entry
};

bool pb_nonzero_TestMessage_Mf64Entry(struct TestMessage_Mf64Entry const *m) {
	return false
	    || m->key
	    || m->value;
}
char *pb_parse_TestMessage_Mf64Entry(char *p, struct TestMessage_Mf64Entry *m) {
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
	char *p = (char*) pb_alloc(a, 51, 1);
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
static const struct proto_field fields_TestMessage_Mf64Entry[] = {
	{PROTO_U32, offsetof(struct TestMessage_Mf64Entry, key), 8, NULL, -1},
	{PROTO_F64, offsetof(struct TestMessage_Mf64Entry, value), 17, NULL, -1}
};
const struct proto_message pb_type_TestMessage_Mf64Entry = {
	sizeof(struct TestMessage_Mf64Entry),
	sizeof(fields_TestMessage_Mf64Entry) / sizeof(struct proto_field),
	fields_TestMessage_Mf64Entry
};

bool pb_nonzero_TestMessage_Msf32Entry(struct TestMessage_Msf32Entry const *m) {
	return false
	    || m->key
	    || m->value;
}
char *pb_parse_TestMessage_Msf32Entry(char *p, struct TestMessage_Msf32Entry *m) {
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
	char *p = (char*) pb_alloc(a, 39, 1);
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
static const struct proto_field fields_TestMessage_Msf32Entry[] = {
	{PROTO_U32, offsetof(struct TestMessage_Msf32Entry, key), 8, NULL, -1},
	{PROTO_SF32, offsetof(struct TestMessage_Msf32Entry, value), 21, NULL, -1}
};
const struct proto_message pb_type_TestMessage_Msf32Entry = {
	sizeof(struct TestMessage_Msf32Entry),
	sizeof(fields_TestMessage_Msf32Entry) / sizeof(struct proto_field),
	fields_TestMessage_Msf32Entry
};

bool pb_nonzero_TestMessage_Msf64Entry(struct TestMessage_Msf64Entry const *m) {
	return false
	    || m->key
	    || m->value;
}
char *pb_parse_TestMessage_Msf64Entry(char *p, struct TestMessage_Msf64Entry *m) {
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
	char *p = (char*) pb_alloc(a, 51, 1);
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
static const struct proto_field fields_TestMessage_Msf64Entry[] = {
	{PROTO_U32, offsetof(struct TestMessage_Msf64Entry, key), 8, NULL, -1},
	{PROTO_SF64, offsetof(struct TestMessage_Msf64Entry, value), 17, NULL, -1}
};
const struct proto_message pb_type_TestMessage_Msf64Entry = {
	sizeof(struct TestMessage_Msf64Entry),
	sizeof(fields_TestMessage_Msf64Entry) / sizeof(struct proto_field),
	fields_TestMessage_Msf64Entry
};

bool pb_nonzero_TestMessage_MfEntry(struct TestMessage_MfEntry const *m) {
	return false
	    || m->key
	    || m->value;
}
char *pb_parse_TestMessage_MfEntry(char *p, struct TestMessage_MfEntry *m) {
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
	char *p = (char*) pb_alloc(a, 41, 1);
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
static const struct proto_field fields_TestMessage_MfEntry[] = {
	{PROTO_U32, offsetof(struct TestMessage_MfEntry, key), 8, NULL, -1},
	{PROTO_FLOAT, offsetof(struct TestMessage_MfEntry, value), 21, NULL, -1}
};
const struct proto_message pb_type_TestMessage_MfEntry = {
	sizeof(struct TestMessage_MfEntry),
	sizeof(fields_TestMessage_MfEntry) / sizeof(struct proto_field),
	fields_TestMessage_MfEntry
};

bool pb_nonzero_TestMessage_MdEntry(struct TestMessage_MdEntry const *m) {
	return false
	    || m->key
	    || m->value;
}
char *pb_parse_TestMessage_MdEntry(char *p, struct TestMessage_MdEntry *m) {
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
	char *p = (char*) pb_alloc(a, 49, 1);
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
static const struct proto_field fields_TestMessage_MdEntry[] = {
	{PROTO_U32, offsetof(struct TestMessage_MdEntry, key), 8, NULL, -1},
	{PROTO_DOUBLE, offsetof(struct TestMessage_MdEntry, value), 17, NULL, -1}
};
const struct proto_message pb_type_TestMessage_MdEntry = {
	sizeof(struct TestMessage_MdEntry),
	sizeof(fields_TestMessage_MdEntry) / sizeof(struct proto_field),
	fields_TestMessage_MdEntry
};

char *pb_parse_TestMessage_MbyEntry(char *p, pb_buf_t *obj, struct TestMessage_MbyEntry *m) {
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
	char *p = (char*) pb_alloc(a, 19, 1);
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
static const struct proto_field fields_TestMessage_MbyEntry[] = {
	{PROTO_U32, offsetof(struct TestMessage_MbyEntry, key), 8, NULL, -1},
	{PROTO_BYTES, offsetof(struct TestMessage_MbyEntry, value), 18, NULL, -1}
};
const struct proto_message pb_type_TestMessage_MbyEntry = {
	sizeof(struct TestMessage_MbyEntry),
	sizeof(fields_TestMessage_MbyEntry) / sizeof(struct proto_field),
	fields_TestMessage_MbyEntry
};

char *pb_parse_TestMessage_MstrEntry(char *p, pb_buf_t *obj, struct TestMessage_MstrEntry *m) {
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
	char *p = (char*) pb_alloc(a, 19, 1);
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
static const struct proto_field fields_TestMessage_MstrEntry[] = {
	{PROTO_U32, offsetof(struct TestMessage_MstrEntry, key), 8, NULL, -1},
	{PROTO_STRING, offsetof(struct TestMessage_MstrEntry, value), 18, NULL, -1}
};
const struct proto_message pb_type_TestMessage_MstrEntry = {
	sizeof(struct TestMessage_MstrEntry),
	sizeof(fields_TestMessage_MstrEntry) / sizeof(struct proto_field),
	fields_TestMessage_MstrEntry
};

bool pb_nonzero_TestMessage_MenEntry(struct TestMessage_MenEntry const *m) {
	return false
	    || m->key
	    || m->value;
}
char *pb_parse_TestMessage_MenEntry(char *p, struct TestMessage_MenEntry *m) {
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
	char *p = (char*) pb_alloc(a, 37, 1);
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
static const struct proto_field fields_TestMessage_MenEntry[] = {
	{PROTO_U32, offsetof(struct TestMessage_MenEntry, key), 8, NULL, -1},
	{PROTO_ENUM, offsetof(struct TestMessage_MenEntry, value), 16, NULL, -1}
};
const struct proto_message pb_type_TestMessage_MenEntry = {
	sizeof(struct TestMessage_MenEntry),
	sizeof(fields_TestMessage_MenEntry) / sizeof(struct proto_field),
	fields_TestMessage_MenEntry
};

char *pb_parse_TestMessage_MmsgEntry(char *p, pb_buf_t *obj, struct TestMessage_MmsgEntry *m) {
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
	char *p = (char*) pb_alloc(a, 19, 1);
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
static const struct proto_field fields_TestMessage_MmsgEntry[] = {
	{PROTO_U32, offsetof(struct TestMessage_MmsgEntry, key), 8, NULL, -1},
	{PROTO_MESSAGE, offsetof(struct TestMessage_MmsgEntry, value), 18, &pb_type_TestMessage, -1}
};
const struct proto_message pb_type_TestMessage_MmsgEntry = {
	sizeof(struct TestMessage_MmsgEntry),
	sizeof(fields_TestMessage_MmsgEntry) / sizeof(struct proto_field),
	fields_TestMessage_MmsgEntry
};

bool pb_nonzero_TestMessage_MpodEntry(struct TestMessage_MpodEntry const *m) {
	return false
	    || m->key
	    || pb_nonzero_TestPod(&m->value);
}
char *pb_parse_TestMessage_MpodEntry(char *p, struct TestMessage_MpodEntry *m) {
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
	char *p = (char*) pb_alloc(a, 19, 1);
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
static const struct proto_field fields_TestMessage_MpodEntry[] = {
	{PROTO_U32, offsetof(struct TestMessage_MpodEntry, key), 8, NULL, -1},
	{PROTO_POD, offsetof(struct TestMessage_MpodEntry, value), 18, &pb_type_TestPod, -1}
};
const struct proto_message pb_type_TestMessage_MpodEntry = {
	sizeof(struct TestMessage_MpodEntry),
	sizeof(fields_TestMessage_MpodEntry) / sizeof(struct proto_field),
	fields_TestMessage_MpodEntry
};

bool pb_nonzero_TestPod(struct TestPod const *m) {
	return false
	    || m->foo_type
	    || m->foo.u;
}
char *pb_parse_TestPod(char *p, struct TestPod *m) {
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
	char *p = (char*) pb_alloc(a, 33, 1);
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
static const struct proto_field fields_TestPod[] = {
	{PROTO_U32, offsetof(struct TestPod, foo), 8, NULL, offsetof(struct TestPod, foo_type)},
	{PROTO_S32, offsetof(struct TestPod, foo), 16, NULL, offsetof(struct TestPod, foo_type)}
};
const struct proto_message pb_type_TestPod = {
	sizeof(struct TestPod),
	sizeof(fields_TestPod) / sizeof(struct proto_field),
	fields_TestPod
};

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
char *pb_parse_TestEnum(char *p, enum TestEnum *v) {
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
int rpc_TestService(struct TestService* rpc, const char *path, char *body, pb_buf_t *resp, pb_buf_t *obj) {
	switch (pr_hash(path, 13) % 5) {
	case 2:
		if(strcmp(path, "/TestService/Test")) {
			return 404;
		} else if (!rpc->Test) {
			return 501;
		} else {
			struct TestMessage in;
			memset(&in, 0, sizeof(in));
			if (pb_parse_TestMessage(body, obj, &in) == pb_errret) {
				return 200;
			}
			struct TestMessage out;
			memset(&out, 0, sizeof(out));
			int ret = rpc->Test(rpc, obj, &in, &out); 
			if (pb_print_TestMessage(resp, &out)) {
				return 500;
			}
			return ret;
		}
	default:
		return 404;
	}
}
