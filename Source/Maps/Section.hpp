#pragma once

#include <cstdint>
#include <deque>
#include <unordered_map>

#include <SFML/Graphics/Color.hpp>

namespace RoguelikeMetroidvania {
namespace Maps {
using namespace sf;

// Forward declarations.
class Room;

////////////////////////////////////////////////////////////////////////////////
/// Represents a colored section of the map that contains rooms.
////////////////////////////////////////////////////////////////////////////////
class Section {
        
    public:
        ////////////////////////////////////////////////////////////////////////
        /// Constructs a section.
        /// 
        /// @param id
        ///     The single-character identifier for this section.
        /// @param color
        ///     The color of this section.
        ////////////////////////////////////////////////////////////////////////
        Section(char id, Color color);
        
        char id;
        Color color; ///< The color of this section.
        std::deque<Room*> rooms; ///< The rooms that this section contains.
        
};

};
};