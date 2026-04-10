#include "Math.h"

namespace fw {
float Lerp(float a, float b, float t) {
    return a + (b - a) * t;
}
}
