#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "../Collision/CollisionMode.hpp"
#include "../Platform/Entity.hpp"

namespace Roguevania {
namespace Platform {
using namespace Roguevania::Collision;
using namespace sf;

////////////////////////////////////////////////////////////////////////////////
/// Represents an enemy in the game.
////////////////////////////////////////////////////////////////////////////////
class Enemy : public Entity {
        
    public:
        Enemy(const Texture& texture, const IntRect& rectangle);
        Enemy(const Texture& texture);
        
        virtual void collide(CollisionMode mode) override;
        
};

};
};