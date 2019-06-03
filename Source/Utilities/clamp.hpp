#pragma once

namespace Roguevania {
namespace Utilities {
        
        ////////////////////////////////////////////////////////////////////////
        /// Clamp a value to the given minimum and maximum.
        ///   - If value < minimum, it will be set to minimum.
        ///   - If value > maximum, it will be set to maximum.
        /// The value will then be returned.
        ////////////////////////////////////////////////////////////////////////
        template <class T> T clamp(T value, T min, T max);
        
        ////////////////////////////////////////////////////////////////////////
        /// Clamp a value to the given minimum and maximum.
        ///   - If value < minimum, it will be set to minimum.
        ///   - If value > maximum, it will be set to maximum.
        /// The value will then overwrite the previous value.
        ////////////////////////////////////////////////////////////////////////
        template <class T> void clamp(T* value, T min, T max);
        
#include "clamp.tpp"
        
};
};