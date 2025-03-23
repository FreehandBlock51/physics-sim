#include "common/math.h"


phy_real_t clamp(phy_real_t value, phy_real_t min, phy_real_t max) {
    if (value < min) {
        return min;
    }
    if (value > max) {
        return max;
    }
    return value;
}

phy_real_t min(phy_real_t a, phy_real_t b) {
    if (a < b) {
        return a;
    }
    return b;
}

phy_real_t max(phy_real_t a, phy_real_t b) {
    if (a > b) {
        return a;
    }
    return b;
}

phy_real_t mod(phy_real_t src, phy_real_t factor) {
    phy_real_t quotient = src / factor;
    phy_real_t quotient_ipart;
    phy_real_t quotient_fpart = modff(quotient, &quotient_ipart);
    return quotient_fpart * factor;
}
