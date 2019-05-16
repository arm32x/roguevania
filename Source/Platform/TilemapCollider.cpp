#include "TilemapCollider.hpp"

using namespace RoguelikeMetroidvania;
using namespace RoguelikeMetroidvania::Platform;

TilemapCollider::TilemapCollider(const Tilemap& tilemap, const CollisionMap& collisionMap)
    : tilemap(tilemap), collisionMap(collisionMap) {
    
}

CollisionMap::Mode TilemapCollider::getTileMode(uint16_t x, uint16_t y) const {
    return collisionMap[tilemap.getTileType(x, y)];
}