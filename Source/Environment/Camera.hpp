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
        
        int32_t focusZoneXLeft;
        int32_t focusZoneXRight;
        
        int32_t focusZoneYTop;
        int32_t focusZoneYBottom;
        
        int32_t panicZoneYTop;
        int32_t panicZoneYBottom;
        
        void update(float delta, Vector2f focalPoint);
        
        View view;
        
};

};
};