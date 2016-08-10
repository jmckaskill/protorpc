#include "protocol.hpp"
#include <protorpc/protorpc.hpp>

uint8_t *pb_print(uint8_t *p, ::rpc::example::TestEnum v){
	switch (int32_t(v)) {
	case 0:
		return pb_print_str(p, "\"ENUM_A\",");
	case 1:
		return pb_print_str(p, "\"ENUM_B\",");
	case 2:
		return pb_print_str(p, "\"ENUM_C\",");
	default:
		return pb_print_str(p, "\"\",");
	}
}
uint8_t *pb_parse(uint8_t *p, ::rpc::example::TestEnum *v) {
	pb_string val;
	*v = ::rpc::example::TestEnum(0);
	switch (pb_parse_enum(&p, &val, 4) % 6) {
	case 1:
		if (val.equals("ENUM_A")) {*v = ::rpc::example::TestEnum(0);}
		break;
	case 2:
		if (val.equals("ENUM_B")) {*v = ::rpc::example::TestEnum(1);}
		break;
	case 3:
		if (val.equals("ENUM_C")) {*v = ::rpc::example::TestEnum(2);}
		break;
	}
	return p;
}

uint8_t *pb_parse_msg(uint8_t *p, pb_objalloc *obj, ::rpc::example::TestRequest *m) {
	if (!pb_parse_multi(&p, '{')) {return p;}
	for (;;) {
		pb_string key;
		switch (pb_parse_field(&p, &key, 3712286036) % 321) {
		case 0:
			return p;
		case 98:
			if (!key.equals("b")) {goto unknown;}
			p = pb_parse(p, &m->b);
			continue;
		case 267:
			if (!key.equals("u32")) {goto unknown;}
			p = pb_parse(p, &m->u32);
			continue;
		case 3:
			if (!key.equals("u64")) {goto unknown;}
			p = pb_parse(p, &m->u64);
			continue;
		case 166:
			if (!key.equals("i32")) {goto unknown;}
			p = pb_parse(p, &m->i32);
			continue;
		case 223:
			if (!key.equals("i64")) {goto unknown;}
			p = pb_parse(p, &m->i64);
			continue;
		case 281:
			if (!key.equals("s32")) {goto unknown;}
			p = pb_parse(p, &m->s32);
			continue;
		case 17:
			if (!key.equals("s64")) {goto unknown;}
			p = pb_parse(p, &m->s64);
			continue;
		case 255:
			if (!key.equals("f32")) {goto unknown;}
			p = pb_parse(p, &m->f32);
			continue;
		case 312:
			if (!key.equals("f64")) {goto unknown;}
			p = pb_parse(p, &m->f64);
			continue;
		case 275:
			if (!key.equals("sf32")) {goto unknown;}
			p = pb_parse(p, &m->sf32);
			continue;
		case 196:
			if (!key.equals("sf64")) {goto unknown;}
			p = pb_parse(p, &m->sf64);
			continue;
		case 102:
			if (!key.equals("f")) {goto unknown;}
			p = pb_parse(p, &m->f);
			continue;
		case 100:
			if (!key.equals("d")) {goto unknown;}
			p = pb_parse(p, &m->d);
			continue;
		case 128:
			if (!key.equals("by")) {goto unknown;}
			p = pb_parse(p, &m->by);
			continue;
		case 242:
			if (!key.equals("str")) {goto unknown;}
			p = pb_parse(p, &m->str);
			continue;
		case 36:
			if (!key.equals("en")) {goto unknown;}
			p = pb_parse(p, &m->en);
			continue;
		case 222:
			if (!key.equals("msg")) {goto unknown;}
			m->msg = obj->append<::rpc::example::TestRequest>();
			p = pb_parse_msg(p, obj, const_cast<::rpc::example::TestRequest*>(m->msg));
			continue;
		case 209:
			if (!key.equals("pod")) {goto unknown;}
			p = pb_parse(p, &m->pod);
			continue;
		case 159:
			if (!key.equals("rb")) {goto unknown;}
			p = pb_parse_array(p, obj, &m->rb);
			continue;
		case 230:
			if (!key.equals("ru32")) {goto unknown;}
			p = pb_parse_array(p, obj, &m->ru32);
			continue;
		case 151:
			if (!key.equals("ru64")) {goto unknown;}
			p = pb_parse_array(p, obj, &m->ru64);
			continue;
		case 129:
			if (!key.equals("ri32")) {goto unknown;}
			p = pb_parse_array(p, obj, &m->ri32);
			continue;
		case 50:
			if (!key.equals("ri64")) {goto unknown;}
			p = pb_parse_array(p, obj, &m->ri64);
			continue;
		case 244:
			if (!key.equals("rs32")) {goto unknown;}
			p = pb_parse_array(p, obj, &m->rs32);
			continue;
		case 165:
			if (!key.equals("rs64")) {goto unknown;}
			p = pb_parse_array(p, obj, &m->rs64);
			continue;
		case 218:
			if (!key.equals("rf32")) {goto unknown;}
			p = pb_parse_array(p, obj, &m->rf32);
			continue;
		case 139:
			if (!key.equals("rf64")) {goto unknown;}
			p = pb_parse_array(p, obj, &m->rf64);
			continue;
		case 38:
			if (!key.equals("rsf32")) {goto unknown;}
			p = pb_parse_array(p, obj, &m->rsf32);
			continue;
		case 280:
			if (!key.equals("rsf64")) {goto unknown;}
			p = pb_parse_array(p, obj, &m->rsf64);
			continue;
		case 163:
			if (!key.equals("rf")) {goto unknown;}
			p = pb_parse_array(p, obj, &m->rf);
			continue;
		case 161:
			if (!key.equals("rd")) {goto unknown;}
			p = pb_parse_array(p, obj, &m->rd);
			continue;
		case 81:
			if (!key.equals("rby")) {goto unknown;}
			p = pb_parse_array(p, obj, &m->rby);
			continue;
		case 205:
			if (!key.equals("rstr")) {goto unknown;}
			p = pb_parse_array(p, obj, &m->rstr);
			continue;
		case 125:
			if (!key.equals("ren")) {goto unknown;}
			p = pb_parse_array(p, obj, &m->ren);
			continue;
		case 49:
			if (!key.equals("rmsg")) {goto unknown;}
			p = pb_parse_array(p, obj, &m->rmsg);
			continue;
		case 172:
			if (!key.equals("rpod")) {goto unknown;}
			p = pb_parse_array(p, obj, &m->rpod);
			continue;
		case 51:
			if (!key.equals("mb")) {goto unknown;}
			p = pb_parse_map(p, obj, &m->mb);
			continue;
		case 315:
			if (!key.equals("mu32")) {goto unknown;}
			p = pb_parse_map(p, obj, &m->mu32);
			continue;
		case 236:
			if (!key.equals("mu64")) {goto unknown;}
			p = pb_parse_map(p, obj, &m->mu64);
			continue;
		case 214:
			if (!key.equals("mi32")) {goto unknown;}
			p = pb_parse_map(p, obj, &m->mi32);
			continue;
		case 135:
			if (!key.equals("mi64")) {goto unknown;}
			p = pb_parse_map(p, obj, &m->mi64);
			continue;
		case 8:
			if (!key.equals("ms32")) {goto unknown;}
			p = pb_parse_map(p, obj, &m->ms32);
			continue;
		case 250:
			if (!key.equals("ms64")) {goto unknown;}
			p = pb_parse_map(p, obj, &m->ms64);
			continue;
		case 303:
			if (!key.equals("mf32")) {goto unknown;}
			p = pb_parse_map(p, obj, &m->mf32);
			continue;
		case 224:
			if (!key.equals("mf64")) {goto unknown;}
			p = pb_parse_map(p, obj, &m->mf64);
			continue;
		case 276:
			if (!key.equals("msf32")) {goto unknown;}
			p = pb_parse_map(p, obj, &m->msf32);
			continue;
		case 12:
			if (!key.equals("msf64")) {goto unknown;}
			p = pb_parse_map(p, obj, &m->msf64);
			continue;
		case 55:
			if (!key.equals("mf")) {goto unknown;}
			p = pb_parse_map(p, obj, &m->mf);
			continue;
		case 53:
			if (!key.equals("md")) {goto unknown;}
			p = pb_parse_map(p, obj, &m->md);
			continue;
		case 184:
			if (!key.equals("mby")) {goto unknown;}
			p = pb_parse_map(p, obj, &m->mby);
			continue;
		case 290:
			if (!key.equals("mstr")) {goto unknown;}
			p = pb_parse_map(p, obj, &m->mstr);
			continue;
		case 228:
			if (!key.equals("men")) {goto unknown;}
			p = pb_parse_map(p, obj, &m->men);
			continue;
		case 134:
			if (!key.equals("mmsg")) {goto unknown;}
			p = pb_parse_map(p, obj, &m->mmsg);
			continue;
		case 257:
			if (!key.equals("mpod")) {goto unknown;}
			p = pb_parse_map(p, obj, &m->mpod);
			continue;
		default:
		unknown:
			p = pb_parse_value(p);
		}
	}
}
void pb_print_msg(pb_buffer *out, ::rpc::example::TestRequest const *m){
	uint8_t *p = pb_print_start(out, 298);
	if (m->b) {
		p = pb_print_str(p, "\"b\":");
		p = pb_print(p, m->b);
	}
	if (m->u32) {
		p = pb_print_str(p, "\"u32\":");
		p = pb_print(p, m->u32);
	}
	if (m->u64) {
		p = pb_print_str(p, "\"u64\":");
		p = pb_print(p, m->u64);
	}
	if (m->i32) {
		p = pb_print_str(p, "\"i32\":");
		p = pb_print(p, m->i32);
	}
	if (m->i64) {
		p = pb_print_str(p, "\"i64\":");
		p = pb_print(p, m->i64);
	}
	if (m->s32) {
		p = pb_print_str(p, "\"s32\":");
		p = pb_print(p, m->s32);
	}
	if (m->s64) {
		p = pb_print_str(p, "\"s64\":");
		p = pb_print(p, m->s64);
	}
	if (m->f32) {
		p = pb_print_str(p, "\"f32\":");
		p = pb_print(p, m->f32);
	}
	if (m->f64) {
		p = pb_print_str(p, "\"f64\":");
		p = pb_print(p, m->f64);
	}
	if (m->sf32) {
		p = pb_print_str(p, "\"sf32\":");
		p = pb_print(p, m->sf32);
	}
	if (m->sf64) {
		p = pb_print_str(p, "\"sf64\":");
		p = pb_print(p, m->sf64);
	}
	if (m->f) {
		p = pb_print_str(p, "\"f\":");
		p = pb_print(p, m->f);
	}
	if (m->d) {
		p = pb_print_str(p, "\"d\":");
		p = pb_print(p, m->d);
	}
	if (m->en) {
		p = pb_print_str(p, "\"en\":");
		p = pb_print(p, m->en);
	}
	out->append_finish(p);
	if (m->by.size()) {
		pb_print_str(out, "\"by\":");
		pb_print_msg(out, m->by);
	}
	if (m->str.size()) {
		pb_print_str(out, "\"str\":");
		pb_print_msg(out, m->str);
	}
	if (m->msg) {
		pb_print_str(out, "\"msg\":");
		pb_print_msg(out, m->msg);
	}
	if (m->pod != ::rpc::example::TestPod()) {
		pb_print_str(out, "\"pod\":");
		pb_print_msg(out, m->pod);
	}
	if (m->rb.size()) {
		pb_print_str(out, "\"rb\":");
		pb_print_array(out, m->rb, 1);
	}
	if (m->ru32.size()) {
		pb_print_str(out, "\"ru32\":");
		pb_print_array(out, m->ru32, 5);
	}
	if (m->ru64.size()) {
		pb_print_str(out, "\"ru64\":");
		pb_print_array(out, m->ru64, 10);
	}
	if (m->ri32.size()) {
		pb_print_str(out, "\"ri32\":");
		pb_print_array(out, m->ri32, 5);
	}
	if (m->ri64.size()) {
		pb_print_str(out, "\"ri64\":");
		pb_print_array(out, m->ri64, 10);
	}
	if (m->rs32.size()) {
		pb_print_str(out, "\"rs32\":");
		pb_print_array(out, m->rs32, 5);
	}
	if (m->rs64.size()) {
		pb_print_str(out, "\"rs64\":");
		pb_print_array(out, m->rs64, 10);
	}
	if (m->rf32.size()) {
		pb_print_str(out, "\"rf32\":");
		pb_print_array(out, m->rf32, 4);
	}
	if (m->rf64.size()) {
		pb_print_str(out, "\"rf64\":");
		pb_print_array(out, m->rf64, 8);
	}
	if (m->rsf32.size()) {
		pb_print_str(out, "\"rsf32\":");
		pb_print_array(out, m->rsf32, 4);
	}
	if (m->rsf64.size()) {
		pb_print_str(out, "\"rsf64\":");
		pb_print_array(out, m->rsf64, 8);
	}
	if (m->rf.size()) {
		pb_print_str(out, "\"rf\":");
		pb_print_array(out, m->rf, 4);
	}
	if (m->rd.size()) {
		pb_print_str(out, "\"rd\":");
		pb_print_array(out, m->rd, 8);
	}
	if (m->rby.size()) {
		pb_print_str(out, "\"rby\":");
		pb_print_array(out, m->rby);
	}
	if (m->rstr.size()) {
		pb_print_str(out, "\"rstr\":");
		pb_print_array(out, m->rstr);
	}
	if (m->ren.size()) {
		pb_print_str(out, "\"ren\":");
		pb_print_array(out, m->ren, 5);
	}
	if (m->rmsg.size()) {
		pb_print_str(out, "\"rmsg\":");
		pb_print_array(out, m->rmsg);
	}
	if (m->rpod.size()) {
		pb_print_str(out, "\"rpod\":");
		pb_print_array(out, m->rpod);
	}
	if (m->mb.size()) {
		pb_print_str(out, "\"mb\":");
		pb_print_map(out, m->mb);
	}
	if (m->mu32.size()) {
		pb_print_str(out, "\"mu32\":");
		pb_print_map(out, m->mu32);
	}
	if (m->mu64.size()) {
		pb_print_str(out, "\"mu64\":");
		pb_print_map(out, m->mu64);
	}
	if (m->mi32.size()) {
		pb_print_str(out, "\"mi32\":");
		pb_print_map(out, m->mi32);
	}
	if (m->mi64.size()) {
		pb_print_str(out, "\"mi64\":");
		pb_print_map(out, m->mi64);
	}
	if (m->ms32.size()) {
		pb_print_str(out, "\"ms32\":");
		pb_print_map(out, m->ms32);
	}
	if (m->ms64.size()) {
		pb_print_str(out, "\"ms64\":");
		pb_print_map(out, m->ms64);
	}
	if (m->mf32.size()) {
		pb_print_str(out, "\"mf32\":");
		pb_print_map(out, m->mf32);
	}
	if (m->mf64.size()) {
		pb_print_str(out, "\"mf64\":");
		pb_print_map(out, m->mf64);
	}
	if (m->msf32.size()) {
		pb_print_str(out, "\"msf32\":");
		pb_print_map(out, m->msf32);
	}
	if (m->msf64.size()) {
		pb_print_str(out, "\"msf64\":");
		pb_print_map(out, m->msf64);
	}
	if (m->mf.size()) {
		pb_print_str(out, "\"mf\":");
		pb_print_map(out, m->mf);
	}
	if (m->md.size()) {
		pb_print_str(out, "\"md\":");
		pb_print_map(out, m->md);
	}
	if (m->mby.size()) {
		pb_print_str(out, "\"mby\":");
		pb_print_map(out, m->mby);
	}
	if (m->mstr.size()) {
		pb_print_str(out, "\"mstr\":");
		pb_print_map(out, m->mstr);
	}
	if (m->men.size()) {
		pb_print_str(out, "\"men\":");
		pb_print_map(out, m->men);
	}
	if (m->mmsg.size()) {
		pb_print_str(out, "\"mmsg\":");
		pb_print_map(out, m->mmsg);
	}
	if (m->mpod.size()) {
		pb_print_str(out, "\"mpod\":");
		pb_print_map(out, m->mpod);
	}
	pb_print_end(out);
}
char *pb_parse_query(char *p, pb_objalloc *obj, ::rpc::example::TestRequest *m) {
	for (;;) {
		pb_string key;
		switch (pb_next_query(&p, &key, 3712286036) % 321) {
		case 0:
			return p;
		case 98:
			if (!key.equals("b")) {goto unknown;}
			p = pb_parse_query(p, &m->b);
			break;
		case 267:
			if (!key.equals("u32")) {goto unknown;}
			p = pb_parse_query(p, &m->u32);
			break;
		case 3:
			if (!key.equals("u64")) {goto unknown;}
			p = pb_parse_query(p, &m->u64);
			break;
		case 166:
			if (!key.equals("i32")) {goto unknown;}
			p = pb_parse_query(p, &m->i32);
			break;
		case 223:
			if (!key.equals("i64")) {goto unknown;}
			p = pb_parse_query(p, &m->i64);
			break;
		case 281:
			if (!key.equals("s32")) {goto unknown;}
			p = pb_parse_query(p, &m->s32);
			break;
		case 17:
			if (!key.equals("s64")) {goto unknown;}
			p = pb_parse_query(p, &m->s64);
			break;
		case 255:
			if (!key.equals("f32")) {goto unknown;}
			p = pb_parse_query(p, &m->f32);
			break;
		case 312:
			if (!key.equals("f64")) {goto unknown;}
			p = pb_parse_query(p, &m->f64);
			break;
		case 275:
			if (!key.equals("sf32")) {goto unknown;}
			p = pb_parse_query(p, &m->sf32);
			break;
		case 196:
			if (!key.equals("sf64")) {goto unknown;}
			p = pb_parse_query(p, &m->sf64);
			break;
		case 102:
			if (!key.equals("f")) {goto unknown;}
			p = pb_parse_query(p, &m->f);
			break;
		case 100:
			if (!key.equals("d")) {goto unknown;}
			p = pb_parse_query(p, &m->d);
			break;
		case 128:
			if (!key.equals("by")) {goto unknown;}
			p = pb_parse_query(p, &m->by);
			break;
		case 242:
			if (!key.equals("str")) {goto unknown;}
			p = pb_parse_query(p, &m->str);
			break;
		default:
		unknown:
			p = pb_skip_query(p);
		}
	}
}
void pb_print_query(pb_buffer *out, ::rpc::example::TestRequest const *m){
	if (m->b) {
		pb_print_query(out, "b=", 2, m->b);
	}
	if (m->u32) {
		pb_print_query(out, "u32=", 4, m->u32);
	}
	if (m->u64) {
		pb_print_query(out, "u64=", 4, m->u64);
	}
	if (m->i32) {
		pb_print_query(out, "i32=", 4, m->i32);
	}
	if (m->i64) {
		pb_print_query(out, "i64=", 4, m->i64);
	}
	if (m->s32) {
		pb_print_query(out, "s32=", 4, m->s32);
	}
	if (m->s64) {
		pb_print_query(out, "s64=", 4, m->s64);
	}
	if (m->f32) {
		pb_print_query(out, "f32=", 4, m->f32);
	}
	if (m->f64) {
		pb_print_query(out, "f64=", 4, m->f64);
	}
	if (m->sf32) {
		pb_print_query(out, "sf32=", 5, m->sf32);
	}
	if (m->sf64) {
		pb_print_query(out, "sf64=", 5, m->sf64);
	}
	if (m->f) {
		pb_print_query(out, "f=", 2, m->f);
	}
	if (m->d) {
		pb_print_query(out, "d=", 2, m->d);
	}
	if (m->by.size()) {
		pb_print_query(out, "by=", 3, m->by);
	}
	if (m->str.size()) {
		pb_print_query(out, "str=", 4, m->str);
	}
	if (m->en) {
		pb_print_query(out, "en=", 3, m->en);
	}
	if (m->rb.size()) {
		pb_print_query(out, "rb=", 3, m->rb);
	}
	if (m->ru32.size()) {
		pb_print_query(out, "ru32=", 5, m->ru32);
	}
	if (m->ru64.size()) {
		pb_print_query(out, "ru64=", 5, m->ru64);
	}
	if (m->ri32.size()) {
		pb_print_query(out, "ri32=", 5, m->ri32);
	}
	if (m->ri64.size()) {
		pb_print_query(out, "ri64=", 5, m->ri64);
	}
	if (m->rs32.size()) {
		pb_print_query(out, "rs32=", 5, m->rs32);
	}
	if (m->rs64.size()) {
		pb_print_query(out, "rs64=", 5, m->rs64);
	}
	if (m->rf32.size()) {
		pb_print_query(out, "rf32=", 5, m->rf32);
	}
	if (m->rf64.size()) {
		pb_print_query(out, "rf64=", 5, m->rf64);
	}
	if (m->rsf32.size()) {
		pb_print_query(out, "rsf32=", 6, m->rsf32);
	}
	if (m->rsf64.size()) {
		pb_print_query(out, "rsf64=", 6, m->rsf64);
	}
	if (m->rf.size()) {
		pb_print_query(out, "rf=", 3, m->rf);
	}
	if (m->rd.size()) {
		pb_print_query(out, "rd=", 3, m->rd);
	}
	if (m->rby.size()) {
		pb_print_query(out, "rby=", 4, m->rby);
	}
	if (m->rstr.size()) {
		pb_print_query(out, "rstr=", 5, m->rstr);
	}
	if (m->ren.size()) {
		pb_print_query(out, "ren=", 4, m->ren);
	}
}

