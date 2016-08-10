#pragma once
#include <protorpc/types.hpp>
#include <protorpc/rpc.hpp>
namespace rpc {
namespace example {
struct TestRequest;
struct TestPod;
struct LoginRequest;
struct LoginReply;
struct LookupDesignRequest;
struct LookupDesignReply;
enum TestEnum : int32_t {
	ENUM_A    = 0,
	ENUM_B    = 1,
	ENUM_C    = 2
};
struct TestPod {
	enum en_foo {
		U,
		I
	} en_foo;
	union {
		uint32_t u;
		int32_t i;
	} foo;
};
struct TestRequest {
	mutable pb_msg                 pb_hdr;
	bool                           b;
	uint32_t                       u32;
	uint64_t                       u64;
	int32_t                        i32;
	int64_t                        i64;
	int32_t                        s32;
	int64_t                        s64;
	uint32_t                       f32;
	uint64_t                       f64;
	int32_t                        sf32;
	int64_t                        sf64;
	float                          f;
	double                         d;
	pb_bytes                       by;
	pb_string                      str;
	enum TestEnum                  en;
	TestRequest const*             msg;
	TestPod                        pod;
	pb_slice<bool>                 rb;
	pb_slice<uint32_t>             ru32;
	pb_slice<uint64_t>             ru64;
	pb_slice<int32_t>              ri32;
	pb_slice<int64_t>              ri64;
	pb_slice<int32_t>              rs32;
	pb_slice<int64_t>              rs64;
	pb_slice<uint32_t>             rf32;
	pb_slice<uint64_t>             rf64;
	pb_slice<int32_t>              rsf32;
	pb_slice<int64_t>              rsf64;
	pb_slice<float>                rf;
	pb_slice<double>               rd;
	pb_slice<pb_bytes>             rby;
	pb_slice<pb_string>            rstr;
	pb_slice<enum TestEnum>        ren;
	pb_slice<TestRequest const*>   rmsg;
	pb_slice<TestPod>              rpod;
	pb_map<uint32_t,bool>          mb;
	pb_map<uint32_t,uint32_t>      mu32;
	pb_map<uint32_t,uint64_t>      mu64;
	pb_map<uint32_t,int32_t>       mi32;
	pb_map<uint32_t,int64_t>       mi64;
	pb_map<uint32_t,int32_t>       ms32;
	pb_map<uint32_t,int64_t>       ms64;
	pb_map<uint32_t,uint32_t>      mf32;
	pb_map<uint32_t,uint64_t>      mf64;
	pb_map<uint32_t,int32_t>       msf32;
	pb_map<uint32_t,int64_t>       msf64;
	pb_map<uint32_t,float>         mf;
	pb_map<uint32_t,double>        md;
	pb_map<uint32_t,pb_bytes>      mby;
	pb_map<uint32_t,pb_string>     mstr;
	pb_map<uint32_t,enum TestEnum> men;
	pb_pmap<uint32_t,TestRequest>  mmsg;
	pb_map<uint32_t,TestPod>       mpod;
};
struct LoginRequest {
	mutable pb_msg    pb_hdr;
	pb_string         username;
	pb_string         password;
	LoginReply const* expected;
};
struct LoginReply {
	mutable pb_msg      pb_hdr;
	pb_bytes            token;
	LoginRequest const* request;
	pb_slice<uint32_t>  numbers;
};
struct LookupDesignRequest {
	double northing;
	double easting;
};
struct LookupDesignReply {
	double elevation;
};
class TestService {
public:
	void handle(rpc_server *s, const char *path);
protected:
	virtual int Test(pb_objalloc *obj, TestRequest *w, TestRequest const *r) = 0;
	virtual int Login(pb_objalloc *obj, LoginReply *w, LoginRequest const *r) = 0;
	virtual int LookupDesign(pb_objalloc *obj, LookupDesignReply *w, LookupDesignRequest const *r) = 0;
	virtual int LookupStream(rpc_dispatch<LookupDesignReply> **out, LookupDesignRequest const *r) = 0;
private:
	static int pb_Test(void *ud, pb_objalloc *obj, pb_buffer *out, char *body);
	static int pb_Login(void *ud, pb_objalloc *obj, pb_buffer *out, char *body);
	static int pb_LookupDesign(void *ud, pb_objalloc *obj, pb_buffer *out, char *body);
	static int pb_LookupStream(void *ud, pb_objalloc *obj, rpc_dispatch_base **out, char *query);
};
class TestServiceClient : public rpc_client {
public:
	TestRequest const* Test(pb_objalloc *obj, TestRequest const *r);
	LoginReply const* Login(pb_objalloc *obj, LoginRequest const *r);
	LookupDesignReply const* LookupDesign(pb_objalloc *obj, LookupDesignRequest const &r);
	bool LookupStream(rpc_stream<LookupDesignReply>* s, LookupDesignRequest const &r);
};
}
}

