#pragma once

#include <array>
#include <cstdint>
#include <string>

#include <SFML/System/InputStream.hpp>

#include "../Collision/CollisionMode.hpp"

namespace RoguelikeMetroidvania {
namespace Collision {
using namespace sf;

////////////////////////////////////////////////////////////////////////////////
/// Represents the collision data for the tiles in a tilemap spritesheet.  
/// Collision-detection logic for tilemaps is handled in the `TilemapCollider`
/// class.
////////////////////////////////////////////////////////////////////////////////
class CollisionMap {
        
    public:
        
        ////////////////////////////////////////////////////////////////////////
        /// Load 256 collision modes from the given file.  The file must be in
        /// binary format with each collision mode occupying one byte.
        /// 
        /// @return
        ///     True if the file could be opened and the data could be read.
        ////////////////////////////////////////////////////////////////////////
        bool loadFromFile(const std::string& filename);
        
        ////////////////////////////////////////////////////////////////////////
        /// Load 256 collision modes from the given input stream.  The data must
        /// be in binary format with each collision mode occupying one byte.
        /// 
        /// @return
        ///     True if the data could be read.
        ////////////////////////////////////////////////////////////////////////
        bool loadFromStream(InputStream& stream);
        
        ////////////////////////////////////////////////////////////////////////
        /// Load 256 collision modes from the given memory location and store
        /// them in `buffer`.  Does no sanity-checks.
        /// 
        /// @return
        ///     True, regardless of whether or not the operation succeeded.
        ////////////////////////////////////////////////////////////////////////
        bool loadFromMemory(const void* data);
        
        ////////////////////////////////////////////////////////////////////////
        /// Get a collision mode by index, typically a tile type.
        ////////////////////////////////////////////////////////////////////////
        CollisionMode operator[](uint8_t index) const;
        
        ////////////////////////////////////////////////////////////////////////
        /// The buffer that stores the data for this collision map.
        ////////////////////////////////////////////////////////////////////////
        std::array<CollisionMode, 256> buffer;
        
};

};
};