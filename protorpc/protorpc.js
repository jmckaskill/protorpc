

// flags
// bits 0-3 - base type
// bit 4 - repeated
// bit 5 - has extra decoder
// 0/16 - bool
// 1/17 - uint32
// 2/18 - int32
// 3/19 - sint32
// 36/52 - enum
// 5/21 - uint64
// 6/22 - int64
// 7/23 - sint64
// 8/24 - fixed32
// 9/25 - sfixed32
// 10/26 - fixed64
// 11/27 - sfixed64
// 12/28 - float
// 13/29 - double
// 14/30 - string
// 15/31/47/63 - bytes & msg

var proto = (function () {
	// message types are stored in a compact form
	// msg = {field: [tag,type,extra]}
	// this converts it into an array useable by the encode & decode functions
	// the array is cached in the message type for reuse
	var load_message = function (fields) {
		var dec = fields._decoder;
		if (dec) {
			return dec;
		}
		dec = [];
		for (var name in fields) {
			var field = fields[name];
			if (field.length == 2) {
				field.push(null);
			}
			field.push(name);
			dec.push(field);
		}
		// a[0] is the tag
		dec.sort((a, b) => a[0] - b[0]);
		fields._decoder = dec;
		return dec;
	};
	// in these functions
	// v is a Uint8Array pointing to the original input buffer
	// r is an array of [current offset, current end]
	var get_varint = function (v, r) {
		var shift = 0;
		var ret = 0;
		while (r[0] < r[1]) {
			var u = v.getUint8(r[0]);
			ret |= (u & 0x7F) << shift;
			r[0]++;
			if ((u & 0x80) == 0) {
				return ret >>> 0;
			}
			shift += 7;
		}
		throw new Error("invalid protobuf");
	};
	var get_varint_64 = function (v, r) {
		var shift = 0;
		var low = 0;
		var high = 0;
		while (r[0] < r[1]) {
			var u = v.getUint8(r[0]);
			var w = u & 0x7F;
			if (shift > 32) {
				high |= w << (shift - 32);
			} else if (shift) {
				low |= w << shift;
				high |= w >>> (32 - shift);
			} else {
				low = w;
			}
			r[0]++;
			if ((u & 0x80) == 0) {
				return [low >>> 0, high >>> 0];
			}
			shift += 7;
		}
		throw new Error("invalid protobuf");
	};
	var skip = function (v, r, tag) {
		switch (tag & 7) {
			case 0: // varint
				while (r[0] < r[1] && (v.getUint8(r[0]) & 0x80)) {
					r[0]++;
				}
				r[0]++;
				break;
			case 1: // fixed 64
				r[0] += 8;
				break;
			case 2: // variable
				r[0] += get_varint(v, r);
				break;
			case 5: // fixed 32
				r[0] += 4;
				break;
		}
		if (r[0] > r[1]) {
			throw new Error("invalid protobuf");
		}
	};
	var get_varlen = function (v, r) {
		var len = get_varint(v, r);
		var ret = [r[0], r[0] + len];
		r[0] += len;
		return ret;
	};
	var utf8len = function (str) {
		var ret = 0;
		var len = str.length;
		for (var i = 0; i < len; i++) {
			var ch = str.charCodeAt(i);
			if (ch < 0x80) {
				ret += 1;
			} else if (ch < 0x800) {
				ret += 2;
			} else if ((ch & 0xFF00) == 0xD800) {
				ret += 4;
				i++;
			} else {
				ret += 3;
			}
		}
		return ret;
	};
	var encode_utf8 = function (v, idx, str) {
		var len = str.length;
		for (var i = 0; i < len; i++) {
			var ch = str.charCodeAt(i);
			if (ch < 0x80) {
				// 0xxxxxxx
				v.setUint8(idx++, ch);
			} else if (ch < 0x800) {
				// 110x xxxx   10xx xxxx
				v.setUint8(idx++, 0xC0 | (ch >>> 6));
				v.setUint8(idx++, 0x80 | (ch & 0x3F));
			} else if ((ch & 0xFF00) == 0xD800) {
				// high surrogate 1101 10xx xxxx xxxx 
				// low surrogate 1101 11xx xxxx xxxx
				var high = ch & 0x3FF;
				var low = str.charCodeAt(++i) & 0x3FF;
				// decode to unicode code point first
				// UTF16 is shifted by 0x10000
				var u = ((high << 10) | low) + 0x10000;
				// then encode to utf-8
				// 1111 0xxx 10xx xxxx 10xx xxxx 10xx xxxx
				v.setUint8(idx++, 0xF0 | (u >>> 18));
				v.setUint8(idx++, 0x80 | ((u >>> 12) & 0x3F));
				v.setUint8(idx++, 0x80 | ((u >>> 6) & 0x3F));
				v.setUint8(idx++, 0x80 | (u & 0x3F));
			} else {
				// 1110 xxxx  10xx xxxx  10xx xxxx
				v.setUint8(idx++, 0xE0 | (ch >>> 12));
				v.setUint8(idx++, 0x80 | ((ch >>> 6) & 0x3F));
				v.setUint8(idx++, 0x80 | (ch & 0x3F));
			}
		}
		return idx;
	}
	var utf8to16 = function (v, i, end) {
		var out = "";
		while (i < end) {
			var c = v.getUint8(i++);
			if (c < 0x80) {
				// 0xxxxxxx
				out += String.fromCharCode(c);
			} else if (c < 0xE0) {
				// 110x xxxx   10xx xxxx
				var c2 = v.getUint8(i++);
				out += String.fromCharCode(((c & 0x1F) << 6) | (c2 & 0x3F));
			} else if (c < 0xF0) {
				// 1110 xxxx  10xx xxxx  10xx xxxx
				var c2 = v.getUint8(i++);
				var c3 = v.getUint8(i++);
				out += String.fromCharCode(((c & 0x0F) << 12) |
					((c2 & 0x3F) << 6) |
					((c3 & 0x3F) << 0));
			} else {
				// 1111 0xxx 10xx xxxx 10xx xxxx 10xx xxxx
				// decode to unicode code point first
				var c2 = v.getUint8(i++);
				var c3 = v.getUint8(i++);
				var c4 = v.getUint8(i++);
				var u = ((c & 7) << 18) | ((c2 & 0x3F) << 12) | ((c3 & 0x3F) << 6) | (c4 & 0x3F);
				// then encode to UTF-16
				// UTF16 is shifted by 0x10000
				u -= 0x10000;
				// high surrogate 1101 10xx xxxx xxxx 
				var high = 0xD800 | (u >> 10);
				// low surrogate 1101 11xx xxxx xxxx
				var low = 0xDC00 | (u & 0x3FF);
				out += String.fromCharCode(high, low);
			}
		}
		return out;
	}
	var decode_u64 = function (low, high) {
		var rem = [];
		while (high) {
			var rh = high % 10;
			var qh = Math.floor(high / 10);
			var xl = low + (rh * 4294967296);
			var rl = xl % 10;
			var ql = Math.floor(xl / 10);
			rem.push(rl + 48 /*'0'*/);
			low = ql;
			high = qh;
		}
		while (low) {
			var r = low % 10;
			var q = Math.floor(low / 10);
			rem.push(r + 48 /*'0'*/);
			low = q;
		}
		rem.reverse();
		if (!rem.length) {
			rem.push(48);
		}
		return String.fromCharCode.apply(null, rem);
	}
	var decode_i64 = function (low, high) {
		if (high >= 0x80000000) {
			return "-" + decode_u64((0xFFFFFFFF - low) + 1, 0xFFFFFFFF - high);
		} else {
			return decode_u64(low, high);
		}
	}
	var encode_u64 = function (str, off) {
		var low = 0.0;
		var high = 0;
		for (var i = off; i < str.length; i++) {
			low *= 10;
			high *= 10;
			var q = Math.floor(low / 4294967296);
			low -= q * 4294967296;
			high += q;
			var ch = str.charCodeAt(i);
			if (ch < 48 /*0*/ || ch > 57 /*9*/) {
				throw new Error("invalid 64 bit integer string");
			}
			low += ch - 48 /*'0'*/;
		}
		return [low, high];
	};
	var encode_i64 = function (str) {
		if (str.charAt(0) == "-") {
			var lh = encode_u64(str, 1);
			lh[0] = 0xFFFFFFFF - (lh[0] - 1);
			lh[1] = 0xFFFFFFFF - lh[1];
			return lh;
		} else {
			return encode_u64(str, 0);
		}
	};
	var decode_message;
	var decode_field = function (v, r, type, extra) {
		switch (type) {
			case 0: // bool
				return get_varint(v, r) != 0;
			case 1: // uint32
				return get_varint(v, r);
			case 2: // int32
				return get_varint(v, r) >> 0;
			case 3: // sint32
				// 0 -> 0, 1 -> -1, 2 -> 1, 3 -> -2
				var u = get_varint(v, r);
				return (u >>> 1) ^ -((u & 1) >> 0);
			case 4: // enum
				var u = get_varint(v, r);
				return extra[u];
			case 5: // uint64
				var lh = get_varint_64(v, r);
				return decode_u64(lh[0], lh[1]);
			case 6: // int64
				var lh = get_varint_64(v, r);
				return decode_i64(lh[0], lh[1]);
			case 7: // sint64
				var lh = get_varint_64(v, r);
				// equivalent to (u >> 1) ^ -(int64_t)(u & 1)
				var low = lh[0];
				var high = lh[1];
				var mask = -(low & 1);
				low = (((low >>> 1) | (high << 31)) ^ mask) >>> 0;
				high = ((high >>> 1) ^ mask) >>> 0;
				return decode_i64(low, high);
			case 8: // fixed32
				var ret = v.getUint32(r[0], true);
				r[0] += 4;
				return ret;
			case 9: // sfixed32
				var ret = v.getInt32(r[0], true);
				r[0] += 4;
				return ret;
			case 10: // fixed64
				var low = v.getUint32(r[0], true);
				var high = v.getUint32(r[0] + 4, true);
				r[0] += 8;
				return decode_u64(low, high);
			case 11: // sfixed64
				var low = v.getUint32(r[0], true);
				var high = v.getUint32(r[0] + 4, true);
				r[0] += 8;
				return decode_i64(low, high);
			case 12: // float
				var ret = v.getFloat32(r[0], true);;
				r[0] += 4;
				return ret;
			case 13: // double
				var ret = v.getFloat64(r[0], true);
				r[0] += 8;
				return ret;
			case 14: // string
				var rs = get_varlen(v, r);
				return utf8to16(v, rs[0], rs[1]);
			case 15: // bytes
				var rs = get_varlen(v, r);
				if (extra) {
					return decode_message(extra, v, rs);
				} else {
					return v.buffer.slice(rs[0], rs[1]);
				}
		}
	}
	var decode_packed = function (v, r, type, extra) {
		var rs = get_varlen(v, r);
		var ret = [];
		while (rs[0] < rs[1]) {
			ret.push(decode_field(v, rs, type, extra));
		}
		return ret;
	};
	var decode_repeated = function (v, r, tag, type, extra) {
		var ret = [];
		for (; ;) {
			ret.push(decode_field(v, r, type, extra));
			var undo = r[0];
			if (r[0] >= r[1] || get_varint(v, r) != tag) {
				r[0] = undo;
				return ret;
			}
		}
	};
	decode_message = function (fields, v, r) {
		var ret = {};
		var msgtype = load_message(fields);
		var fidx = 0;
		var off = 0;
		while (r[0] < r[1] && fidx < msgtype.length) {
			var field = msgtype[fidx];
			var want = field[0];
			var type = field[1];
			var undo = r[0];
			var have = get_varint(v, r);
			if (want < have) {
				r[0] = undo;
				fidx++;
				continue;
			} else if (have < want) {
				skip(v, r, have);
				continue;
			}

			var extra = field[2];
			var name = field[3];
			var base = type & 15;

			if ((type & 16) == 0) {
				ret[name] = decode_field(v, r, base, extra);
			} else if (base <= 13) {
				ret[name] = decode_packed(v, r, base, extra);
			} else {
				ret[name] = decode_repeated(v, r, have, base, extra);
			}
		}
		return ret;
	};
	var enum_decoder = function (enc) {
		var dec = {};
		for (var i in enc) {
			var name = enc[i];
			dec[name] = parseInt(i, 10);
		}
		enc._decoder = dec;
		return dec;
	};
	var encode_varint = function (v, idx, val) {
		while (val >= 0x80) {
			v.setUint8(idx++, 0x80 | (val & 0x7F));
			val >>>= 7;
		}
		v.setUint8(idx++, val);
		return idx;
	};
	var encode_varint_64 = function (v, idx, low, high) {
		while (high || low >= 0x80) {
			v.setUint8(idx++, 0x80 | (low & 0x7F));
			low = (low >>> 7) | (high << 25);
			high >>>= 7;
		}
		v.setUint8(idx++, low);
		return idx;
	};
	var varint_length = function (val) {
		if (val < 0x80) {
			return 1;
		} else if (val < 0x4000) {
			return 2;
		} else if (val < 0x200000) {
			return 3;
		} else if (val < 0x10000000) {
			return 4;
		} else {
			return 5;
		}
	};
	var varint_length_64 = function (low, high) {
		if (!high) {
			return varint_length(low);
		} else {
			// we have at least 32 bits
			high >>>= 3;
			var ret = 5;
			while (high) {
				ret += 1;
				high >>>= 7;
			}
			return ret;
		}
	};
	var message_length;
	var encode_message;
	var field_length = function (val, type, extra, save) {
		switch (type) {
			case 0: // bool
				return 1;
			case 1: // uint32
				return varint_length(val);
			case 2: // int32
				return varint_length(val >>> 0);
			case 3: // sint32
				var u = (val << 1) ^ (val >> 31);
				return varint_length(u);
			case 4: // enum
				var dec = extra._decoder || enum_decoder(extra);
				var u = dec[val];
				return varint_length(u);
			case 5: // uint64
				var lh = encode_u64(val, 0);
				var low = lh[0];
				var high = lh[1];
				save.push(low);
				save.push(high);
				return varint_length_64(low, high);
			case 6: // int64
				var lh = encode_i64(val);
				var low = lh[0];
				var high = lh[1];
				save.push(low);
				save.push(high);
				return varint_length_64(low, high);
			case 7: // sint64
				var lh = encode_i64(val);
				// equivalent to ((uint64_t)i << 1) ^ (uint64_t)(i >> 63)
				var low = lh[0];
				var high = lh[1];
				var mask = high >> 31;
				high = ((high << 1) | (low >>> 31)) ^ mask;
				low = (low << 1) ^ mask
				save.push(low);
				save.push(high);
				return varint_length_64(low, high);
			case 8: // fixed32
			case 9: // sfixed32
			case 12: // float
				return 4;
			case 10: // fixed64
			case 11: // sfixed64
			case 13: // double
				return 8;
			case 14: // string
				var u8len = utf8len(val);
				save.push(u8len);
				return varint_length(u8len) + u8len;
			case 15: // bytes/messages
				var len;
				if (extra) {
					var savei = save.length;
					save.push(0);
					len = message_length(extra, val, save);
					save[savei] = len;
				} else {
					len = val.byteLength;
				}
				return varint_length(len) + len;
		}
	};
	var encode_field = function (v, idx, val, type, extra, save) {
		switch (type) {
			case 0: // bool
				v.setUint8(idx, val ? 1 : 0);
				return idx + 1;
			case 1: // uint32
				return encode_varint(v, idx, val);
			case 2: // int32
				return encode_varint(v, idx, val >>> 0);
			case 3: // sint32
				var u = (val << 1) ^ (val >> 31);
				return encode_varint(v, idx, u);
			case 4: // enum
				var dec = extra._decoder;
				var u = dec[val];
				return encode_varint(v, idx, u);
			case 5: // uint64
			case 6: // int64
			case 7: // sint64
				var low = save[save[0]++];
				var high = save[save[0]++];
				return encode_varint_64(v, idx, low, high);
			case 8: // fixed32
				v.setUint32(idx, val, true);
				return idx + 4;
			case 9: // sfixed32
				v.setInt32(idx, val, true);
				return idx + 4;
			case 12: // float
				v.setFloat32(idx, val, true);
				return idx + 4;
			case 10: // fixed64
				var lh = encode_u64(val, 0);
				v.setUint32(idx, lh[0], true);
				v.setUint32(idx + 4, lh[1], true);
				return idx + 8;
			case 11: // sfixed64
				var lh = encode_i64(val);
				v.setUint32(idx, lh[0], true);
				v.setUint32(idx + 4, lh[1], true);
				return idx + 8;
			case 13: // double
				v.setFloat64(idx, val, true);
				return idx + 8;
			case 14: // string
				var u8len = save[save[0]++];
				idx = encode_varint(v, idx, u8len);
				return encode_utf8(v, idx, val);
			case 15: // bytes/messages
				if (extra) {
					var len = save[save[0]++];
					idx = encode_varint(v, idx, len);
					return encode_message(v, idx, val, extra, save);
				} else {
					var len = val.byteLength;
					var src = new DataView(val);
					idx = encode_varint(v, idx, len);
					for (var i = 0; i < len; i++) {
						v.setUint8(idx + i, src.getUint8(i));
					}
					return idx + len;
				}
		}
	};
	var packed_length = function (val, type, extra, save) {
		var elen = 0;
		var slen = val.length;
		var savei = save.length;
		save.push(0);
		for (var i = 0; i < slen; i++) {
			elen += field_length(val[i], type, extra, save);
		}
		save[savei] = elen;
		return varint_length(elen) + elen;
	};
	var encode_packed = function (v, idx, val, type, extra, save) {
		var elen = save[save[0]++];
		var slen = val.length;
		idx = encode_varint(v, idx, elen);
		for (var i = 0; i < slen; i++) {
			idx = encode_field(v, idx, val[i], type, extra, save);
		}
		return idx;
	};
	var repeated_length = function (val, taglen, type, extra, save) {
		var ret = 0;
		for (var i = 0; i < val.length; i++) {
			ret += taglen + field_length(val[i], type, extra, save);
		}
		return ret;
	};
	var encode_repeated = function (v, idx, val, tag, type, extra, save) {
		var slen = val.length;
		for (var i = 0; i < slen; i++) {
			idx = encode_varint(v, idx, tag);
			idx = encode_field(v, idx, val[i], type, extra, save);
		}
		return idx;
	};
	message_length = function (fields, m, save) {
		var msgtype = load_message(fields);
		var ret = 0;
		for (var i in msgtype) {
			var field = msgtype[i];
			var tag = field[0];
			var type = field[1];
			var extra = field[2];
			var name = field[3];
			var val = m[name];

			if (!val) {
				continue;
			}

			var base = type & 15;
			var taglen = varint_length(tag);
			if ((type & 16) == 0) {
				ret += taglen + field_length(val, base, extra, save);
			} else if (val.length) {
				if (base <= 13) {
					ret += taglen + packed_length(val, base, extra, save);
				} else {
					ret += repeated_length(val, taglen, base, extra, save);
				}
			}
		}
		return ret;
	};
	encode_message = function (v, idx, m, fields, save) {
		var msgtype = fields._decoder;
		for (var i in msgtype) {
			var field = msgtype[i];
			var tag = field[0];
			var type = field[1];
			var extra = field[2];
			var name = field[3];
			var val = m[name];

			if (!val) {
				continue;
			}

			var base = type & 15;
			if ((type & 16) == 0) {
				idx = encode_varint(v, idx, tag);
				idx = encode_field(v, idx, val, base, extra, save);
			} else if (val.length) {
				if (base <= 13) {
					idx = encode_varint(v, idx, tag);
					idx = encode_packed(v, idx, val, base, extra, save);
				} else {
					idx = encode_repeated(v, idx, val, tag, base, extra, save);
				}
			}
		}
		return idx;
	};
	var encode = function (fields, m) {
		// returns arraybuffer
		var save = [1]; // save[0] is the index used by encode_*
		var len = message_length(fields, m, save);
		var ab = new ArrayBuffer(len);
		encode_message(new DataView(ab), 0, m, fields, save);
		return ab;
	};
	var call = function (path, itype, otype, timeout, msg) {
		// returns Promise
		return new Promise(function (resolve, reject) {
			var encmsg = encode(itype, msg);
			var req = new XMLHttpRequest();
			req.open("POST", path);
			req.setRequestHeader("Content-Type", "application/protobuf");
			req.responseType = "arraybuffer";
			req.timeout = timeout;
			req.onload = function (ev) {
				if (200 <= req.status && req.status < 300) {
					var ab = /** @type {!ArrayBuffer}*/ (req.response);
					var resp = decode_message(otype, new DataView(ab), [0, ab.byteLength]);
					resolve(resp);
				} else {
					reject(req.status);
				}
			};
			req.ontimeout = () => reject("timeout");
			req.onerror = (ev) => reject(ev);
			req.send(encmsg);
		});
	};
	var new_client = function (svc, timeout) {
		if (!svc._prototype) {
			var bp = svc._basePath;
			delete svc._basePath;

			var proto = {};
			for (var name in svc) {
				(function (name, method) {
					var path = method[0];
					var itype = method[1];
					var otype = method[2];
					proto[name] = function (request) {
						return call(bp + path, itype, otype, this._timeout || 10000, request);
					}
				})(name, svc[name]);
			}
			svc._prototype = proto;
		}

		var c = Object.create(svc._prototype);
		if (timeout) {
			c._timeout = timeout;
		}
		return c;
	};

	return {
		decode: function (fields, buf) {
			// returns decoded object
			if (buf.buffer) {
				// buf is a typed view or data view
				return decode_message(fields, new DataView(buf.buffer), [buf.byteOffset, buf.byteOffset + buf.byteLength]);
			} else {
				// buf is an array buffer
				return decode_message(fields, new DataView(buf), [0, buf.byteLength]);
			}
		},
		new_client: new_client,
		encode: encode,
		utf8to16: utf8to16,
	};
})();

