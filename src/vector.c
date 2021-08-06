#include "mlang/containers/vector.h"

#include "mlang/debug/mem.h"

#include <string.h>
#include <stdio.h>

static void *_vector_grow(void *vector, uint64 new_capacity);
static void *_vector_insert_at(void *vector, uint64 index, void *value_ptr, const char *name);
static void _vector_remove_at(void *vector, uint64 index, void *value_ptr, const char *name);

void *_vector_create(uint64 capacity, uint64 stride) {
    uint64 header_size = VECTOR_ARRAY * sizeof(uint64);
    uint64 array_size = capacity * stride;
    uint64 total_size = header_size + array_size;

    uint64 *vector = xmalloc(total_size);
    memset(vector, 0, total_size);

    vector[VECTOR_CAPACITY] = capacity;
    vector[VECTOR_SIZE] = 0;
    vector[VECTOR_STRIDE] = stride;
    return (void *)(vector + VECTOR_ARRAY);
}

void _vector_destroy(void *vector) {
    uint64 *block = (uint64 *)vector - VECTOR_ARRAY;
    uint64 cap = block[VECTOR_CAPACITY];
    uint64 stride = block[VECTOR_STRIDE];

    uint64 header_size = VECTOR_ARRAY * sizeof(uint64);
    uint64 vector_size = cap * stride;
    uint64 total_size = header_size + vector_size;
    
    xfree(block, total_size);
}

void* _vector_push_back(void *vector, void *value_ptr) {
    uint64 size = vector_size(vector);
    return _vector_insert_at(vector, size, value_ptr, "vector_push_back");
}

void _vector_pop_back(void *vector, void *value_ptr) {
    uint64 size = vector_size(vector);
    _vector_remove_at(vector, size - 1, value_ptr, "vector_pop_back");
}

void *_vector_emplace(void *vector, uint64 index, void *value_ptr) {
    return _vector_insert_at(vector, index, value_ptr, "vector_emplace");
}

void _vector_remove(void *vector, uint64 index, void *value_ptr) {
    _vector_remove_at(vector, index, value_ptr, "vector_remove");
}

uint64 _vector_get_field(void *vector, uint64 field) {
    uint64 *header = (uint64 *)vector - VECTOR_ARRAY;
    return header[field];
}

void _vector_set_field(void *vector, uint64 field, uint64 value) {
    uint64 *header = (uint64 *)vector - VECTOR_ARRAY;
    header[field] = value;
}

void *_vector_grow(void *vector, uint64 new_capacity) {
    uint64 size = vector_size(vector);
    uint64 stride = vector_stride(vector);

    void *new_vector = _vector_create(new_capacity, stride);
    _vector_set_field(new_vector, VECTOR_SIZE, size);
    memcpy(new_vector, vector, size * stride);

    _vector_destroy(vector);

    return new_vector;
}

void *_vector_insert_at(void *vector, uint64 index, void *value_ptr, const char *name) {
    uint64 size = vector_size(vector);
    uint64 stride = vector_stride(vector);
    uint64 capacity = vector_capacity(vector);
    if(index > size) {
        printf("%s:  index out of range\n", name);
        return vector;
    }
    
    if(size >= capacity) {
        vector = _vector_grow(vector, VECTOR_RESIZE_FACTOR * capacity);
    }

    uint64 address = (uint64)vector;

    // Time to shift the memory to make room for the new element
    if(index < size) {
        memcpy(
            (void *)(address + ((index + 1) * stride)),
            (void *)(address + (index * stride)),
            stride * (size - index)
        );
    }

    memcpy((void *)(address + (index * stride)), value_ptr, stride);
    
    _vector_set_field(vector, VECTOR_SIZE, size + 1);

    return vector;
}

void _vector_remove_at(void *vector, uint64 index, void *value_ptr, const char *name) {
    uint64 size = vector_size(vector);
    uint64 stride = vector_stride(vector);
    if(index >= size) {
        printf("%s:  index out of range\n", "vector_remove");
        return;
    }
    
    uint64 address = (uint64)vector;
    if(value_ptr != NULL) {
        memcpy(value_ptr, (void *)(address + (index * stride)), stride);
    }

    // Time to shift the memory to have a contiguous block of memory
    if(index < size - 1) {
        memcpy(
            (void *)(address + (index * stride)),
            (void *)(address + ((index + 1) * stride)),
            stride * (size - index)
        );
    }

    _vector_set_field(vector, VECTOR_SIZE, size - 1);
}