uint8_t *pb_parse(uint8_t *p, ::rpc::example::TestPod *m) {
	if (!pb_parse_multi(&p, '{')) {return p;}
	for (;;) {
		pb_string key;
		switch (pb_parse_field(&p, &key, 121) % 8) {
		case 0:
			return p;
		case 5:
			if (!key.equals("u")) {goto unknown;}
			m->en_foo = ::rpc::example::TestPod::U;
			p = pb_parse(p, &m->foo.u);
			continue;
		case 1:
			if (!key.equals("i")) {goto unknown;}
			m->en_foo = ::rpc::example::TestPod::I;
			p = pb_parse(p, &m->foo.i);
			continue;
		default:
		unknown:
			p = pb_parse_value(p);
		}
	}
}
void pb_print_msg(pb_buffer *out, ::rpc::example::TestPod const &v){
	uint8_t *p = pb_print_start(out, 30);
	if (v.en_foo == ::rpc::example::TestPod::U) {
		p = pb_print_str(p, "\"u\":");
		p = pb_print(p, v.foo.u);
	}
	if (v.en_foo == ::rpc::example::TestPod::I) {
		p = pb_print_str(p, "\"i\":");
		p = pb_print(p, v.foo.i);
	}
	out->append_finish(p);
	pb_print_end(out);
}
char *pb_parse_query(char *p, ::rpc::example::TestPod *m) {
	for (;;) {
		pb_string key;
		switch (pb_next_query(&p, &key, 121) % 8) {
		case 0:
			return p;
		case 5:
			if (!key.equals("u")) {goto unknown;}
			m->en_foo = ::rpc::example::TestPod::U;
			p = pb_parse_query(p, &m->foo.u);
			break;
		case 1:
			if (!key.equals("i")) {goto unknown;}
			m->en_foo = ::rpc::example::TestPod::I;
			p = pb_parse_query(p, &m->foo.i);
			break;
		default:
		unknown:
			p = pb_skip_query(p);
		}
	}
}
void pb_print_query(pb_buffer *out, ::rpc::example::TestPod const &v){
	if (v.en_foo == ::rpc::example::TestPod::U) {
		pb_print_query(out, "u=", 2, v.foo.u);
	}
	if (v.en_foo == ::rpc::example::TestPod::I) {
		pb_print_query(out, "i=", 2, v.foo.i);
	}
}
bool operator==(::rpc::example::TestPod const& a, ::rpc::example::TestPod const& b) {
	return true
		&& a.en_foo == b.en_foo
		&& (a.en_foo != ::rpc::example::TestPod::U || a.foo.u == b.foo.u)
		&& (a.en_foo != ::rpc::example::TestPod::I || a.foo.i == b.foo.i);
}

