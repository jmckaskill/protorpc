#pragma once

#include <string.h>

// P should point to a structure of the form
// struct {
//	int len;
//  <type> const ** v;
// };

#define pv_append(P, V)     ((P)->v[(P)->len++] = (V))
#define pv_remove(P, IDX)   memmove((void*) &(P)->v[IDX], (void*) &(P)->v[(IDX)+1], sizeof(void*) * (((P)->len--) - (IDX) - 1))
