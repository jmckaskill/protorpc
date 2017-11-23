#define BUILDING_PROTORPC
#include "../protobuf.h"


char *pb_put_u32(char *p, uint32_t v) {
    while (v >= 0x80) {
        *(uint8_t*) p = 0x80 | (uint8_t) v;
		p++;
        v >>= 7;
    }
	*(uint8_t*) p = (uint8_t) v;
    return p + 1;
}
char *pb_put_u64(char *p, uint64_t v) {
    while (v >= 0x80) {
        *(uint8_t*) p = 0x80 | (uint8_t) v;
		p++;
        v >>= 7;
    }
    *p++ = (uint8_t) v;
    return p;
}
char *pb_put_s32(char *p, int32_t v) {
    return pb_put_u32(p, ((uint32_t)(v) << 1) ^ (uint32_t)(v >> 31));
}
char *pb_put_s64(char *p, int64_t v) {
    return pb_put_u64(p, ((uint64_t)(v) << 1) ^ (uint64_t)(v >> 63));
}

char *pb_put_packed_u32(char *p, const uint32_t *v, int len) {
    int szlen = pb_u32_size(len * 5);
    char *start = p = p + szlen;
    for (int i = 0; i < len; i++) {
        p = pb_put_u32(p, v[i]);
    }
    pb_put_finish(p, start, szlen);
    return p;
}
char *pb_put_packed_u64(char *p, const uint64_t *v, int len) {
    int szlen = pb_u32_size(len * 10);
    char *start = p = p + szlen;
    for (int i = 0; i < len; i++) {
        p = pb_put_u64(p, v[i]);
    }
    pb_put_finish(p, start, szlen);
    return p;
}
char *pb_put_packed_s32(char *p, const int32_t *v, int len) {
    int szlen = pb_u32_size(len * 5);
    char *start = p = p + szlen;
    for (int i = 0; i < len; i++) {
        p = pb_put_s32(p, v[i]);
    }
    pb_put_finish(p, start, szlen);
    return p;
}
char *pb_put_packed_s64(char *p, const int64_t *v, int len) {
    int szlen = pb_u32_size(len * 10);
    char *start = p = p + szlen;
    for (int i = 0; i < len; i++) {
        p = pb_put_s64(p, v[i]);
    }
    pb_put_finish(p, start, szlen);
    return p;
}

int pb_u32_size(int sz) {
    if (sz < 0x80) {
        return 1;
    } else if (sz < 0x4000) {
        return 2;
    } else if (sz < 0x200000) {
        return 3;
    } else {
        return 4;
    }
}

void pb_put_finish(char *p, char *start, int szlen) {
	uint8_t *h = (uint8_t*) start;
    size_t sz = p - start;
    switch (szlen) {
    case 1:
        h[-1] = (uint8_t) sz;
        break;
    case 2:
        h[-2] = (uint8_t) (sz & 0x7F) | 0x80;
        h[-1] = (uint8_t) (sz >> 7);
        break;
    case 3:
        h[-3] = (uint8_t) (sz & 0x7F) | 0x80;
        h[-2] = (uint8_t) ((sz >> 7) & 0x7F) | 0x80;
        h[-1] = (uint8_t) (sz >> 14);
        break;
    case 4:
        h[-4] = (uint8_t) (sz & 0x7F) | 0x80;
        h[-3] = (uint8_t) ((sz >> 7) & 0x7F) | 0x80;
        h[-2] = (uint8_t) ((sz >> 14) & 0x7F) | 0x80;
        h[-1] = (uint8_t) (sz >> 21);
        break;
    }
}
