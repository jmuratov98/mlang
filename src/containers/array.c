#include "containers/array.h"

#include <stdlib.h>
#include <string.h>

typedef struct {
    size_t capacity, size, stride;
} array_header;

#define _array_calculate_header(a) (array_header*)((uint8_t*)(a) - sizeof(array_header))

static void* _array_grow(void* array, size_t new_cap);

void* _array_reserve(size_t stride, size_t cap) {
    size_t header_size = sizeof(array_header);
    size_t body_size = cap * stride;
    
    void* array = calloc(1, header_size + body_size);

    array_header* header = array;
    header->capacity = cap;
    header->size = 0;
    header->stride = stride;
    
    return (void*)((uint8_t*)array + header_size); 
}

void _array_free(void* array) {
    array_header* header = _array_calculate_header(array);
    free(header);
}

void* _array_push_back(void* array, void* value) {
    array_header* header = _array_calculate_header(array);
    if(header->size >= header->capacity)
        array = _array_grow(array, header->capacity * 2);

    header = _array_calculate_header(array);
    
    uint64_t addr = (uint64_t)array;
    addr += (header->size * header->stride);
    memcpy((void*)addr, value, header->stride);

    header->size++;

    return array;
}

void* _array_grow(void* array, size_t new_cap) {
    array_header* header = _array_calculate_header(array);
    size_t stride = header->stride;
    size_t header_size = sizeof(array_header); 
    size_t total_size = header_size + (stride * new_cap);
    header->capacity = new_cap;

    void* temp = realloc(header, total_size);

    return (void*)((uint8_t*)temp + header_size);
}

size_t _array_size(void* array) {
    array_header* header = _array_calculate_header(array);
    return header->size;
}

size_t _array_capacity(void* array) {
    array_header* header = _array_calculate_header(array);
    return header->capacity;
}