#include "common.h"
#include "protorpc/http.h"
#include "protorpc/char-array.h"
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

static const proto_method *lookup_method(void *svc, const char *path, int len, const pb_string **by_path, size_t num) {
	pb_string pathstr = { len, path };
	const pb_string *mname = binary_search(by_path, num, pathstr);
	if (!mname) {
		return NULL;
	}
	const proto_method *method = (const proto_method*)mname;
	proto_bidi_fn *fns = (proto_bidi_fn*)svc;
	proto_bidi_fn fn = fns[method->offset];
	if (!fn) {
		return NULL;
	}

	return method;
}

const proto_method *pb_lookup_method(void *svc, const proto_service *type, const char *path, int len) {
	return lookup_method(svc, path, len, type->methods_by_path, type->num_methods);
}

const proto_method *pb_lookup_stream(void *svc, const proto_service *type, const char *path, int len) {
	return lookup_method(svc, path, len, type->streams_by_path, type->num_streams);
}


// pb_dispatch dispatches a RPC service call
// An HTTP header + output data is written to the `out` buffer.
// The function returns the number of bytes used.
// The input data is provided in `in`. Note that one more byte than
// provided will be written to (ie in[insz] = 0) to null terminate
// the input.
// This function services both JSON and protobuf request data by looking
// at the data.
int pb_dispatch(void *svc, const proto_method *method, http *h, char *out, int outsz) {
	proto_bidi_fn bifn = ((proto_bidi_fn*)svc)[method->offset];
	proto_in_fn infn = ((proto_in_fn*)svc)[method->offset];

	int insz;
	char *in = http_request_data(h, &insz);
	bool is_text = (insz && in[0] == '{');
	pb_allocator *obj = &h->obj;
	char *objstart = obj->next;
	bool is_bidi = method->output != NULL;

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

	http_consume_data(h, insz);

	void *outm = is_bidi ? pb_calloc(obj, 1, method->output->datasz) : inm;

	if (!inm || !outm) {
		// decoding the input failed
		obj->next = objstart;
		return sprintf(out, "HTTP/1.1 400 Malformed Payload\r\nContent-Length:0\r\n\r\n");
	}

	int sts = is_bidi ? bifn(svc, h, inm, outm) : infn(svc, h, inm);
	if (h->state == HTTP_RECEIVING_WEBSOCKET) {
		return 0;
	}
	if (!sts) {
		sts = 200; // OK
	} else if (sts < 100 || sts > 999) {
		sts = 500; // internal server error
	}

	int ret = sprintf(out, "HTTP/1.1 %d \r\nContent-Type:%s\r\nContent-Length:      \r\n\r\n",
		sts, is_text ? "application/json;charset=utf-8" : "application/protobuf");

	// encode the output
	int sz = 0;
	if (is_bidi) {
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
