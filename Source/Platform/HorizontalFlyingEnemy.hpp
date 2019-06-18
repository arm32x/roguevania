#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>

#include "../Platform/Enemy.hpp"

namespace Roguevania {
namespace Platform {
using namespace sf;

////////////////////////////////////////////////////////////////////////////////
/// A horizontal flying enemy flies left and right.
/// It flies vertically in a short sine-wave shape.
////////////////////////////////////////////////////////////////////////////////
class HorizontalFlyingEnemy final : public Enemy {
        
    public:
        enum Direction {
            Left, Right
        };
        
        HorizontalFlyingEnemy(const Texture& texture, const IntRect& rectangle, float x, float y);
        HorizontalFlyingEnemy(const Texture& texture, const IntRect& rectangle, const Vector2f& position);
        HorizontalFlyingEnemy(const Texture& texture, float x, float y);
        HorizontalFlyingEnemy(const Texture& texture, const Vector2f& position);
        
        virtual void update(float delta) override;
        
    private:
        Vector2f base;
        float cycle = 0.0f;
        
};

};
};