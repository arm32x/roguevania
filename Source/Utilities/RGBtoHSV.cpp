#include "RGBtoHSV.hpp"
#include <cmath>
using namespace Roguevania;
using namespace sf;

// Translated from JavaScript from a website.
// http://www.kourbatov.com/faq/rgb2hsv.htm
std::tuple<float, float, float> Utilities::RGBtoHSV(uint8_t R, uint8_t G, uint8_t B) {
    float r = R / 255.0f, g = G / 255.0f, b = B / 255.0f;
    
    float minRGB = std::min(r, std::min(g, b));
    float maxRGB = std::max(r, std::max(g, b));
    
    if (minRGB == maxRGB) return std::tuple<float, float, float>(0.0f, 0.0f, maxRGB);
    
    float d = r == minRGB ? g - b : b == minRGB ? r - g : b - r;
    float h = r == minRGB ? 3 : b == minRGB ? 1 : 5;
    
    float H = (60.0f * (h - d / (maxRGB - minRGB))) / 360.0f;
    float S = (maxRGB - minRGB) / maxRGB;
    float V = maxRGB;
    
    return std::tuple<float, float, float>(H, S, V);
}

std::tuple<float, float, float> Utilities::RGBtoHSV(const Color& color) {
    return Utilities::RGBtoHSV(color.r, color.g, color.b);
}