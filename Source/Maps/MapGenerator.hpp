#pragma once

#include <cstdint>
#include <deque>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#include <randutils/randutils.hpp>
#pragma GCC diagnostic pop

#include "Room.hpp"

namespace RoguelikeMetroidvania {
namespace Maps {

////////////////////////////////////////////////////////////////////////////////
/// The map generator used to generate maps.
////////////////////////////////////////////////////////////////////////////////
class MapGenerator {
    
    public:
        ////////////////////////////////////////////////////////////////////////
        /// Constructs a map generator with a specific seed.
        ////////////////////////////////////////////////////////////////////////
        MapGenerator(uint64_t seed);
        
        ////////////////////////////////////////////////////////////////////////
        /// Constructs a map generator with a randomly-generated seed.
        ////////////////////////////////////////////////////////////////////////
        MapGenerator();
        
        ////////////////////////////////////////////////////////////////////////
        /// Sets the seed to be used for level generation.  Using the same seed
        /// should produce the same level.
        /// 
        /// @param seed
        ///     The seed to give to the random number generator.
        ////////////////////////////////////////////////////////////////////////
        void setSeed(uint64_t seed);
        
        ////////////////////////////////////////////////////////////////////////
        /// Generate the type of a single room.  This is an 8-bit mask with
        /// information such as which sides have exits.  See the table below for
        /// details for specific bits.
        /// 
        /// | Bit | Description                                                |
        /// |:---:|:---------------------------------------------------------- |
        /// | 0-1 | The direction in which this room extends.                  |
        /// | 2-3 | The length of this room’s extension.                       |
        /// |  4  | Set if the top side of a room has a door.                  |
        /// |  5  | Set if the right side of a room has a door.                |
        /// |  6  | Set if the bottom side of a room has a door.               |
        /// |  7  | Set if the left side of a room has a door.                 |
        ////////////////////////////////////////////////////////////////////////
        Room generateRoom();
        
        ////////////////////////////////////////////////////////////////////////
        /// Generates a deque of rooms based on the given width and height.
        /// This is arranged like a Z — from left to right, top to bottom.
        /// 
        /// @param width, height
        ///     The size of the map, measured in rooms.
        /// @return
        ///     The generated rooms.
        ////////////////////////////////////////////////////////////////////////
        std::deque<Room> generateRooms(uint16_t width, uint16_t height);
        
        ////////////////////////////////////////////////////////////////////////
        /// Generates sections based on the given rooms.
        /// 
        /// @param rooms
        ///     The rooms to process into sections.  The rooms in this deque
        ///     will be updated with section information.
        /// @param width, height
        ///     The size of the map, measured in rooms.
        /// @return
        ///     The generated sections.
        ////////////////////////////////////////////////////////////////////////
        std::deque<Section> generateSections(std::deque<Room>& rooms, uint16_t width, uint16_t height, uint32_t minimumSectionSize = 32);
        
        ////////////////////////////////////////////////////////////////////////
        /// Gets the random number generator used by this map generator.  Useful
        /// for determining other random aspects of the level.
        /// 
        /// @return
        ///     The random number generator that this level generator uses.
        ////////////////////////////////////////////////////////////////////////
        randutils::mt19937_rng& getRandom();
        
    private:
        ////////////////////////////////////////////////////////////////////////
        /// Processes a deque of rooms by doing a few things:
        ///   - Ensure that no rooms are connected to the edge of the map.
        ///   - Sometimes, connect rooms where only one room has a connection.
        ///   - Disconnect rooms where only one room has a connection.
        /// 
        /// This function is called automatically by ‘generateRooms’.
        /// 
        /// @param rooms
        ///     A deque of rooms, given in the format returned by
        ///     ‘generateRooms’.  This deque will be updated with the result.
        /// @param width, height
        ///     The size of the map, measured in rooms.
        ////////////////////////////////////////////////////////////////////////
       void connectRooms(std::deque<Room>& rooms, uint16_t width, uint16_t height);
        
        ////////////////////////////////////////////////////////////////////////
        /// The random number generator to use for generating this map.
        ////////////////////////////////////////////////////////////////////////
        randutils::mt19937_rng random;
    
};

};
};