uint8_t *pb_print(uint8_t *p, ::rpc::example::TestEnum v);
uint8_t *pb_parse(uint8_t *p, ::rpc::example::TestEnum *v);

uint8_t *pb_parse_msg(uint8_t *p, pb_objalloc *obj, ::rpc::example::TestRequest *m);
void pb_print_msg(pb_buffer *out, ::rpc::example::TestRequest const *m);
char *pb_parse_query(char *p, pb_objalloc *obj, ::rpc::example::TestRequest *m);
void pb_print_query(pb_buffer *out, ::rpc::example::TestRequest const *m);

uint8_t *pb_parse(uint8_t *p, ::rpc::example::TestPod *m);
void pb_print_msg(pb_buffer *out, ::rpc::example::TestPod const &v);
char *pb_parse_query(char *p, ::rpc::example::TestPod *m);
void pb_print_query(pb_buffer *out, ::rpc::example::TestPod const &v);
bool operator==(::rpc::example::TestPod const& a, ::rpc::example::TestPod const& b);
inline bool operator!=(::rpc::example::TestPod const& a, ::rpc::example::TestPod const& b) {return !(a == b);}

uint8_t *pb_parse_msg(uint8_t *p, pb_objalloc *obj, ::rpc::example::LoginRequest *m);
void pb_print_msg(pb_buffer *out, ::rpc::example::LoginRequest const *m);
char *pb_parse_query(char *p, pb_objalloc *obj, ::rpc::example::LoginRequest *m);
void pb_print_query(pb_buffer *out, ::rpc::example::LoginRequest const *m);

uint8_t *pb_parse_msg(uint8_t *p, pb_objalloc *obj, ::rpc::example::LoginReply *m);
void pb_print_msg(pb_buffer *out, ::rpc::example::LoginReply const *m);
char *pb_parse_query(char *p, pb_objalloc *obj, ::rpc::example::LoginReply *m);
void pb_print_query(pb_buffer *out, ::rpc::example::LoginReply const *m);

uint8_t *pb_parse(uint8_t *p, ::rpc::example::LookupDesignRequest *m);
void pb_print_msg(pb_buffer *out, ::rpc::example::LookupDesignRequest const &v);
char *pb_parse_query(char *p, ::rpc::example::LookupDesignRequest *m);
void pb_print_query(pb_buffer *out, ::rpc::example::LookupDesignRequest const &v);
bool operator==(::rpc::example::LookupDesignRequest const& a, ::rpc::example::LookupDesignRequest const& b);
inline bool operator!=(::rpc::example::LookupDesignRequest const& a, ::rpc::example::LookupDesignRequest const& b) {return !(a == b);}

uint8_t *pb_parse(uint8_t *p, ::rpc::example::LookupDesignReply *m);
void pb_print_msg(pb_buffer *out, ::rpc::example::LookupDesignReply const &v);
char *pb_parse_query(char *p, ::rpc::example::LookupDesignReply *m);
void pb_print_query(pb_buffer *out, ::rpc::example::LookupDesignReply const &v);
bool operator==(::rpc::example::LookupDesignReply const& a, ::rpc::example::LookupDesignReply const& b);
inline bool operator!=(::rpc::example::LookupDesignReply const& a, ::rpc::example::LookupDesignReply const& b) {return !(a == b);}
