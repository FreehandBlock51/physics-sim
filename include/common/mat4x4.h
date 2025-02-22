#pragma once

/**
 * Utilities and defines for a 4x4 Matrix
 */

#include "common/defines.h"
#include "common/vec3.h"
#include "common/vec4.h"

/**
 * A Matrix with 4 rows and 4 columns
 */
typedef phy_real_t mat4x4_t[4][4];

/**
 * The identity 4x4 matrix. Any matrix multiplied by this will remain
 * unchanged.
 */
#define MAT4x4_IDENTITY ((mat4x4_t) {  { 1, 0, 0, 0 }, \
                                       { 0, 1, 0, 0 }, \
                                       { 0, 0, 1, 0 }, \
                                       { 0, 0, 0, 1 }  })

/**
 * Multiplies matrix A by matrix B, storing the result (AxB) into dest
 */
void mat4x4_times_mat4x4(mat4x4_t dest, mat4x4_t a, mat4x4_t b);

/**
 * Populates the given matrix with the 4x4 identity matrix
 */
void mat4x4_make_identity(mat4x4_t matrix);

/**
 * @brief Prints a matrix to standard output
 * @param matrix The matrix to print
 */
void mat4x4_print(mat4x4_t matrix);
