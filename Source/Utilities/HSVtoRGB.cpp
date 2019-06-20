#include "HSVtoRGB.hpp"
#include <cmath>
using namespace Roguevania;
using namespace sf;

// Translated from JavaScript from a StackOverflow answer.
// https://stackoverflow.com/a/8208967/3410752
Color Utilities::HSVtoRGB(float h, float s, float v) {
    float r, g, b;
    
    h = std::fmod(h, 1.0f);
    long  i = std::floor(h * 6.0f);
    float f = h * 6.0f - i;
    float p = v * (1 - s);
    float q = v * (1 - f * s);
    float t = v * (1 - (1 - f) * s);
    
    switch (i % 6) {
        case 0: r = v, g = t, b = p; break;
        case 1: r = q, g = v, b = p; break;
        case 2: r = p, g = v, b = t; break;
        case 3: r = p, g = q, b = v; break;
        case 4: r = t, g = p, b = v; break;
        case 5: r = v, g = p, b = q; break;
    }
    
    return Color(r * 255, g * 255, b * 255);
}