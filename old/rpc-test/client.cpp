#include "protocol.hpp"
#include <protorpc/protorpc.hpp>

using namespace rpc::example;

int main(int argc, char *argv[]) {
    pb_objalloc obj;
    TestServiceClient client;
    client.setup("localhost", "8080", "/api", NULL);

    LoginRequest req = {};
    req.username.setref(argv[1], strlen(argv[1]));
    req.password.setref("bar");
    auto resp = client.Login(&obj, &req);
    if (!resp) {
        fprintf(stderr, "error %d %s\n", client.err(), client.strerr());
        return -1;
    }
    //std::cerr << resp << std::endl;
    fprintf(stderr, "response %.*s\n", (int)resp->token.size(), &resp->token[0]);
    return 0;
}