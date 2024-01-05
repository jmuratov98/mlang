#pragma once

#include "common.h"

typedef struct {
    const char* filename;
} file_t;

void stream_init(file_t* file, const char* filename);
void stream_deinit(file_t* file);

void stream_get(file_t file);
void stream_getn(file_t file, size_t n);

char stream_peek(file_t file);
char stream_peekn(file_t file, size_t n);

