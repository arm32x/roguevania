#pragma once

#include <cstdint>
#include <ostream>
#include <vector>

namespace Roguevania {
namespace Collision {

////////////////////////////////////////////////////////////////////////////////
/// A collision mode determines how game objects should interact with each
/// other.  This enum defines all collision modes used in the game.
/// 
/// Collision modes are implicitly prioritized by increasing value, except for
/// values in the range 0xF0 to 0xFF, which have the lowest priority.
////////////////////////////////////////////////////////////////////////////////
// TODO: Add documentation for members.
class CollisionMode {
    
    public:
        enum Value : uint8_t {
            Undefined   = 0xFF,
            NotTouching = 0xF0,
            None        = 0x00,
            Bullet      = 0x10,
            SlopeTL     = 0x90,
            SlopeTR     = 0x91,
            SlopeBL     = 0x92,
            SlopeBR     = 0x93,
            SemiSolid   = 0xA0,
            Solid       = 0xB0,
            Damaging    = 0xD0,
        };
        
        CollisionMode();
        CollisionMode(const CollisionMode::Value& value);
        
        bool operator==(const CollisionMode::Value& other) const;
        bool operator==(const CollisionMode&        other) const;
        bool operator!=(const CollisionMode::Value& other) const;
        bool operator!=(const CollisionMode&        other) const;
        
        static CollisionMode prioritize(const std::vector<CollisionMode>& modes);
        
        operator uint8_t() const;
        
    private:
        CollisionMode::Value value;
        
};

};
};

// This is equivalent to a ‘using’ declaration, except I can undefine it later.
#define CollisionMode Roguevania::Collision::CollisionMode 

std::ostream& operator<<(std::ostream& stream, const CollisionMode&        mode);
std::ostream& operator<<(std::ostream& stream, const CollisionMode::Value& mode);

#undef CollisionMode