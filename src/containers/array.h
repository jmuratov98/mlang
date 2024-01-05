#pragma once

#include "common.h"

#define ARRAY_DEFAULT_CAPACITY          10

#define array(T)                        T*

#define array_create(T)                 _array_reserve(sizeof(T), ARRAY_DEFAULT_CAPACITY)
#define array_reserve(T, cap)           _array_reserve(sizeof(T), cap)

#define array_free(a)                   _array_free(a)

#define array_push_back(a, vp)          { typeof(vp) temp = vp; a = _array_push_back(a, &temp); }

#define array_size(a)                   _array_size(a)
#define array_capacity(a)               _array_capacity(a)

void* _array_reserve(size_t stride, size_t cap);
void  _array_free(void* array);
void* _array_push_back(void* array, void* value_ptr);

size_t _array_size(void* array);
size_t _array_capacity(void* array);