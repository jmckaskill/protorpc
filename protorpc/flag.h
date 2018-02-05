#pragma once

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef int(*flag_exit_fn)(int code, char *msg);
extern flag_exit_fn flag_exit;

void flag_bool(bool *p, char shopt, const char *longopt, const char *usage);
void flag_int(int *p, char shopt, const char *longopt, const char *usage);
void flag_double(double *p, char shopt, const char *longopt, const char *usage);
void flag_string(const char **p, char shopt, const char *longopt, const char *usage);
int flag_parse(int *argc, char **argv, const char *usage, int minargs);

#ifdef __cplusplus
}
#endif
