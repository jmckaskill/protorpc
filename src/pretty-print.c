#define BUILDING_PROTORPC
#include "../protorpc.h"

static int add_newline(pb_buf_t *o, int depth) {
    char *p = o->next;
    if (p + depth + 1 > o->end) {
        return -1;
    }
    *(p++) = '\n';
    while (depth) {
        *(p++) = '\t';
        depth--;
    }
    o->next = p;
    return 0;
}

static const char *copy_string(pb_buf_t *o, const char *in, const char *end) {
	const char *begin = in;
	while (in < end) {
        switch (*in) {
		case '\0':
        case '\r':
        case '\n':
            return NULL;
        case '\"':
            in++;
            if (pb_append(o, begin, in - begin)) {
                return NULL;
            }
            return in;
        case '\\':
			in++;
			if (in == end) {
				return NULL;
			}
            break;
        }
        in++;
    }
	return NULL;
}

int pb_pretty_print(pb_buf_t *o, const char *in, int len) {
	char *begin = o->next;
    int depth = 0;
    bool just_opened = false;
	bool just_comma = false;
	const char *end = in + len;

	while (in < end) {
        switch (*in) {
        case ' ':
        case '\t':
        case '\n':
        case '\r':
            // strip existing whitespace
            in++;
            break;
        case '{':
        case '[':
			if (pb_append(o, in, 1)) {
                goto err;
            }
            just_opened = true;
			just_comma = false;
            depth++;
            in++;
            break;
        case '}':
        case ']':
            depth--;
            // collapse {} or [] with no entries 
            if (!just_opened && add_newline(o, depth)) {
                goto err;
            }
            just_opened = false;
			just_comma = false;
            if (pb_append(o, in, 1)) {
                goto err;
            }
            in++;
            break;
        case ':':
            if (pb_append(o, ": ", 2)) {
                goto err;
            }
            in++;
            break;
        case ',':
			just_comma = true;
            in++;
            break;
        case '"':
            // strings
			if (just_comma) {
				if (pb_append(o, ",", 1) || add_newline(o, depth)) {
                    goto err;
                }
				just_comma = false;
			} else if (just_opened) {
                if (add_newline(o, depth)) {
                    goto err;
                }
                just_opened = false;
            }
			if (pb_append(o, "\"", 1)) {
				goto err;
			}
            in = copy_string(o, in+1, end);
            if (!in) {
				goto err;
            }
            break;
        default:
            // numbers, booleans, etc
			if (just_comma) {
                if (pb_append(o, ",", 1) || add_newline(o, depth)) {
                    goto err;
                }
				just_comma = false;
			} else if (just_opened) {
				if (add_newline(o, depth)) {
                    goto err;
                }
				just_opened = false;
			}
			if (pb_append(o, in, 1)) {
                goto err;
            }
            in++;
            break;
        }
    }

	if (depth || pb_append(o, "\n", 1)) {
        goto err;
    }

    return 0;

err:
    o->next = begin;
	return -1;
}
