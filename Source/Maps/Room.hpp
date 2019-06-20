#pragma once

#include <cstdint>
#include <deque>

#include <stx/optional.hpp>

#include <SFML/Graphics/Color.hpp>

#include "../Maps/Tilemap.hpp"

namespace Roguevania {
namespace Maps {
using namespace sf;
using stx::nullopt;

// Forward declarations.
class Section;
class Tilemap;

////////////////////////////////////////////////////////////////////////////////
/// Represents a room on the map.
////////////////////////////////////////////////////////////////////////////////
class Room final {
        
    public:
        ////////////////////////////////////////////////////////////////////////
        /// Creates a room with a specified layout inside a section.
        /// 
        /// @param doors, openings
        ///     The layout of the room.  Determines which sides of the room have
        ///     doors and which sides of the room are open to other rooms.
        /// @param section
        ///     The section that this room is in.
        ////////////////////////////////////////////////////////////////////////
        Room(Section* section, uint8_t doors, uint8_t openings);
        
        ////////////////////////////////////////////////////////////////////////
        /// Creates a room with a specified layout that is not in a section.
        /// 
        /// @param doors, openings
        ///     The layout of the room.  Determines which sides of the room have
        ///     doors and which sides of the room are open to other rooms.
        ////////////////////////////////////////////////////////////////////////
        Room(uint8_t doors, uint8_t openings);
        
        ////////////////////////////////////////////////////////////////////////
        /// Creates a room with a specified layout inside a section.
        /// 
        /// @param combined
        ///     The layout of the room.  Determines which sides of the room have
        ///     doors and which sides of the room are open to other rooms.  The
        ///     upper four bits represent the openings and the lower four bits
        ///     represent the doors.
        /// @param section
        ///     The section that this room is in.
        ////////////////////////////////////////////////////////////////////////
        Room(Section* section, uint8_t combined);
        
        ////////////////////////////////////////////////////////////////////////
        /// Creates a room with a specified layout that is not in a section.
        /// 
        /// @param combined
        ///     The layout of the room.  Determines which sides of the room have
        ///     doors and which sides of the room are open to other rooms.  The
        ///     upper four bits represent the openings and the lower four bits
        ///     represent the doors.
        ////////////////////////////////////////////////////////////////////////
        Room(uint8_t combined);
        
        ////////////////////////////////////////////////////////////////////////
        /// Creates a room with the default layout inside a section.
        /// 
        /// @param section
        ///     The section that this room is in.
        ////////////////////////////////////////////////////////////////////////
        Room(Section* section);
        
        ////////////////////////////////////////////////////////////////////////
        /// Creates a room with the default layout that is not in a section.
        ////////////////////////////////////////////////////////////////////////
        Room();
        
        uint8_t  doors    : 4;      ///< Determines which sides of the room have a door.
        uint8_t  openings : 4;      ///< Determines which sides of the room are open (no wall).
        Section* section;           ///< A pointer to the section that this room is in.
        uint16_t x;                 ///< The X position of the room within the map.
        uint16_t y;                 ///< The Y position of the room within the map.
        
        stx::optional<Tilemap> tilemap = nullopt; ///< A tilemap representing the layout of this room.
        std::vector<Entity*> entities;            ///< A vector of all entities that are contained in this room.
        
};

};
};