#pragma once

#include "../Collision/CollisionMode.hpp"
#include "../Platform/Entity.hpp"

namespace Roguevania {
namespace Platform {
using namespace Roguevania::Collision;

////////////////////////////////////////////////////////////////////////////////
/// Represents an enemy in the game.
////////////////////////////////////////////////////////////////////////////////
class Enemy : public Entity {
        
    public:
        using Entity::Entity;
        
        virtual void collide(CollisionMode mode) override;
        
};

};
};