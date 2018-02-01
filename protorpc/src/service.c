#include "common.h"
#include <string.h>

const proto_method *pb_lookup_method(void *svc, const proto_service *type, const char *path) {
	pb_string pathstr = { strlen(path), path };
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
// It returns the http status code
// The output data is written to the buffer pointed to by out
// and out->len is updated to reflect the length of the output.
// The input data is provided in `in`. Note that one more byte than
// provided will be written to (ie in.p[in.len] = 0) to null terminate
// the input.
// This function services both JSON and protobuf request data by looking
// at the data.
int pb_dispatch(void *svc, const proto_method *method, pb_allocator *obj, char *in, int insz, char *out, int *outsz) {
	proto_method_fn *fns = (proto_method_fn*)svc;
	proto_method_fn fn = fns[method->offset];
	
	bool is_text = (insz && in[0] == '{');
	char *objstart = obj->next;

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
		return 400; // bad request
	}

	int ret = fn(svc, obj, inm, outm);
	if (!ret) {
		ret = 200; // OK
	} else if (ret < 0 || ret > 999) {
		ret = 500; // internal server error
	}

	// encode the output
	int outlen;
	if (is_text) {
		outlen = pb_print(outm, method->output, out, *outsz);
	} else {
		outlen = pb_encoded_size(outm, method->output);
		if (0 < outlen && outlen < *outsz) {
			outlen = pb_encode(outm, method->output, out);
		} else {
			outlen = -1;
		}
	}

	obj->next = objstart;

	if (outlen < 0) {
		// encoding the output failed
		return 500; // internal server error
	}

	*outsz = outlen;
	return ret;
}
