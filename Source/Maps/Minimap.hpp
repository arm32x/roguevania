#pragma once

#include <cstdint>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/View.hpp>

#include "Tilemap.hpp"

namespace RoguelikeMetroidvania {
namespace Maps {
using namespace sf;

////////////////////////////////////////////////////////////////////////////////
/// A minimap shows the layout of the rooms in a map without showing the rooms’
/// contents.  This is implemented as a subclass of `Tilemap` with additional
/// view functionality.
////////////////////////////////////////////////////////////////////////////////
class Minimap : public Tilemap {
        
    public:
        ////////////////////////////////////////////////////////////////////////
        /// Creates a minimap with the given size.
        /// 
        /// @param width, height
        ///     The size of the map that this minimap represents, in rooms.
        ////////////////////////////////////////////////////////////////////////
        Minimap(uint16_t width, uint16_t height);
        
    protected:
        ////////////////////////////////////////////////////////////////////////
        /// Draws the minimap to the given render target.
        ////////////////////////////////////////////////////////////////////////
        virtual void draw(RenderTarget& target, RenderStates states) const override;
        
        ////////////////////////////////////////////////////////////////////////
        /// The view that this minimap uses to render itself.
        ////////////////////////////////////////////////////////////////////////
        View view;
        
};

};
};