uint8_t *pb_parse_msg(uint8_t *p, pb_objalloc *obj, ::rpc::example::LoginRequest *m) {
	if (!pb_parse_multi(&p, '{')) {return p;}
	for (;;) {
		pb_string key;
		switch (pb_parse_field(&p, &key, 364) % 10) {
		case 0:
			return p;
		case 7:
			if (!key.equals("username")) {goto unknown;}
			p = pb_parse(p, &m->username);
			continue;
		case 2:
			if (!key.equals("password")) {goto unknown;}
			p = pb_parse(p, &m->password);
			continue;
		case 8:
			if (!key.equals("expected")) {goto unknown;}
			m->expected = obj->append<::rpc::example::LoginReply>();
			p = pb_parse_msg(p, obj, const_cast<::rpc::example::LoginReply*>(m->expected));
			continue;
		default:
		unknown:
			p = pb_parse_value(p);
		}
	}
}
void pb_print_msg(pb_buffer *out, ::rpc::example::LoginRequest const *m){
	uint8_t *p = pb_print_start(out, 0);
	out->append_finish(p);
	if (m->username.size()) {
		pb_print_str(out, "\"username\":");
		pb_print_msg(out, m->username);
	}
	if (m->password.size()) {
		pb_print_str(out, "\"password\":");
		pb_print_msg(out, m->password);
	}
	if (m->expected) {
		pb_print_str(out, "\"expected\":");
		pb_print_msg(out, m->expected);
	}
	pb_print_end(out);
}
char *pb_parse_query(char *p, pb_objalloc *obj, ::rpc::example::LoginRequest *m) {
	for (;;) {
		pb_string key;
		switch (pb_next_query(&p, &key, 364) % 10) {
		case 0:
			return p;
		case 7:
			if (!key.equals("username")) {goto unknown;}
			p = pb_parse_query(p, &m->username);
			break;
		case 2:
			if (!key.equals("password")) {goto unknown;}
			p = pb_parse_query(p, &m->password);
			break;
		default:
		unknown:
			p = pb_skip_query(p);
		}
	}
}
void pb_print_query(pb_buffer *out, ::rpc::example::LoginRequest const *m){
	if (m->username.size()) {
		pb_print_query(out, "username=", 9, m->username);
	}
	if (m->password.size()) {
		pb_print_query(out, "password=", 9, m->password);
	}
}

