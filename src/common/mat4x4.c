#include "common/mat4x4.h"

#include <stdio.h>

void mat4x4_times_mat4x4(mat4x4_t dest, mat4x4_t a, mat4x4_t b) {
    for (size_t row = 0; row < 4; row++) {
        for (size_t column = 0; column < 4; column++) {
            dest[row][column] = 0;
            for (size_t common = 0; common < 4; common++) {
                dest[row][column] += a[common][column] * b[row][common];
            }
        }
    }
}

void mat4x4_make_identity(mat4x4_t matrix) {
    for (size_t row = 0; row < 4; row++) {
        for (size_t column = 0; column < 4; column++) {
            if (row == column) {
                matrix[row][column] = 1;
            }
            else {
                matrix[row][column] = 0;
            }
        }
    }
}

void mat4x4_print(mat4x4_t matrix) {
    for (size_t row = 0; row < 4; row++) {
        for (size_t column = 0; column < 4; column++) {
            printf("%f ", matrix[row][column]);
        }
        printf("\n");
    }
}
