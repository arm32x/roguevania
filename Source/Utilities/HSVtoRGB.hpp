#pragma once

#include <SFML/Graphics/Color.hpp>

namespace Roguevania {
namespace Utilities {
using namespace sf;
        
        ////////////////////////////////////////////////////////////////////////
        /// Converts an HSV color to an RGB color.
        /// 
        /// @param h, s, v
        ///     The HSV values to convert to RGB.  All are floating-point
        ///     numbers from 0.0 to 1.0.
        /// @return
        ///     An SFML color representing the RGB output.
        ////////////////////////////////////////////////////////////////////////
        Color HSVtoRGB(float h, float s, float v);
        
};

};