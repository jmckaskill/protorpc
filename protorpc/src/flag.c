#include <protorpc/flag.h>
#include <protorpc/str.h>
#include <stdlib.h>
#include <stdio.h>

enum flag_type {
	FLAG_BOOL,
	FLAG_INT,
	FLAG_DOUBLE,
	FLAG_STRING,
};

union all_types {
	bool b;
	const char *s;
	int i;
	double d;
};

struct flag {
	enum flag_type type;
	union all_types *pval;
	char shopt;
	const char *longopt;
	int longlen;
	const char *usage;
};

static const char *g_usage;
static struct flag *g_flags;
static int g_num;
static int g_cap;

static int normal_exit(int code, char *msg) {
	fputs(msg, stderr);
	exit(code);
	return 0;
}

flag_exit_fn flag_exit = &normal_exit;

static void print_spaces(str_t *o, int num) {
	while (num > 0) {
		str_addch(o, ' ');
		num--;
	}
}

static void print_usage(str_t *o) {
	str_addf(o, "Usage: %s\n", g_usage);
	if (g_num) {
		str_addf(o, "\nMandatory arguments to long options are mandatory for short options too.\n");
		for (int i = 0; i < g_num; i++) {
			struct flag *f = &g_flags[i];

			if (f->shopt) {
				str_addf(o, "  -%c", f->shopt);
			} else {
				str_addf(o, "    ");
			}

			int pad = 24;

			if (f->longopt) {
				pad -= str_addf(o, "%c --%s", f->shopt ? ',' : ' ', f->longopt);

				switch (f->type) {
				case FLAG_INT:
					pad -= str_addf(o, "=%d", f->pval->i);
					break;
				case FLAG_DOUBLE:
					pad -= str_addf(o, "=%g", f->pval->d);
					break;
				case FLAG_STRING:
					if (f->pval->s) {
						pad -= str_addf(o, "=%s", f->pval->s);
					}
					break;
				case FLAG_BOOL:
					break;
				}

				print_spaces(o, pad);
			} else {
				switch (f->type) {
				case FLAG_INT:
					pad -= str_addf(o, " %d", f->pval->i);
					break;
				case FLAG_DOUBLE:
					pad -= str_addf(o, " %g", f->pval->d);
					break;
				case FLAG_STRING:
					if (f->pval->s) {
						pad -= str_addf(o, " %s", f->pval->s);
					}
					break;
				case FLAG_BOOL:
					break;
				}

				print_spaces(o, pad);
			}

			str_addf(o, "%s\n", f->usage);
		}
	}
}

static void append(enum flag_type type, void *p, char shopt, const char *longopt, const char *usage) {
	if (g_num == g_cap) {
		g_cap = (g_cap + 16) * 3 / 2;
		g_flags = (struct flag*) realloc(g_flags, g_cap * sizeof(*g_flags));
	}
	struct flag *f = &g_flags[g_num++];
	f->type = type;
	f->shopt = shopt;
	f->longopt = longopt;
	f->longlen = longopt ? strlen(longopt) : 0;
	f->usage = usage;
	f->pval = (union all_types *) p;
}

void flag_bool(bool *p, char shopt, const char *longopt, const char *usage) {
	append(FLAG_BOOL, p, shopt, longopt, usage);
}

void flag_int(int *p, char shopt, const char *longopt, const char *usage) {
	append(FLAG_INT, p, shopt, longopt, usage);
}

void flag_double(double *p, char shopt, const char *longopt, const char *usage) {
	append(FLAG_DOUBLE, p, shopt, longopt, usage);
}

void flag_string(const char **p, char shopt, const char *longopt, const char *usage) {
	append(FLAG_STRING, (void*) p, shopt, longopt, usage);
}

static struct flag *find_long(const char *name, int len) {
	for (int j = 0; j < g_num; j++) {
		struct flag *f = &g_flags[j];
		if (f->longopt && len == f->longlen && !memcmp(name, f->longopt, len)) {
			return f;
		}
	}
	return NULL;
}

static struct flag *find_short(char name) {
	for (int j = 0; j < g_num; j++) {
		struct flag *f = &g_flags[j];
		if (f->shopt == name) {
			return f;
		}
	}
	return NULL;
}

static int process_flag(struct flag *f, char *arg, char *value) {
	if (!value && f->type != FLAG_BOOL) {
		str_t o = STR_INIT;
		str_addf(&o, "expected value for %s\n", arg);
		print_usage(&o);
		return flag_exit(2, str_release(&o));
	}

	switch (f->type) {
	case FLAG_BOOL:
		f->pval->b = true;
		break;
	case FLAG_INT:
		f->pval->i = strtol(value, NULL, 0);
		break;
	case FLAG_DOUBLE:
		f->pval->d = strtod(value, NULL);
		break;
	case FLAG_STRING:
		f->pval->s = value;
		break;
	}

	return 0;
}

static char *remove_argument(int i, int *pargc, char **argv) {
	char *ret = argv[i];
	memmove((void*) &argv[i], (void*) &argv[i + 1], (*pargc - (i + 1)) * sizeof(argv[i]));
	(*pargc)--;
	return ret;
}

static int unknown_flag(char *arg) {
	str_t o = STR_INIT;
	str_addf(&o, "unknown flag %s\n", arg);
	print_usage(&o);
	return flag_exit(2, str_release(&o));
}

int flag_parse(int *pargc, char **argv, const char *usage, int minargs) {
	g_usage = usage;
	remove_argument(0, pargc, argv);
	for (int i = 0; i < *pargc;) {
		if (argv[i][0] != '-') {
			i++;
			continue;
		}

		char *arg = remove_argument(i, pargc, argv);

		if (!strcmp(arg, "--help") || !strcmp(arg, "-h")) {
			str_t o = STR_INIT;
			print_usage(&o);
			return flag_exit(1, str_release(&o));
		} else if (!strcmp(arg, "--")) {
			break;
		}

		int err;

		if (arg[1] == '-') {
			// long form
			int len = strlen(arg);
			char *value = memchr(arg, '=', len);
			if (value) {
				len = value - arg;
			}

			struct flag *f = find_long(arg + 2, len - 2);
			if (!f) {
				return unknown_flag(arg);
			}

			err = process_flag(f, arg, value + 1);

		} else if (arg[1] && !arg[2]) {
			// short form
			struct flag *f = find_short(arg[1]);
			if (!f) {
				return unknown_flag(arg);
			}

			char *value = NULL;
			if (f->type != FLAG_BOOL && i < *pargc) {
				value = remove_argument(i, pargc, argv);
			}

			err = process_flag(f, arg, value);
		} else {
			err = unknown_flag(arg);
		}

		if (err) {
			return err;
		}
	}

	if (*pargc < minargs) {
		str_t o = STR_INIT;
		str_addf(&o, "expected %d arguments\n", minargs);
		print_usage(&o);
		return flag_exit(2, str_release(&o));
	}
	
	// re null-terminate the argument list
	argv[*pargc] = NULL;

	free(g_flags);
	g_num = 0;
	g_cap = 0;
	return 0;
}

