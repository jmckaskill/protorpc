#define BUILDING_PROTORPC
#include "../protobuf.h"

const char *pb_get_u32(const char *p, const char *e, uint32_t *pv) {
    size_t shift = 0;
    uint32_t v = 0;
    while (p < e) {
        uint32_t u = (uint32_t) *(uint8_t*) p;
		p++;
        v |= (u & 0x7F) << shift;
        if (!(u & 0x80)) {
            *pv = v;
            return p;
        }
        shift += 7;
    }
    *pv = 0;
    return e;
}

const char *pb_get_u64(const char *p, const char *e, uint64_t *pv) {
    size_t shift = 0;
    uint64_t v = 0;
	while (p < e) {
		uint64_t u = (uint64_t) *(uint8_t*) p;
		p++;
        v |= (u & 0x7F) << shift;
        if (!(u & 0x80)) {
            *pv = v;
            return p;
        }
        shift += 7;
    }
    *pv = 0;
    return e;
}

const char *pb_get_s32(const char *p, const char *e, int32_t *pv) {
    uint32_t u;
    p = pb_get_u32(p,e,&u);
    *pv = (u >> 1) ^ -(int32_t)(u&1);
    return p;
}

const char *pb_get_s64(const char *p, const char *e, int64_t *pv) {
    uint64_t u;
    p = pb_get_u64(p,e,&u);
    *pv = (u >> 1) ^ -(int64_t)(u&1);
    return p;
}

const char *pb_get_f32(const char *p, const char *e, union pb_f32 *pv) {
    if (p + 4 <= e) {
        pv->u = *(uint32_t*) p;
        return p+4;
    }
    return e;
}

const char *pb_get_f64(const char *p, const char *e, union pb_f64 *pv) {
    if (p + 8 <= e) {
        pv->u = *(uint64_t*) p;
        return p+8;
    }
    return e;
}

const char *pb_get_string(const char *p, const char *e, pb_string_t *pv) {
    uint32_t sz;
    p = pb_get_u32(p,e,&sz);
    if (p + sz <= e) {
        pv->buf = p;
        pv->len = sz;
        return p + sz;
    }
    pv->buf = NULL;
    pv->len = 0;
    return e;
}

const char *pb_get_packed_bool(const char *p, const char *e, pb_buf_t *a, bool **pv, int *plen) {
	(void) a;
    static_assert(sizeof(bool) == 1, "incompatible bool size");
    pb_string_t bytes;
    p = pb_get_string(p,e,&bytes);
    *pv = (bool*) bytes.buf;
    *plen = bytes.len;
    return p;
}
const char *pb_get_packed_u32(const char *p, const char *e, pb_buf_t *a, uint32_t **pv, int *plen) {
	(void) a;
    pb_string_t b;
    const char *ret = pb_get_string(p,e,&b);
    p = b.buf;
    e = b.buf + b.len;
    *plen = 0;
	*pv = (uint32_t*)a->next;
    while (p < e) {
		if (a->next + ((*plen)*sizeof(uint32_t)) > a->end) {
			return NULL;
		}
        p = pb_get_u32(p, e, &(*pv)[(*plen)++]);
    }
	a->next += (*plen)*sizeof(uint32_t);
    return ret;
}
const char *pb_get_packed_u64(const char *p, const char *e, pb_buf_t *a, uint64_t **pv, int *plen) {
    pb_string_t b;
    const char *ret = pb_get_string(p,e,&b);
	p = b.buf;
	e = b.buf + b.len;
    *plen = 0;
	*pv = (uint64_t*)a->next;
    while (p < e) {
		if (a->next + ((*plen) * sizeof(uint64_t)) > a->end) {
			return NULL;
		}
        p = pb_get_u64(p, e, &(*pv)[(*plen)++]);
    }
	a->next += (*plen) * sizeof(uint64_t);
    return ret;
}
const char *pb_get_packed_s32(const char *p, const char *e, pb_buf_t *a, int32_t **pv, int *plen) {
    pb_string_t b;
    const char *ret = pb_get_string(p,e,&b);
	p = b.buf;
	e = b.buf + b.len;
    *plen = 0;
	*pv = (int32_t*)a->next;
	while (p < e) {
		if (a->next + ((*plen) * sizeof(int32_t)) > a->end) {
			return NULL;
		}
        p = pb_get_s32(p, e, &(*pv)[(*plen)++]);
    }
	a->next += (*plen) * sizeof(int32_t);
    return ret;
}
const char *pb_get_packed_s64(const char *p, const char *e, pb_buf_t *a, int64_t **pv, int *plen) {
    pb_string_t b;
    const char *ret = pb_get_string(p,e,&b);
	p = b.buf;
	e = b.buf + b.len;
    *plen = 0;
	*pv = (int64_t*)a->next;
	while (p < e) {
		if (a->next + ((*plen) * sizeof(int64_t)) > a->end) {
			return NULL;
		}
        p = pb_get_s64(p, e, &(*pv)[(*plen)++]);
    }
	a->next += (*plen) * sizeof(int64_t);
    return ret;
}
const char *pb_get_packed_f32(const char *p, const char *e, pb_buf_t *a, union pb_f32 **pv, int *plen) {
	(void) a;
    pb_string_t b;
    p = pb_get_string(p,e,&b);
    *pv = (union pb_f32*) b.buf;
    *plen = b.len / 4;
    return p;
}
const char *pb_get_packed_f64(const char *p, const char *e, pb_buf_t *a, union pb_f64 **pv, int *plen) {
	(void) a;
    pb_string_t b;
    p = pb_get_string(p,e,&b);
    *pv = (union pb_f64*) b.buf;
    *plen = b.len / 8;
    return p;
}


