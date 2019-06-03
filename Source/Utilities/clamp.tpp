#pragma once

using namespace Roguevania;
using namespace Roguevania::Utilities;

template <class T>
T clamp(T value, T min, T max) {
    if (value < min) value = min;
    if (value > max) value = max;
    return value;
}

template <class T>
void clamp(T* value, T min, T max) {
    *value = clamp(*value, min, max);
}