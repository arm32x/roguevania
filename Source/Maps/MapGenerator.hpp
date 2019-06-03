#pragma once

#include <cstdint>
#include <deque>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#include <randutils/randutils.hpp>
#pragma GCC diagnostic pop

#include "Room.hpp"

namespace Roguevania {
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
        /// Generate a single room.
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
        
        ////////////////////////////////////////////////////////////////////////
        /// Generates the internal layout of a room from a randomly-chosen
        /// template file.
        /// 
        /// @param room
        ///     The room to generate the internal layout for.
        /// @return
        ///     A pointer to a tilemap dynamically allocated using `new`.  This
        ///     pointer is also assigned to the `tilemap` member variable on the
        ///     room passed.
        ////////////////////////////////////////////////////////////////////////
        Tilemap* generateRoomLayout(Room& room);
        
        ////////////////////////////////////////////////////////////////////////
        /// Generates the internal layout of a room from a specified file.
        /// 
        /// @param room
        ///     The room to generate the internal layout for.
        /// @param filename
        ///     The template file to use for room generation.
        /// @return
        ///     A pointer to a tilemap dynamically allocated using `new`.  This
        ///     pointer is also assigned to the `tilemap` member variable on the
        ///     room passed.
        ////////////////////////////////////////////////////////////////////////
        Tilemap* generateRoomLayoutFromFile(Room& room, const char* filename);
        
        ////////////////////////////////////////////////////////////////////////
        /// Generates the internal layout of a room from data obtained from an
        /// input stream.
        /// 
        /// @param room
        ///     The room to generate the internal layout for.
        /// @param stream
        ///     The stream to read template data from.
        /// @return
        ///     A pointer to a tilemap dynamically allocated using `new`.  This
        ///     pointer is also assigned to the `tilemap` member variable on the
        ///     room passed.
        ////////////////////////////////////////////////////////////////////////
        Tilemap* generateRoomLayoutFromStream(Room& room, std::istream& stream);
        
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