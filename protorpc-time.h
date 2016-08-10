#pragma once
#include "protorpc-dll.h"
#include <os/time.h>

PROTORPC_API char *pb_parse_time(char *p, os_time_t *pv);
PROTORPC_API char *pb_print_time(char *p, os_time_t v);
PROTORPC_API const char *pb_get_time(const char *p, const char *e, os_time_t *pv);
PROTORPC_API char *pb_put_time(char *p, os_time_t v);
