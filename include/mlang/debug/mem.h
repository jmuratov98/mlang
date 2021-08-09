#pragma once

#include "mlang/common.h"

#ifdef MLANG_DEBUG

    #define xmalloc(size) _xmalloc( size, __FILE__, __LINE__, __FUNCTION__)
    #define xfree(block, size) _xfree( block, size, __FILE__, __LINE__, __FUNCTION__)

    void *_xmalloc(uint64 size, const char *file, int line, const char *func);
    void _xfree(void *block, uint64 size, const char *file, int line, const char *func);

#else
    #include <stdlib.h> 

    #define xmalloc(size) malloc(size)
    #define xfree(block, size) free(block)

#endif