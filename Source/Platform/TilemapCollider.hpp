#pragma once

#include <array>
#include <cstdint>

#include "../Maps/CollisionMap.hpp"
#include "../Maps/Tilemap.hpp"

namespace RoguelikeMetroidvania {
namespace Platform {
using namespace RoguelikeMetroidvania::Maps;

class TilemapCollider {
        
    public:
        TilemapCollider(const Tilemap& tilemap, const CollisionMap& collisionMap);
        
        const Tilemap& tilemap;
        const CollisionMap& collisionMap;
        
        CollisionMap::Mode getTileMode(uint16_t x, uint16_t y) const;
        
};

};
};