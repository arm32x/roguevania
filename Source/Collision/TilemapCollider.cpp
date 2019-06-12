#include "TilemapCollider.hpp"

#include <algorithm>
#include <cmath>

#include "../Maps/Tilemap.hpp"
#include "../Platform/Entity.hpp"
#include "../Utilities/clamp.hpp"
#include "../Program.hpp"

using namespace Roguevania;
using namespace Roguevania::Collision;
using namespace Roguevania::Maps;
using namespace Roguevania::Platform;
using namespace sf;

std::vector<TilemapCollider*> TilemapCollider::all;

TilemapCollider::TilemapCollider(const Tilemap& tilemap, const CollisionMap& collisionMap)
    : tilemap(tilemap), collisionMap(collisionMap) {
    all.push_back(this);
}

TilemapCollider::~TilemapCollider() {
    all.erase(std::remove(all.begin(), all.end(), this));
}

CollisionMode TilemapCollider::getTileCollisionMode(uint16_t x, uint16_t y) const {
    if (x >= tilemap.width || y >= tilemap.height) return CollisionMode::NotTouching;
    return collisionMap[tilemap.getTileType(x, y)];
}

std::vector<Vector2<uint16_t>> TilemapCollider::getTilesTouching(const Entity& entity) const {
    int32_t left   = std::floor((std::round(entity.getPosition().x) - tilemap.getPosition().x) / tilemap.tileSize);
    int32_t top    = std::floor((std::round(entity.getPosition().y) - tilemap.getPosition().y) / tilemap.tileSize);
    int32_t right  = std::ceil(((std::round(entity.getPosition().x) - tilemap.getPosition().x) + entity.getTextureRect().width)  / tilemap.tileSize);
    int32_t bottom = std::ceil(((std::round(entity.getPosition().y) - tilemap.getPosition().y) + entity.getTextureRect().height) / tilemap.tileSize);
    Utilities::clamp(&left,   0, +tilemap.width);
    Utilities::clamp(&top,    0, +tilemap.height);
    Utilities::clamp(&right,  0, +tilemap.width);
    Utilities::clamp(&bottom, 0, +tilemap.height);
    if (Keyboard::isKeyPressed(Keyboard::Tilde)) Program::log(Log::Debug, "TilemapCollider") << "(" << left << ", " << top << ") / (" << right << ", " << bottom << ")" << std::endl;
    std::vector<Vector2<uint16_t>> result;
    for (int32_t y = top; y < bottom; y++) {
        for (int32_t x = left; x < right; x++) {
            result.emplace_back(x, y);
        }
    }
    return result;
}

CollisionMode TilemapCollider::prioritizeTileModes(const std::vector<Vector2<uint16_t>>& tiles) const {
    std::vector<CollisionMode> modes;
    for (const Vector2<uint16_t>& tile : tiles) {
        modes.push_back(getTileCollisionMode(tile.x, tile.y));
    }
    return CollisionMode::prioritize(modes);
}