uint8_t *pb_parse_msg(uint8_t *p, pb_objalloc *obj, ::rpc::example::LoginReply *m) {
	if (!pb_parse_multi(&p, '{')) {return p;}
	for (;;) {
		pb_string key;
		switch (pb_parse_field(&p, &key, 13) % 7) {
		case 0:
			return p;
		case 2:
			if (!key.equals("token")) {goto unknown;}
			p = pb_parse(p, &m->token);
			continue;
		case 6:
			if (!key.equals("request")) {goto unknown;}
			m->request = obj->append<::rpc::example::LoginRequest>();
			p = pb_parse_msg(p, obj, const_cast<::rpc::example::LoginRequest*>(m->request));
			continue;
		case 1:
			if (!key.equals("numbers")) {goto unknown;}
			p = pb_parse_array(p, obj, &m->numbers);
			continue;
		default:
		unknown:
			p = pb_parse_value(p);
		}
	}
}
void pb_print_msg(pb_buffer *out, ::rpc::example::LoginReply const *m){
	uint8_t *p = pb_print_start(out, 0);
	out->append_finish(p);
	if (m->token.size()) {
		pb_print_str(out, "\"token\":");
		pb_print_msg(out, m->token);
	}
	if (m->request) {
		pb_print_str(out, "\"request\":");
		pb_print_msg(out, m->request);
	}
	if (m->numbers.size()) {
		pb_print_str(out, "\"numbers\":");
		pb_print_array(out, m->numbers, 5);
	}
	pb_print_end(out);
}
char *pb_parse_query(char *p, pb_objalloc *obj, ::rpc::example::LoginReply *m) {
	for (;;) {
		pb_string key;
		switch (pb_next_query(&p, &key, 13) % 7) {
		case 0:
			return p;
		case 2:
			if (!key.equals("token")) {goto unknown;}
			p = pb_parse_query(p, &m->token);
			break;
		default:
		unknown:
			p = pb_skip_query(p);
		}
	}
}
void pb_print_query(pb_buffer *out, ::rpc::example::LoginReply const *m){
	if (m->token.size()) {
		pb_print_query(out, "token=", 6, m->token);
	}
	if (m->numbers.size()) {
		pb_print_query(out, "numbers=", 8, m->numbers);
	}
}

