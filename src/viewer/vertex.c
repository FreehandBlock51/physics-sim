#include "viewer/vertex.h"

#include <stdlib.h>
#include <malloc.h>
#include "common/defines.h"

varray_t *varray_create(size_t initial_capacity) {
    varray_t *array = calloc(1, (sizeof *array));
    if (array == NULL) {
        return NULL;
    }
    array->size = 0;
    array->array = calloc(initial_capacity, (sizeof *array->array));
    if (array->array == NULL) {
        free(array);
        return NULL;
    }
    array->capacity = initial_capacity;

    return array;
}

PRIVATE_FUNC int varray_resize_capacity(varray_t *array, size_t new_capacity) {
    if (array->array != NULL && new_capacity < array->size) {
        return VERTEX_ERROR_PARAMS;
    }
    array->array = reallocarray(array->array, new_capacity, (sizeof *array->array));
    if (array->array == NULL) {
        return VERTEX_ERROR_ALLOC;
    }
    array->capacity = new_capacity;
    return VERTEX_SUCCESS;
}

#define varray_double_size(array) varray_resize_capacity(array, array->capacity * 2)

PRIVATE_FUNC inline int _varray_double_capacity_if_too_small(varray_t *array, size_t needed_space) {
    // include null check here b/c this should be called before any addition to the array
    // reallocarray() can handle null, so a null array isn't invalid input
    if (array->size + needed_space < array->capacity || array->array == NULL) {
        return varray_double_size(array);
    }
    return VERTEX_SUCCESS;
}

#define VARRAY_DOUBLE_CAPACITY_IF_TOO_SMALL(array, needed_space) { int result; if ((result = _varray_double_capacity_if_too_small(array, needed_space)) != VERTEX_SUCCESS) return result; }

PRIVATE_FUNC inline void varray_append_without_capacity_check(varray_t *array, varray_item_t value) {
    array->array[array->size++] = value;
}
PRIVATE_FUNC int varray_shrink_to_current_size(varray_t *array) {
    if (array->capacity < array->size) {
        return VERTEX_ERROR_PARAMS;
    }
    if (array->capacity == array->size) {
        return VERTEX_SUCCESS;
    }

    array->array = reallocarray(array->array, array->size, (sizeof *array->array));
    if (array->array == NULL) {
        return VERTEX_ERROR_ALLOC;
    }

    array->capacity = array->size;
    return VERTEX_SUCCESS;
}


int varray_add_vertex(varray_t *array, varray_item_t x, varray_item_t y, varray_item_t z) {
    VARRAY_DOUBLE_CAPACITY_IF_TOO_SMALL(array, 3)

    varray_append_without_capacity_check(array, x);
    varray_append_without_capacity_check(array, y);
    varray_append_without_capacity_check(array, z);
    return VERTEX_SUCCESS;
}

int varray_add_float(varray_t *array, varray_item_t value) {
    VARRAY_DOUBLE_CAPACITY_IF_TOO_SMALL(array, 1)
    varray_append_without_capacity_check(array, value);
    return VERTEX_SUCCESS;
}

int varray_add_floats(varray_t *array, varray_item_t *floats, size_t amount_to_add) {
    VARRAY_DOUBLE_CAPACITY_IF_TOO_SMALL(array, amount_to_add)
    for (size_t i = VERTEX_SUCCESS; i < amount_to_add; i++) {
        varray_append_without_capacity_check(array, floats[i]);
    }
    return VERTEX_SUCCESS;
}

int varray_clear(varray_t *array) {
    FAIL_UNLESS(int, varray_shrink_to_current_size(array), return)

    array->size = 0; // faster than clearing the memory

    return VERTEX_SUCCESS;
}

void varray_draw(GLenum mode, varray_t *array) {
    // Create a Vertex Buffer Object (VBO) to store our vertices
    static GLuint VBO = 0;
    if (VBO == 0) {
        glGenBuffers(1, &VBO);
    }

    // put vertices in the VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, array->size * (sizeof *array->array), array->array, GL_STATIC_DRAW);

    size_t vertex_stride = TODO(); // the size of a single vertex
    glVertexAttribPointer(0, array->size / vertex_stride, GL_FLOAT, GL_FALSE, vertex_stride, (void*)0);
    glEnableVertexAttribArray(0);

    glDrawArrays(mode, 0, array->size);
}

void varray_destroy(varray_t *array) {
    free(array->array);
    free(array);
}

iarray_t *iarray_create(size_t initial_capacity) {
    iarray_t *array = calloc(1, (sizeof *array));
    if (array == NULL) {
        return NULL;
    }

    array->size = 0;
    array->array = calloc(initial_capacity, (sizeof *array->array));
    if (array->array == NULL) {
        free(array);
        return NULL;
    }
    array->capacity = initial_capacity;

    return array;
}

