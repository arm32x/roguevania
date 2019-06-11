#pragma once

#include <array>
#include <cstdint>
#include <vector>

#include <SFML/System/Vector2.hpp>

#include "../Collision/CollisionMap.hpp"
#include "../Collision/CollisionMode.hpp"
#include "../Platform/Entity.hpp"

namespace Roguevania {

// Forward declarations.
namespace Maps {
    class Tilemap;
};

namespace Collision {
using namespace Roguevania::Maps;
using namespace Roguevania::Platform;
using namespace sf;

////////////////////////////////////////////////////////////////////////////////
/// Provides collision-detection logic for tilemaps by linking them with a
/// collision map.
////////////////////////////////////////////////////////////////////////////////
// TODO:  Integrate this into Tilemap somehow.
class TilemapCollider {
        
    public:
        ////////////////////////////////////////////////////////////////////////
        /// Create a new tilemap collider and adds it to the `all` vector.
        /// 
        /// @param tilemap
        ///     The tilemap that will be linked to the new tilemap collider.
        /// @param collisionMap
        ///     The collision map that will be linked to the new tilemap
        ///     collider.
        ////////////////////////////////////////////////////////////////////////
        TilemapCollider(const Tilemap& tilemap, const CollisionMap& collisionMap);
        
        ////////////////////////////////////////////////////////////////////////
        /// Destructs a tilemap collider and removes it from the `all` vector.
        ////////////////////////////////////////////////////////////////////////
        ~TilemapCollider();
        
        ////////////////////////////////////////////////////////////////////////
        /// The tilemap that this tilemap collider is linked to.
        ////////////////////////////////////////////////////////////////////////
        const Tilemap& tilemap;
        
        ////////////////////////////////////////////////////////////////////////
        /// The collision map that this tilemap collider is linked to.
        ////////////////////////////////////////////////////////////////////////
        const CollisionMap& collisionMap;
        
        ////////////////////////////////////////////////////////////////////////
        /// Get the collision mode of the given tile on the tilemap.
        /// 
        /// @param x, y
        ///     The position of the tile on the tilemap to get the collision
        ///     mode of.
        /// @return
        ///     The collision mode of the requested title.
        ////////////////////////////////////////////////////////////////////////
        CollisionMode getTileCollisionMode(uint16_t x, uint16_t y) const;
        
        ////////////////////////////////////////////////////////////////////////
        /// Get all of the tiles in the linked tilemap that are touching the
        /// given entity.
        /// 
        /// @param entity
        ///     The entity that the returned tiles will be touching.
        /// @return
        ///     A vector of positions of the tiles in the tilemap that are
        ///     touching the given entity.
        ////////////////////////////////////////////////////////////////////////
        std::vector<Vector2<uint16_t>> getTilesTouching(const Entity& entity) const;
        
        ////////////////////////////////////////////////////////////////////////
        /// Gets the highest-priority collision mode of the given tiles.
        /// 
        /// @param tiles
        ///     The positions of the tiles to get the collision mode from.
        /// @return
        ///     The highest-priority collision mode of all of the given tiles.
        ////////////////////////////////////////////////////////////////////////
        CollisionMode prioritizeTileModes(const std::vector<Vector2<uint16_t>>& tiles) const;
        
        ////////////////////////////////////////////////////////////////////////
        /// A vector of all tilemap colliders that currently exist.
        ////////////////////////////////////////////////////////////////////////
        static std::vector<TilemapCollider*> all;
        
};

};
};