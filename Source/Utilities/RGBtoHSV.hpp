#pragma once

#include <cstdint>
#include <tuple>

#include <SFML/Graphics/Color.hpp>

namespace Roguevania {
namespace Utilities {
using namespace sf;
        
        ////////////////////////////////////////////////////////////////////////
        /// Converts an RGB color to an HSV color.
        /// 
        /// @param r, g, b
        ///     The RGB color to convert.
        /// @return
        ///     A tuple with the HSV values (between 0.0 and 1.0).
        ////////////////////////////////////////////////////////////////////////
        std::tuple<float, float, float> RGBtoHSV(uint8_t r, uint8_t g, uint8_t b);
        
        ////////////////////////////////////////////////////////////////////////
        /// Converts an RGB color to an HSV color.
        /// 
        /// @param color
        ///     The RGB color to convert.
        /// @return
        ///     A tuple with the HSV values (between 0.0 and 1.0).
        ////////////////////////////////////////////////////////////////////////
        std::tuple<float, float, float> RGBtoHSV(const Color& color);
        
};

};