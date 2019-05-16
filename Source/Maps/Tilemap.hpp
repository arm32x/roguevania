#pragma once

#include <cstdint>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/VertexArray.hpp>

namespace RoguelikeMetroidvania {
namespace Maps {
using namespace sf;

////////////////////////////////////////////////////////////////////////////////
/// A tilemap is a grid filled with tiles from a given texture.  Implemented as
/// a `VertexArray` of quads representing each tile.
////////////////////////////////////////////////////////////////////////////////
class Tilemap : public Drawable, public Transformable {
        
    public:
        ////////////////////////////////////////////////////////////////////////
        /// Creates a tilemap with the given size and tile size.
        /// 
        /// @param width, height
        ///     The width and height of the tilemap, measured in tiles.
        /// @param tileSize
        ///     The width and height of the square tiles in this tilemap.
        ////////////////////////////////////////////////////////////////////////
        Tilemap(uint16_t width, uint16_t height, uint32_t tileSize);
        
        ////////////////////////////////////////////////////////////////////////
        /// Creates a tilemap with the given size and tile size.
        /// 
        /// @param width, height
        ///     The width and height of the tilemap, measured in tiles.
        /// @param tileSize
        ///     The width and height of the square tiles in this tilemap.
        /// @param graphicsFilename
        ///     The filename of the texture used for graphics in this tilemap.
        ////////////////////////////////////////////////////////////////////////
        Tilemap(uint16_t width, uint16_t height, uint32_t tileSize, const char* graphicsFilename);
        
        ////////////////////////////////////////////////////////////////////////
        /// Creates a tilemap with the given size.
        /// 
        /// @param width, height
        ///     The width and height of the tilemap, measured in tiles.
        ////////////////////////////////////////////////////////////////////////
        Tilemap(uint16_t width, uint16_t height);
        
        ////////////////////////////////////////////////////////////////////////
        /// Creates a tilemap with the given size with a texture loaded from the
        /// given filename.
        /// 
        /// @param width, height
        ///     The width and height of the tilemap, measured in tiles.
        /// @param graphicsFilename
        ///     The filename of the texture used for graphics in this tilemap.
        ////////////////////////////////////////////////////////////////////////
        Tilemap(uint16_t width, uint16_t height, const char* graphicsFilename);
        
        ////////////////////////////////////////////////////////////////////////
        /// Get the type of a tile on the map.
        /// 
        /// @param x, y
        ///     The position of the tile to get the type of.
        /// @return
        ///     The type of the tile.  Only looks at the texture coordinates of
        ///     the top-left vertex.
        ////////////////////////////////////////////////////////////////////////
        uint8_t getTileType(uint16_t x, uint16_t y) const;
        
        ////////////////////////////////////////////////////////////////////////
        /// Set the tile type for a tile on the map.
        /// 
        /// @param x, y
        ///     The position of the tile to set.
        /// @param type
        ///     The new type of the tile.  This determines the texture used.
        ////////////////////////////////////////////////////////////////////////
        void setTileType(uint16_t x, uint16_t y, uint8_t type);
        
        ////////////////////////////////////////////////////////////////////////
        /// Get the color of a tile on the map.  Only takes into account the
        /// top-left corner and ignores the other three corners.
        /// 
        /// @param x, y
        ///     The position of the tile to get the color of.
        /// @return
        ///     The color of the top-left corner of the tile.
        ////////////////////////////////////////////////////////////////////////
        const Color& getTileColor(uint16_t x, uint16_t y) const;
        
        ////////////////////////////////////////////////////////////////////////
        /// Set the color of a tile on the map.  Sets the color of all corners
        /// of the quad.
        /// 
        /// @param x, y
        ///     The position of the tile to set the color of.
        /// @param color
        ///     The new color of the tile.
        ////////////////////////////////////////////////////////////////////////
        void setTileColor(uint16_t x, uint16_t y, const Color& color);
        
        ////////////////////////////////////////////////////////////////////////
        /// The width of this tilemap, measured in tiles.
        ////////////////////////////////////////////////////////////////////////
        const uint16_t width;
        
        ////////////////////////////////////////////////////////////////////////
        /// The height of this tilemap, measured in tiles.
        ////////////////////////////////////////////////////////////////////////
        const uint16_t height;
        
        ////////////////////////////////////////////////////////////////////////
        /// The width and height of the square tiles in this tilemap.
        ////////////////////////////////////////////////////////////////////////
        const uint32_t tileSize;
        
    protected:
        ////////////////////////////////////////////////////////////////////////
        /// Draw this tilemap to the given render target.
        ////////////////////////////////////////////////////////////////////////
        virtual void draw(RenderTarget& target, RenderStates states) const override;
        
        ////////////////////////////////////////////////////////////////////////
        /// The `VertexArray` that stores the quads representing this tilemap.
        ////////////////////////////////////////////////////////////////////////
        VertexArray vertices;
        
        ////////////////////////////////////////////////////////////////////////
        /// A texture that contains the graphics data for this tilemap. It is
        /// expected to be square with a total of 256 tiles.
        ////////////////////////////////////////////////////////////////////////
        Texture graphics;
        
};

};
};
