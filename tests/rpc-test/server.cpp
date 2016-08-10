#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "protocol.hpp"
#include <stdio.h>
#include <math.h>
#include <inttypes.h>
#include <protorpc/protorpc.hpp>
#include <thread>
#include <signal.h>

using namespace rpc::example;

rpc_dispatch<LookupDesignReply> g_design_broadcast;
void lookup_thread() {
	LookupDesignReply msg = {};
	for (;;) {
		std::this_thread::sleep_for(std::chrono::seconds(2));
		msg.elevation++;
		g_design_broadcast.publish(msg);
	}
}

class MyTestService : public TestService {
public:
	virtual int Test(pb_objalloc *obj, TestRequest *out, const TestRequest *in) {
		fprintf(stderr, "in test method\n");
		fprintf(stderr, "b %d\n", in->b);
		fprintf(stderr, "u32 %u\n", in->u32);
		fprintf(stderr, "u64 %" PRIu64"\n", in->u64);
		fprintf(stderr, "i32 %d\n", in->i32);
		fprintf(stderr, "i64 %" PRId64"\n", in->i64);
		fprintf(stderr, "s32 %d\n", in->s32);
		fprintf(stderr, "s64 %" PRId64"\n", in->s64);
		fprintf(stderr, "f32 %u\n", in->f32);
		fprintf(stderr, "f64 %" PRIu64"\n", in->f64);
		fprintf(stderr, "sf32 %d\n", in->sf32);
		fprintf(stderr, "sf64 %" PRId64"\n", in->sf64);
		fprintf(stderr, "f %g\n", in->f);
		fprintf(stderr, "d %g\n", in->d);
		fprintf(stderr, "by");
		for (size_t i = 0; i < in->by.size(); i++) {
			fprintf(stderr, "%02x", in->by[i]);
		}
		fprintf(stderr, "\nstr %.*s\n", (int)in->str.size(), &in->str[0]);
		fprintf(stderr, "en %d\n", in->en);
		fprintf(stderr, "rb %d\n", (int) in->rb.size());
		fprintf(stderr, "ru32 %d\n", (int) in->ru32.size());
		fprintf(stderr, "ru64 %d\n", (int) in->ru64.size());
		fprintf(stderr, "ri32 %d\n", (int) in->ri32.size());
		fprintf(stderr, "ri64 %d\n", (int) in->ri64.size());
		fprintf(stderr, "rs32 %d\n", (int) in->rs32.size());
		fprintf(stderr, "rs64 %d\n", (int) in->rs64.size());
		fprintf(stderr, "rf32 %d\n", (int) in->rf32.size());
		fprintf(stderr, "rf64 %d\n", (int) in->rf64.size());
		fprintf(stderr, "rsf32 %d\n", (int) in->rsf32.size());
		fprintf(stderr, "rsf64 %d\n", (int) in->rsf64.size());
		fprintf(stderr, "rf %d\n", (int) in->rf.size());
		fprintf(stderr, "rd %d\n", (int) in->rd.size());
		fprintf(stderr, "rby %d\n", (int) in->rby.size());
		fprintf(stderr, "rstr %d\n", (int) in->rstr.size());
		fprintf(stderr, "ren %d\n", (int) in->ren.size());
		fprintf(stderr, "rmsg %d\n", (int) in->rmsg.size());
		fprintf(stderr, "rmsg[0]->u32 %u\n", in->rmsg[0]->u32);
		fprintf(stderr, "rpod %d\n", (int) in->rpod.size());
		*out = *in;
		return 200;
	}
	virtual int Login(pb_objalloc *obj, LoginReply* out, const LoginRequest* in) {
		//std::cerr << "login request " << in << std::endl;
		out->token.setref(&in->username[0], in->username.size());
		out->request = in;
		uint32_t* num = out->numbers.create(obj, 3);
		for (size_t i = 0; i < 3; i++) {
			num[i] = (uint32_t) rand();
		}
		return 200;
	}
	virtual int LookupDesign(pb_objalloc *obj, LookupDesignReply* out, const LookupDesignRequest* in) {
		return 200;
	}
	virtual int LookupStream(rpc_dispatch<LookupDesignReply> **out, const LookupDesignRequest* r) {
		fprintf(stderr, "have subscription\n");;
		*out = &g_design_broadcast;
		return 200;
	}
};

int main() {
#ifndef _WIN32
	signal(SIGPIPE, SIG_IGN);
#endif
	fprintf(stderr, "starting up\n");

	std::thread lookup(&lookup_thread);
	lookup.detach();

	rpc_server serv;

	MyTestService service;
	service.handle(&serv, "/api");

	serv.listen_and_serve("localhost", "8080");
	return 1;
}