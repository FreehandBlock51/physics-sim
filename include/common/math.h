#pragma once
/**
 * Extends the built-in math.h with other useful functions
 */

#include <math.h>
#include "common/defines.h"

/**
 * clamps val so that it is greater than or equal to low and
 * less than or equal to high
 */
phy_real_t clamp(phy_real_t value, phy_real_t min, phy_real_t max);

/**
 * @brief Finds the minimum of two values
 */
phy_real_t min(phy_real_t a, phy_real_t b);

/**
 * @brief Finds the maximum of two values
 */
phy_real_t max(phy_real_t a, phy_real_t b);

/**
 * @brief Calculates src % factor using floating point values
 * @param src the dividend
 * @param factor the divisor
 * @return the remainder
 */
phy_real_t mod(phy_real_t src, phy_real_t factor);