uint8_t *pb_parse(uint8_t *p, ::rpc::example::LookupDesignRequest *m) {
	if (!pb_parse_multi(&p, '{')) {return p;}
	for (;;) {
		pb_string key;
		switch (pb_parse_field(&p, &key, 1) % 4) {
		case 0:
			return p;
		case 1:
			if (!key.equals("northing")) {goto unknown;}
			p = pb_parse(p, &m->northing);
			continue;
		case 3:
			if (!key.equals("easting")) {goto unknown;}
			p = pb_parse(p, &m->easting);
			continue;
		default:
		unknown:
			p = pb_parse_value(p);
		}
	}
}
void pb_print_msg(pb_buffer *out, ::rpc::example::LookupDesignRequest const &v){
	uint8_t *p = pb_print_start(out, 63);
	if (v.northing) {
		p = pb_print_str(p, "\"northing\":");
		p = pb_print(p, v.northing);
	}
	if (v.easting) {
		p = pb_print_str(p, "\"easting\":");
		p = pb_print(p, v.easting);
	}
	out->append_finish(p);
	pb_print_end(out);
}
char *pb_parse_query(char *p, ::rpc::example::LookupDesignRequest *m) {
	for (;;) {
		pb_string key;
		switch (pb_next_query(&p, &key, 1) % 4) {
		case 0:
			return p;
		case 1:
			if (!key.equals("northing")) {goto unknown;}
			p = pb_parse_query(p, &m->northing);
			break;
		case 3:
			if (!key.equals("easting")) {goto unknown;}
			p = pb_parse_query(p, &m->easting);
			break;
		default:
		unknown:
			p = pb_skip_query(p);
		}
	}
}
void pb_print_query(pb_buffer *out, ::rpc::example::LookupDesignRequest const &v){
	if (v.northing) {
		pb_print_query(out, "northing=", 9, v.northing);
	}
	if (v.easting) {
		pb_print_query(out, "easting=", 8, v.easting);
	}
}
bool operator==(::rpc::example::LookupDesignRequest const& a, ::rpc::example::LookupDesignRequest const& b) {
	return true
		&& a.northing == b.northing
		&& a.easting == b.easting;
}