static const char *skip(const char *p, const char *e, uint8_t typ) {
    uint32_t sz;
    switch (typ & 7) {
    case PB_WIRE_VARINT:
        while (*p & 0x80) {
            if (++p == e) {
                goto err;
            }
        }
        break;
    case PB_WIRE_F64:
        p += 8;
        break;
    case PB_WIRE_VARIABLE:
        p = pb_get_u32(p,e,&sz);
        p += sz;
        break;
    case PB_WIRE_F32:
        p += 4;
        break;
    default:
        goto err;
    }
    if (p > e) {
        goto err;
    }
    return p;
err:
    return e;
}


int pb_skipto_1(const char **p, const char *e, uint8_t tag) {
    for (;;) {
        int d = pb_cmp_tag_1(*p, e, tag);
        if (d == 0) {
            return 1;
        } else if (d > 0) {
            return 0;
        }
        *p = skip(*p + 1, e, **p);
    }
}
int pb_skipto_2(const char **p, const char *e, uint16_t tag) {
    for (;;) {
        int d = pb_cmp_tag_2(*p, e, tag);
        if (d == 0) {
            return 1;
        } else if (d > 0) {
            return 0;
        }
        *p = skip(*p + 2, e, **p);
    }
}
int pb_skipto_3(const char **p, const char *e, uint32_t tag) {
    for (;;) {
        int d = pb_cmp_tag_3(*p, e, tag);
        if (d == 0) {
            return 1;
        } else if (d > 0) {
            return 0;
        }
        *p = skip(*p + 3, e, **p);
    }
}
int pb_skipto_4(const char **p, const char *e, uint32_t tag) {
    for (;;) {
        int d = pb_cmp_tag_4(*p, e, tag);
        if (d == 0) {
            return 1;
        } else if (d > 0) {
            return 0;
        }
        *p = skip(*p + 4, e, **p);
    }
}


const char *pb_toend_1(const char *p, const char *e) {
    while (pb_cmp_tag_1(p, e, 0x80) < 0) {
        p = skip(p+1, e, *p);
    }
    return p;
}
const char *pb_toend_2(const char *p, const char *e) {
    while (pb_cmp_tag_2(p, e, 0x4000) < 0) {
        p = skip(p+2, e, *p);
    }
    return p;
}
const char *pb_toend_3(const char *p, const char *e) {
    while (pb_cmp_tag_3(p, e, 0x200000) < 0) {
        p = skip(p+3, e, *p);
    }
    return p;
}
