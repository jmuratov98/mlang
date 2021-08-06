#include "mlang/debug/mem.h"

// refactor this
#ifdef MLANG_DEBUG

    #include <stdlib.h>
    #include <inttypes.h>
    #include <stdio.h>
    #include <string.h>

    typedef enum {
        VECTOR, TOKEN, LEXER,
        TOTAL_MEMORY_SIZE,
    } mem_type_e;

    typedef struct {
        uint64 total_mem;
        uint64 memory[TOTAL_MEMORY_SIZE];
    } mem_info_t;

    static mem_info_t mem_info;

    void *_xmalloc(uint64 size, const char *file, int line, const char *func) {
        mem_info.total_mem += size;
        if(strcmp(func, "_vector_create") == 0) mem_info.memory[VECTOR] += size;
        if(strcmp(func, "token_create") == 0) mem_info.memory[TOKEN] += size;
        if(strcmp(func, "read_file") == 0) mem_info.memory[LEXER] += size;
        void *block = malloc(size);
        printf ("total allocated = %s, %i, %s, %p[%li]\n", file, line, func, block, size);
        for (uint64 i = 0; i < TOTAL_MEMORY_SIZE; i++) {
            printf("type = %d, memory allocated = %llu\n", i, mem_info.memory[i]);
        }
        return block;
    }

    void _xfree(void *block, uint64 size, const char *file, int line, const char *func) {
        mem_info.total_mem -= size;
        if(strcmp(func, "_vector_destroy") == 0) mem_info.memory[VECTOR] -= size;
        if(strcmp(func, "token_destroy") == 0) mem_info.memory[TOKEN] -= size;
        if(strcmp(func, "lexer_destroy") == 0) mem_info.memory[LEXER] -= size;
        printf ("total free = %s, %i, %s, %p[%li]\n", file, line, func, block, size);
        for (uint64 i = 0; i < TOTAL_MEMORY_SIZE; i++) {
            printf("type = %d, memory allocated = %llu\n", i, mem_info.memory[i]);
        }
        return free(block);
    }

#endif