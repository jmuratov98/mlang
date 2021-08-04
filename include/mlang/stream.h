#pragma once

typedef struct {
    const char *path;
} stream_t;

void stream_init(stream_t* stream, const char *path);

char *read_file(stream_t* stream);