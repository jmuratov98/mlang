#include "mlang/stream.h"

#include "mlang/containers/vector.h"

#include "mlang/debug/mem.h"

#include <stdio.h>

void stream_init(stream_t *stream, const char * path) {
    stream->path = path;
}

char *read_file(stream_t *stream) {
    FILE *fp = fopen(stream->path, "rb");
    if(!fp) {
        perror("read_file:   Error opening file\n");
        return NULL;
    }

    fseek(fp, 0, SEEK_END);
    size_t length = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char *buffer = xmalloc(length * sizeof(char));

    fread(buffer, sizeof(char), length, fp);
    buffer[length] = '\0';

    fclose(fp);

    return buffer;
}