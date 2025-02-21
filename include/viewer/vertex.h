#pragma once

/**
 * Utilities for managing OpenGL's vertex/index arrays
 */

#include <stddef.h>
#include <gl_includes.h>

/**
 * The value returned if any of these functions successfully execute
 */
#define VERTEX_SUCCESS 0

/**
 * The value returned if any of these functions recieves invalid input
 */
#define VERTEX_ERROR_PARAMS -1

/**
 * The value returned if any of these functions encounters an allocator error
 */
#define VERTEX_ERROR_ALLOC -3

/**
 * The capacity of a vertex array created using varray_make()
 */
#define VARRAY_DEFAULT_CAPACITY 16

/**
 * The capacity of an index array created using iarray_make()
 */
#define IARRAY_DEFAULT_CAPACITY 8

/**
 * @brief The type of item stored in a vertex array
 */
typedef GLfloat varray_item_t;

/**
 * @brief A vertex array.  Used by OpenGL to render objects
 */
struct VertexArray {
    varray_item_t *array;
    size_t size;
    size_t stride;
    size_t capacity;
};
typedef struct VertexArray varray_t;

/**
 * @brief Creates a vertex array
 * @param initial_capacity The initial capacity of the array
 * @param stride The number of items in a single vertex
 * @return A pointer to the array on success, or NULL on failure
 */
varray_t *varray_create(size_t initial_capacity, size_t stride);

/**
 * Creates a vertex array with the default initial capacity
 */
#define varray_make(stride) varray_create(VARRAY_DEFAULT_CAPACITY, stride)

/**
 * @brief Adds a standard 3-valued vertex to the array
 * @param array The array to add to
 * @param x The x-component of the new vertex
 * @param y The y-component of the new vertex
 * @param z The z-component of the new vertex
 * @return 0 on success, -1 on failure
 */
int varray_add_vertex(varray_t *array, varray_item_t x, varray_item_t y, varray_item_t z);

/**
 * @brief Adds a single value to the array
 * @param array The array to add to
 * @param value The value to add
 * @return 0 on success, -1 on failure
 */
int varray_add_float(varray_t *array, varray_item_t value);

/**
 * @brief Adds multiple values to the array
 * @param array The array to add to
 * @param floats The values to add
 * @param amount_to_add The amount of values to add
 * @return 0 on success, -1 on failure
 */
int varray_add_floats(varray_t *array, varray_item_t *floats, size_t amount_to_add);

/**
 * @brief Clears the contents of the array
 * @param array The array to clear
 * @note This function may not actually zero out its contents
 */
int varray_clear(varray_t *array);

/**
 * @brief Draws the vertex array
 * @param mode The mode OpenGL will use to draw the array
 * @param array The array to draw
 */
void varray_draw(GLenum mode, varray_t *array);

/**
 * @brief Frees the vertex array
 * @param array The array to free
 */
void varray_destroy(varray_t *array);

/**
 * @brief The type of item stored in an index array
 */
typedef GLint iarray_item_t;

/**
 * @brief An index array.  Used with a vertex array to prevent repetition
 */
struct IndexArray {
    iarray_item_t *array;
    size_t size;
    size_t capacity;
};
typedef struct IndexArray iarray_t;

/**
 * @brief Creates an index array
 * @param initial_capacity The initial capacity of the array
 * @return A pointer to the array on success, or NULL on failure
 */
iarray_t *iarray_create(size_t initial_capacity);

/**
 * @brief Adds an index to the array
 * @param array The array to add to
 * @param index The index to add
 * @return 0 on success, or -1 on failure
 */
int iarray_add_index(iarray_t *array, iarray_item_t index);

/**
 * @brief Adds indices to the array
 * @param array The array to add to
 * @param indices The indices to add
 * @param index_count The number of indices to add
 * @return 0 on success, or -1 on failure
 */
int iarray_add_indices(iarray_t *array, iarray_item_t *indices, size_t index_count);

/**
 * @brief Uses an index array to draw vertices in a vertex array
 * @param mode The mode OpenGL will use to draw the array
 * @param indices The index array
 * @param vertices The vertex array
 */
void iarray_draw(GLenum mode, iarray_t *indices, varray_t *vertices);

/**
 * @brief Clears the contents of the array
 * @param array The array to clear
 * @note This function may not actually zero out its contents
 */
int iarray_clear(iarray_t *array);

/**
 * @brief Frees an index array
 * @param array The array to free
 */
void iarray_destroy(iarray_t *array);