PRIVATE_FUNC int iarray_resize_capacity(iarray_t *array, size_t new_capacity) {
    if (array->array != NULL && new_capacity < array->size) {
        return VERTEX_ERROR_PARAMS;
    }
    array->array = reallocarray(array->array, new_capacity, (sizeof *array->array));
    if (array->array == NULL) {
        return VERTEX_ERROR_ALLOC;
    }
    array->capacity = new_capacity;
    return VERTEX_SUCCESS;
}

#define iarray_double_size(array) iarray_resize_capacity(array, array->capacity * 2)

PRIVATE_FUNC inline int _iarray_double_capacity_if_too_small(iarray_t *array, size_t needed_space) {
    // include null check here b/c this should be called before any addition to the array
    // reallocarray() can handle null, so a null array isn't invalid input
    if (array->size + needed_space < array->capacity || array->array == NULL) {
        return iarray_double_size(array);
    }
    return VERTEX_SUCCESS;
}

#define IARRAY_DOUBLE_CAPACITY_IF_TOO_SMALL(array, needed_space) { int result; if ((result = _iarray_double_capacity_if_too_small(array, needed_space)) != VERTEX_SUCCESS) return result; }

PRIVATE_FUNC inline void iarray_append_without_capacity_check(iarray_t *array, iarray_item_t value) {
    array->array[array->size++] = value;
}
PRIVATE_FUNC int iarray_shrink_to_current_size(iarray_t *array) {
    if (array->capacity < array->size) {
        return VERTEX_ERROR_PARAMS;
    }
    if (array->capacity == array->size) {
        return VERTEX_SUCCESS;
    }

    array->array = reallocarray(array->array, array->size, (sizeof *array->array));
    if (array->array == NULL) {
        return VERTEX_ERROR_ALLOC;
    }

    array->capacity = array->size;
    return VERTEX_SUCCESS;
}

int iarray_add_index(iarray_t *array, iarray_item_t index) {
    IARRAY_DOUBLE_CAPACITY_IF_TOO_SMALL(array, 1)
    iarray_append_without_capacity_check(array, index);
    return VERTEX_SUCCESS;
}

int iarray_add_indices(iarray_t *array, iarray_item_t *indices, size_t index_count) {
    IARRAY_DOUBLE_CAPACITY_IF_TOO_SMALL(array, index_count)
    for (size_t i = 0; i < index_count; i++) {
        iarray_append_without_capacity_check(array, indices[i]);
    }
    return VERTEX_SUCCESS;
}

void iarray_draw(GLenum mode, iarray_t *indices, varray_t *vertices) {
    // Create a Vertex Buffer Object (VBO) to store our vertices
    static GLuint VBO = 0;
    // Create an Element Buffer Object (EBO) to store our vertices
    static GLuint EBO = 0;
    if (VBO == 0) {
        glGenBuffers(1, &VBO);
    }

    // put vertices in the VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices->size * (sizeof *vertices->array), vertices->array, GL_STATIC_DRAW);

    // put indices in the EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices->size * (sizeof *indices->array), indices->array, GL_STATIC_DRAW);

    size_t vertex_stride = TODO(); // the size of a single vertex
    glVertexAttribPointer(0, vertices->size / vertex_stride, GL_FLOAT, GL_FALSE, vertex_stride, (void*)0);
    glEnableVertexAttribArray(0);



    glDrawArrays(mode, 0, vertices->size);
}

int iarray_clear(iarray_t *array) {
    FAIL_UNLESS(int, iarray_shrink_to_current_size(array), return)

    array->size = 0; // faster than clearing the memory

    return VERTEX_SUCCESS;
}

void iarray_destroy(iarray_t *array) {
    free(array->array);
    free(array);
}

ivarray_t *ivarray_create(size_t initial_vertex_capacity, size_t initial_index_capacity, size_t vertex_size) {
    ivarray_t *array = calloc(1, (sizeof *array));
    if (array == NULL) {
        return NULL;
    }

    array->vertex_size = vertex_size;

    array->vertices = varray_create(initial_vertex_capacity);
    if (array->vertices == NULL) {
        free(array);
        return NULL;
    }

    array->indexes = iarray_create(initial_index_capacity);
    if (array->indexes == NULL) {
        free(array->vertices);
        free(array);
        return NULL;
    }

    return array;
}

int ivarray_add_vertex(ivarray_t *array, varray_item_t *vertex, size_t vertex_size) {
    return TODO();
}

int ivarray_add_vertices(ivarray_t *array, varray_item_t *vertices, size_t vertex_size, size_t vertex_count) {
    return TODO();
}

void ivarray_draw(GLenum mode, ivarray_t *array) {
    iarray_draw(mode, array->indexes, array->vertices);
}

void ivarray_destroy(ivarray_t *array) {
    iarray_destroy(array->indexes);
    varray_destroy(array->vertices);
    free(array);
}

