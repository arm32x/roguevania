#pragma once

#include <cstdint>

#include <SFML/Graphics/View.hpp>
#include <SFML/System/Vector2.hpp>

namespace Roguevania {
namespace Environment {
using namespace sf;

////////////////////////////////////////////////////////////////////////////////
/// A class used to manipulate SFML views.
////////////////////////////////////////////////////////////////////////////////
class Camera {
        
    public:
        Camera();
        
        int32_t focusZoneXLeft    = 320 - 32;
        int32_t focusZoneXRight   = 320 + 32;
        
        int32_t focusZoneYTop     = 180 - 8;
        int32_t focusZoneYBottom  = 180 + 8;
        
        int32_t panicZoneYTop     = 180 - 64;
        int32_t panicZoneYBottom  = 180 + 64;
        
        void update(float delta, Vector2f focalPoint);
        
        View view;
        
};

};
};