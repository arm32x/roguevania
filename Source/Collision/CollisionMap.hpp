#pragma once

#include <array>
#include <cstdint>
#include <string>

#include <SFML/System/InputStream.hpp>

#include "../Collision/CollisionMode.hpp"

namespace Roguevania {
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
        /// Load 256 collision modes from the given file.
        /// 
        /// @param filename
        ///     The name of the file to read the collision modes from.  The file
        ///     should be in binary format with each mode taking one byte.
        /// @return
        ///     True if the file could be opened and the data could be read.
        ////////////////////////////////////////////////////////////////////////
        void loadFromFile(const char* filename);
        
        ////////////////////////////////////////////////////////////////////////
        /// Load 256 collision modes from the given input stream.
        /// 
        /// @param stream
        ///     The input stream to read the collision modes from.  Should be
        ///     reading in binary format (no newline processing), with each mode
        ///     taking one byte.
        /// @return
        ///     True if the data could be read from the input stream.
        ////////////////////////////////////////////////////////////////////////
        void loadFromStream(std::istream& stream);
        
        ////////////////////////////////////////////////////////////////////////
        /// Load 256 collision modes from the given memory location and store
        /// them in `buffer`.  Does no sanity-checks.
        /// 
        /// @return
        ///     True, regardless of whether or not the operation succeeded.
        ////////////////////////////////////////////////////////////////////////
        void loadFromMemory(const uint8_t* data);
        
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