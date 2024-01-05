#ifndef _MLANG_COMMON_H_
#define _MLANG_COMMON_H_

#include "types.h"

typedef struct {
    const char* filename;
    uint64_t line, column;
} location_t;

#endif // _MLANG_COMMON_H_