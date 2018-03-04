#include "common.h"
#include <string.h>
#include <stdio.h>

const char *pb_lookup_file(const proto_dir *d, const char *path, int len, int *respsz) {
	pb_string pathstr = { len, path };
	const pb_string *fname = binary_search(d->by_path, d->num_files, pathstr);
	if (!fname) {
		*respsz = 0;
		return NULL;
	}
	const proto_file *f = (const proto_file*)fname;
	*respsz = f->len;
	return f->response;
}

const proto_method *pb_lookup_method(void *svc, const proto_service *type, const char *path, int len) {
	pb_string pathstr = { len, path };
	const pb_string *mname = binary_search(type->by_path, type->num_methods, pathstr);
	if (!mname) {
		return NULL;
	}
	const proto_method *method = (const proto_method*)mname;
	proto_method_fn *fns = (proto_method_fn*)svc;
	proto_method_fn fn = fns[method->offset];
	if (!fn) {
		return NULL;
	}
	
	return method;
}


// pb_dispatch dispatches a RPC service call
// An HTTP header + output data is written to the `out` buffer.
// The function returns the number of bytes used.
// The input data is provided in `in`. Note that one more byte than
// provided will be written to (ie in[insz] = 0) to null terminate
// the input.
// This function services both JSON and protobuf request data by looking
// at the data.
int pb_dispatch(void *svc, const proto_method *method, pb_allocator *obj, char *in, int insz, char *out, int outsz) {
	proto_method_fn *fns = (proto_method_fn*)svc;
	proto_method_fn fn = fns[method->offset];
	
	bool is_text = (insz && in[0] == '{');
	char *objstart = obj->next;

	if (outsz < 256) {
		return 0;
	}

	// decode the input
	void *inm;
	if (is_text) {
		in[insz] = 0;
		inm = pb_parse(obj, method->input, in);
	} else {
		inm = pb_decode(obj, method->input, in, insz);
	}

	void *outm = pb_calloc(obj, 1, method->output->datasz);

	if (!inm || !outm) {
		// decoding the input failed
		obj->next = objstart;
		return sprintf(out, "HTTP/1.1 400 Malformed Payload\r\nContent-Length:0\r\n\r\n");
	}

	int sts = fn(svc, obj, inm, outm);
	if (!sts) {
		sts = 200; // OK
	} else if (sts < 100 || sts > 999) {
		sts = 500; // internal server error
	}

	int ret = sprintf(out, "HTTP/1.1 %d \r\nContent-Type:%s\r\nContent-Length:      \r\n\r\n",
		sts, is_text ? "application/json;charset=utf-8" : "application/protobuf");

	// encode the output
	int sz;
	if (is_text) {
		sz = pb_print(out + ret, outsz - ret, outm, method->output, 0);
	} else {
		sz = pb_encoded_size(outm, method->output);
		if (0 <= sz && sz < (outsz - ret)) {
			sz = pb_encode(outm, method->output, out + ret);
		} else {
			sz = -1;
		}
	}

	obj->next = objstart;

	if (sz < 0 || sz > 999999) {
		// encoding the output failed
		return sprintf(out, "HTTP/1.1 500 Output Failure\r\nContent-Length:0\r\n\r\n");
	}

	// fill out Content-Length
	// start at last digit and work backwards
	char *slen = out + ret - strlen(" \r\n\r\n");
	ret += sz;
	do {
		*slen-- = (sz % 10) + '0';
		sz /= 10;
	} while (sz);

	return ret;
}
