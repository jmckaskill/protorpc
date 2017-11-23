#include "bin-to-json.proto.h"
#include <os/str.h>
#include <os/pipe.h>

int main() {
    fprintf(stderr, "starting\n");
    pb_alloc_t obj = PB_ALLOC_INIT;
	str_t json = STR_INIT;
	str_t pretty = STR_INIT;

    fprintf(stderr, "read all\n");
    str_t in = STR_INIT;
    str_fread_all(&in, stdin);
    fprintf(stderr, "all read %d\n", in.len);

    struct TestMessage msg = {0};
    pb_get_TestMessage((uint8_t*)in.buf, (uint8_t*)in.buf+in.len, &obj, &msg);
    fprintf(stderr, "decode %d\n", msg.sub->u32);

    pb_print_TestMessage(&json, &msg);
    fprintf(stderr, "printed %d\n", json.len);
	str_setlen(&json, json.len-1); // remove the trailing comma

    pb_pretty_print(&pretty, json.buf, json.len);
    fprintf(stderr, "prettied %d\n", pretty.len);

	set_binary(stdout);
	fwrite(pretty.buf, 1, pretty.len, stdout);
    
	fprintf(stderr, "written\n");
    return 0;
}