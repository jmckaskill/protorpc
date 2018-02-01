#include "common.h"
#include <string.h>

// pb_dispatch dispatches a RPC service call
// It returns the http status code
// The output data is written to the buffer pointed to by out
// and out->len is updated to reflect the length of the output.
// The input data is provided in `in`. Note that one more byte than
// provided will be written to (ie in.p[in.len] = 0) to null terminate
// the input.
// `path` specifies the path of the HTTP request
// This function services both JSON and protobuf request data by looking
// at the data.
int pb_dispatch(void *svc, const proto_service *type, pb_allocator *obj, const char *path, pb_bytes in, pb_bytes *out) {
	// lookup the url
	pb_string pathstr = { strlen(path), path };
	const pb_string *mname = binary_search(type->by_path, type->num_methods, pathstr);
	if (!mname) {
		return 404; // not found
	}
	const proto_method *method = (const proto_method*)mname;
	proto_method_fn *fns = (proto_method_fn*)svc;
	proto_method_fn fn = fns[method->offset];
	if (!fn) {
		return 501; // not implemented
	}

	bool is_text = (in.len && in.p[0] == '{');
	char *inp = (char*)in.p;
	char *objstart = obj->next;

	// decode the input
	void *inm;
	if (is_text) {
		inp[in.len] = 0;
		inm = pb_parse(obj, method->input, inp);
	} else {
		inm = pb_decode(obj, method->input, inp, in.len);
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
	char *outp = (char*)out->p;
	int outlen;
	if (is_text) {
		outlen = pb_print(outm, method->output, outp, out->len);
	} else {
		outlen = pb_encoded_size(outm, method->output);
		if (0 < outlen && outlen < out->len) {
			outlen = pb_encode(outm, method->output, outp);
		}
	}

	obj->next = objstart;

	if (outlen < 0) {
		// encoding the output failed
		return 500; // internal server error
	}

	out->len = outlen;
	return ret;
}
