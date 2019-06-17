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

namespace Roguevania {
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
        
        ////////////////////////////////////////////////////////////////////////
        /// Sets the position of an overlay indicator on the minimap.  Will
        /// create a new overlay if the requested overlay does not exist.
        /// 
        /// @param id
        ///     The identifier of the overlay to create or modify.
        /// @param x, y
        ///     The new position of the overlay on the minimap.
        ////////////////////////////////////////////////////////////////////////
        void setOverlayPosition(uint32_t id, uint16_t x, uint16_t y);
        
        ////////////////////////////////////////////////////////////////////////
        /// Sets the type of an overlay indicator on the minimap.  Will create a
        /// new overlay if the requested overlay does not exist.
        /// 
        /// @param id
        ///     The identifier of the overlay to create or modify.
        /// @param type
        ///     The new type of the overlay.
        ////////////////////////////////////////////////////////////////////////
        void setOverlayType(uint32_t id, uint8_t type);
        
        ////////////////////////////////////////////////////////////////////////
        /// Sets the color of an overlay indicator on the minimap.  Will create
        /// a new overlay if the requested overlay does not exist.
        /// 
        /// @param id
        ///     The identifier of the overlay to create or modify.
        /// @param color
        ///     The new color of the overlay.
        ////////////////////////////////////////////////////////////////////////
        void setOverlayColor(uint32_t id, const Color& color);
        
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