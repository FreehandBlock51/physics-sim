#include "viewer/color.h"

// taken and modified from stackoverflow: https://stackoverflow.com/questions/726549/algorithm-for-additive-color-mixing-for-rgb-values
void color_blend(color_t *dest, color_t foreground, color_t background) {
    dest->a = 1 - (1 - foreground.a) * (1 - background.a);
    if (dest->a < PHYSICS_EPSILON) {
        return; // Fully transparent -- r,g,b not important
    }
    dest->r = foreground.r * foreground.a / dest->a + background.r * background.a * (1 - foreground.a) / dest->a;
    dest->g = foreground.g * foreground.a / dest->a + background.g * background.a * (1 - foreground.a) / dest->a;
    dest->b = foreground.b * foreground.a / dest->a + background.b * background.a * (1 - foreground.a) / dest->a;
}
