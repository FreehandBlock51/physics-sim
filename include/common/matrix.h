#pragma once

/**
 * Implementation macros for the matNxM structs
 * This header was an attempt to 'speed up' the matrix definition
 * process by providing a worse version of generics.  All it ended
 * up doing was making debugging impossible.  Do not use this file.
 * It just isn't worth it
 */

#include <stddef.h>
#include "common/defines.h"

/**
 * The name of an arbitrarily large matrix
 */
#define MATRIX(width, height) mat ## width ## x ## height

/**
 * The typename of an arbitrarily large matrix
 */
#define MATRIX_T(width, height) JOIN2(MATRIX(width, height), _t)


/**
 * The typedef for an arbitrarily large matrix
 */
#define MATRIX_TYPEDEF(width, height) \
  typedef phy_real_t MATRIX_T(width, height) [height] [width]

/**
 * Can be used to either get or set the value of a given matrix cell
 */
#define MATRIX_CELL(matrix, width, height, row, column) \
    (matrix)[row][column]
    //~ matrix[row * width + column]

/**
 * Multiplies A by B (AxB) and stores the result in dest.
 * A must be an (a_height)x(common_dim) matrix,
 * B must be a (common_dim)x(b_width) matrix,
 * and dest must be a (a_height)x(b_width) matrix
 */
#define MATRIX_TIMES_MATRIX(                                           \
        dest,                                                          \
        a_width, a,                                                    \
        b_height, b,                                                   \
        common_dim                                                     \
)                                                                      \
{                                                                      \
    for (size_t row = 0; row < b_height; row++) {                      \
        for (size_t column = 0; column < a_width; column++) {          \
            MATRIX_CELL(dest, a_width, b_height, row, column) = 0;     \
            for (size_t common = 0; common < common_dim; common++) {   \
                MATRIX_CELL(dest, a_width, b_height, row, column) +=   \
                    MATRIX_CELL(a, a_width, common_dim, common, column) * \
                    MATRIX_CELL(b, common_dim, b_height, row, common); \
            }                                                          \
        }                                                              \
    }                                                                  \
}

// mat ## a_width ## x ## common_dim ## _times_ ## mat ## common_dim ## x ## b_height
/**
 * Declares a function for matrix-by-matrix multiplication
 */
#define MATRIX_TIMES_MATRIX_DECL(a_width, b_height, common_dim)        \
void                                                                   \
JOIN3(                                                                 \
        MATRIX(a_width, common_dim),                                   \
        _times_,                                                       \
        MATRIX(common_dim, b_height)                                   \
)                                                                      \
(                                                                      \
  MATRIX_T(a_width, b_height) dest,                                    \
  MATRIX_T(a_width, common_dim) a,                                     \
  MATRIX_T(common_dim, b_height) b                                     \
)

/**
 * Implements a function for matrix-by-matrix multiplication
 */
#define MATRIX_TIMES_MATRIX_IMPL(a_width, b_height, common_dim)        \
MATRIX_TIMES_MATRIX_DECL(a_width, b_height, common_dim)                \
MATRIX_TIMES_MATRIX(dest, a_width, a, b_height, b, common_dim)

/**
 * Makes the given square matrix into an identity matrix
 */
#define SQUARE_MATRIX_MAKE_IDENTITY(matrix, size) {                 \
    for (size_t row = 0; row < size; row++) {                       \
        for (size_t column = 0; column < size; column++) {          \
            if (row == column) {                                    \
                MATRIX_CELL(matrix, size, size, row, column) = 1;   \
            }                                                       \
            else {                                                  \
                MATRIX_CELL(matrix, size, size, row, column) = 1;   \
            }                                                       \
        }                                                           \
    }                                                               \
}

// mat ## size ## x ## size ## _make_identity
/**
 * Declares the matrix identity function
 */
#define SQUARE_MATRIX_MAKE_IDENTITY_DECL(size)                      \
void                                                                \
JOIN2(MATRIX(size, size), _make_identity)(MATRIX_T(size, size) matrix)


/**
 * Implements the matrix identity function
 */
#define SQUARE_MATRIX_MAKE_IDENTITY_IMPL(size)  \
SQUARE_MATRIX_MAKE_IDENTITY_DECL(size)          \
SQUARE_MATRIX_MAKE_IDENTITY(matrix, size)
