#pragma once

/**
 * Contains utilities and definitions for colors
 */

#include "common/defines.h"

/**
 * A color defined with RGBA
 * Each value should be between 0 and 1
 */
union Color {
    float raw[4];
    struct {
        float r;
        float g;
        float b;
        float a;
    };
};
typedef union Color color_t;

/**
 * Constructs a color from the given values
 */
#define color_make(r, g, b, a) ((color_t){ { r, g, b, a } })
/**
 * Constructs an opaque color from the given values
 */
#define color_rgb(r, g, b) color_make(r, g, b, 1)

/**
 * Like color_make, but uses a range of 0-255 instead of 0-1
 */
#define color_from_bytes(r, g, b, a) \
    color_make(r / 255.0, g / 255.0, b / 255.0, a / 255.0)

/**
 * Like color_rgb, but uses a range of 0-255 instead of 0-1
 */
#define color_from_bytes_rgb(r, g, b) color_from_bytes(r, g, b, 255)

/* Color constants */

#define COLOR_TRANSPARENT   color_from_bytes(  0,   0,   0,   0)
#define COLOR_BLACK         color_from_bytes(  0,   0,   0, 255)
#define COLOR_RED           color_from_bytes(255,   0,   0, 255)
#define COLOR_GREEN         color_from_bytes(  0, 255,   0, 255)
#define COLOR_BLUE          color_from_bytes(  0,   0, 255, 255)
#define COLOR_YELLOW        color_from_bytes(255, 255,   0, 255)
#define COLOR_PURPLE        color_from_bytes(255,   0, 255, 255)
#define COLOR_CYAN          color_from_bytes(  0, 255, 255, 255)
#define COLOR_WHITE         color_from_bytes(255, 255, 255, 255)

/**
 * Blends two colors together and stores the result in dest
 */
void color_blend(color_t *dest, color_t foreground, color_t background);
