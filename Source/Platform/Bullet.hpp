#pragma once

#include <cstdint>
#include <vector>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>

#include "../Platform/Entity.hpp"

namespace Roguevania {
namespace Platform {
using namespace sf;

// TODO:  Add documentation.
class Bullet : public Entity {
        
    public:
        Bullet(const Texture& texture, const IntRect& rectangle, float x, float y, float velocityX, float velocityY);
        Bullet(const Texture& texture, const IntRect& rectangle, const Vector2f& position, const Vector2f& velocity);
        
        virtual void update(float delta) override;
        
        static std::vector<Bullet> bullets;
        
};

};
};