uint8_t *pb_parse(uint8_t *p, ::rpc::example::LookupDesignReply *m) {
	if (!pb_parse_multi(&p, '{')) {return p;}
	for (;;) {
		pb_string key;
		switch (pb_parse_field(&p, &key, 1) % 3) {
		case 0:
			return p;
		case 1:
			if (!key.equals("elevation")) {goto unknown;}
			p = pb_parse(p, &m->elevation);
			continue;
		default:
		unknown:
			p = pb_parse_value(p);
		}
	}
}
void pb_print_msg(pb_buffer *out, ::rpc::example::LookupDesignReply const &v){
	uint8_t *p = pb_print_start(out, 33);
	if (v.elevation) {
		p = pb_print_str(p, "\"elevation\":");
		p = pb_print(p, v.elevation);
	}
	out->append_finish(p);
	pb_print_end(out);
}
char *pb_parse_query(char *p, ::rpc::example::LookupDesignReply *m) {
	for (;;) {
		pb_string key;
		switch (pb_next_query(&p, &key, 1) % 3) {
		case 0:
			return p;
		case 1:
			if (!key.equals("elevation")) {goto unknown;}
			p = pb_parse_query(p, &m->elevation);
			break;
		default:
		unknown:
			p = pb_skip_query(p);
		}
	}
}
void pb_print_query(pb_buffer *out, ::rpc::example::LookupDesignReply const &v){
	if (v.elevation) {
		pb_print_query(out, "elevation=", 10, v.elevation);
	}
}
bool operator==(::rpc::example::LookupDesignReply const& a, ::rpc::example::LookupDesignReply const& b) {
	return true
		&& a.elevation == b.elevation;
}
void ::rpc::example::TestService::handle(rpc_server *s, const char *path) {
	std::string p = path;
	size_t psz = p.size();
	p += "/Test";
	s->handle_post(p, &pb_Test, this);
	p.resize(psz);
	p += "/Login";
	s->handle_post(p, &pb_Login, this);
	p.resize(psz);
	p += "/LookupDesign";
	s->handle_post(p, &pb_LookupDesign, this);
	p.resize(psz);
	p += "/LookupStream";
	s->handle_stream(p, &pb_LookupStream, this);
}
int ::rpc::example::TestService::pb_Test(void *ud, pb_objalloc *obj, pb_buffer *out, char *body) {
	TestRequest r = {};
	TestRequest w = {};
	pb_parse_msg((uint8_t*)body, obj, &r);
	int sts = static_cast<TestService*>(ud)->Test(obj, &w, &r);
	pb_print_msg(out, &w);
	return sts;
}
int ::rpc::example::TestService::pb_Login(void *ud, pb_objalloc *obj, pb_buffer *out, char *body) {
	LoginRequest r = {};
	LoginReply w = {};
	pb_parse_msg((uint8_t*)body, obj, &r);
	int sts = static_cast<TestService*>(ud)->Login(obj, &w, &r);
	pb_print_msg(out, &w);
	return sts;
}
int ::rpc::example::TestService::pb_LookupDesign(void *ud, pb_objalloc *obj, pb_buffer *out, char *body) {
	LookupDesignRequest r = {};
	LookupDesignReply w = {};
	pb_parse_msg((uint8_t*)body, obj, &r);
	int sts = static_cast<TestService*>(ud)->LookupDesign(obj, &w, &r);
	pb_print_msg(out, w);
	return sts;
}
int ::rpc::example::TestService::pb_LookupStream(void *ud, pb_objalloc *obj, rpc_dispatch_base **out, char *query) {
	LookupDesignRequest r = {};
	auto w = reinterpret_cast<rpc_dispatch<LookupDesignReply>**>(out);
	pb_parse_query(query, &r);
	return static_cast<TestService*>(ud)->LookupStream(w, &r);
}
const ::rpc::example::TestRequest *::rpc::example::TestServiceClient::Test(pb_objalloc *obj, TestRequest const *r) {
	size_t hsz;
	pb_buffer *buf = init_request("/Test", 5, &hsz);
	pb_print_msg(buf, r);
	char *data = send_request(hsz);
	if (!data) {
		return NULL;
	}
	auto ret = obj->append<TestRequest>();
	pb_parse_msg((uint8_t*)data, obj, ret);
	return ret;
}
const ::rpc::example::LoginReply *::rpc::example::TestServiceClient::Login(pb_objalloc *obj, LoginRequest const *r) {
	size_t hsz;
	pb_buffer *buf = init_request("/Login", 6, &hsz);
	pb_print_msg(buf, r);
	char *data = send_request(hsz);
	if (!data) {
		return NULL;
	}
	auto ret = obj->append<LoginReply>();
	pb_parse_msg((uint8_t*)data, obj, ret);
	return ret;
}
const ::rpc::example::LookupDesignReply *::rpc::example::TestServiceClient::LookupDesign(pb_objalloc *obj, LookupDesignRequest const &r) {
	size_t hsz;
	pb_buffer *buf = init_request("/LookupDesign", 13, &hsz);
	pb_print_msg(buf, r);
	char *data = send_request(hsz);
	if (!data) {
		return NULL;
	}
	auto ret = obj->append<LookupDesignReply>();
	pb_parse_msg((uint8_t*)data, obj, ret);
	return ret;
}
bool ::rpc::example::TestServiceClient::LookupStream(rpc_stream<::rpc::example::LookupDesignReply>* s, LookupDesignRequest const &r) {
	pb_buffer *buf = init_stream("/LookupStream", 13, s);
	pb_print_query(buf, r);
	return send_stream();
}
