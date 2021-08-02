#include "mlang/common.h"

// Memory allocated:
// capacity {uint64}: the max size of the array
// size {uint64}: the size of the array
// stride {uint64}: the length of an element
// array {void *}: The dynamically allocated array

#define VECTOR_CAPACITY     0
#define VECTOR_SIZE         1
#define VECTOR_STRIDE       2
#define VECTOR_ARRAY        3

#define VECTOR_DEFAULT_CAPACITY 5
#define VECTOR_RESIZE_FACTOR    2

void *_vector_create(uint64 capacity, uint64 stride);
void _vector_destroy(void *vector);

void *_vector_push_back(void *vector, void *value_ptr);
void _vector_pop_back(void *vector, void *value_ptr);

void *_vector_emplace(void *vector, uint64 index, void *value_ptr);
void _vector_remove(void *vector, uint64 index, void *value_ptr);

uint64 _vector_get_field(void *vector, uint64 field);
void _vector_set_field(void *vector, uint64 field, uint64 value);

/**
 * @brief creates a dynamically allocated array
 * 
 * @param {type} the type of the array
 * @return {void *} the array
 */
#define vector_create(type) _vector_create(VECTOR_DEFAULT_CAPACITY, sizeof(type))

/**
 * @brief frees the dynamically allocated array
 * 
 * @param {vector} the array block
 * @return {void}
 */
#define vector_destroy(vector) _vector_destroy(vector)

/**
 * @brief adds the element to the back of the array
 * 
 * @param {vector} the array block
 * @param {value} the value to be added
 * @return {void}
 */
#define vector_push_back(vector, value) { typeof(value) temp = value; vector = _vector_push_back(vector, &temp); }

/**
 * @brief pops the last element from the array and returns the element
 * 
 * @param {vector} the array block
 * @param {void *} the value to be added
 */
#define vector_pop_back(vector, value_ptr) _vector_pop_back(vector, value_ptr)

/**
 * @brief adds the element to associated index
 * 
 * @param {vector} the array block
 * @param {index} the index in which to place the array
 * @param {value} the value to be added
 * @return {void}
 */
#define vector_emplace(vector, index, value) { typeof(value) temp = value; vector = _vector_emplace(vector, index, &temp); }

/**
 * @brief removes the element at the specified index and moves it to value_ptr
 * 
 * @param {vector} the array block
 * @param {index} the index in which to place the array
 * @param {void*} the value to be added
 */
#define vector_remove(vector, index, value_ptr) _vector_remove(vector, index, value_ptr)

/**
 * @brief gets the size of the array
 * 
 * @param {vector} the array block
 * @return {uint64} the size of the array
 */
#define vector_size(vector) _vector_get_field(vector, VECTOR_SIZE)

/**
 * @brief gets the capacity of the array
 * 
 * @param {vector} the array block
 * @return {uint64} the capacity of the array
 */
#define vector_capacity(vector) _vector_get_field(vector, VECTOR_CAPACITY)

/**
 * @brief gets the element stride
 * 
 * @param {vector} the array block
 * @return {uint64} the stride of each element
 */
#define vector_stride(vector) _vector_get_field(vector, VECTOR_STRIDE)
