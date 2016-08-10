#define BUILDING_PROTORPC
#include <protorpc/protorpc.h>

static void add_newline(str_t *o, int depth) {
	str_grow(o, o->len + depth + 1);
	o->buf[o->len++] = '\n';
    while (depth) {
		o->buf[o->len++] = '\t';
        depth--;
    }
}

static const char *copy_string(str_t *o, const char *in, const char *end) {
	const char *begin = in;
	while (in < end) {
        switch (*in) {
		case '\0':
        case '\r':
        case '\n':
            return NULL;
        case '\"':
            in++;
			str_add2(o, begin, (int) (in - begin));
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

int pb_pretty_print(str_t *o, const char *in, int len) {
	int begin = o->len;
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
			str_addch(o, *in);
            just_opened = true;
			just_comma = false;
            depth++;
            in++;
            break;
        case '}':
        case ']':
            depth--;
            // collapse {} or [] with no entries 
            if (!just_opened) {
                add_newline(o, depth);
            }
            just_opened = false;
			just_comma = false;
			str_addch(o, *in);
            in++;
            break;
        case ':':
			str_add(o, ": ");
            in++;
            break;
        case ',':
			just_comma = true;
            in++;
            break;
        case '"':
            // strings
			if (just_comma) {
				str_addch(o, ',');
				add_newline(o, depth);
				just_comma = false;
			} else if (just_opened) {
                add_newline(o, depth);
                just_opened = false;
            }
			str_addch(o, '\"');
            in = copy_string(o, in+1, end);
            if (!in) {
				goto err;
            }
            break;
        default:
            // numbers, booleans, etc
			if (just_comma) {
				str_addch(o, ',');
				add_newline(o, depth);
				just_comma = false;
			} else if (just_opened) {
				add_newline(o, depth);
				just_opened = false;
			}
			str_addch(o, *in);
            in++;
            break;
        }
    }

	if (depth) {
		goto err;
	}

	str_addch(o, '\n');
    return 0;

err:
	str_setlen(o, begin);
	return -1;
}
