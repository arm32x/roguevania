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
// TODO:  Add documentation for members.
class SineWaveEnemy final : public Enemy {
        
    public:
        enum Direction {
            Left, Right
        };
        
        SineWaveEnemy(const Texture& texture, const IntRect& rectangle, float x, float y, const Vector2f& speed = Vector2f(32.0f, 8.0f), const Vector2f& distance = Vector2f(64.0f, 4.0f));
        SineWaveEnemy(const Texture& texture, const IntRect& rectangle, const Vector2f& position, const Vector2f& speed = Vector2f(32.0f, 8.0f), const Vector2f& distance = Vector2f(64.0f, 4.0f));
        SineWaveEnemy(const Texture& texture, float x, float y, const Vector2f& speed = Vector2f(32.0f, 8.0f), const Vector2f& distance = Vector2f(64.0f, 4.0f));
        SineWaveEnemy(const Texture& texture, const Vector2f& position, const Vector2f& speed = Vector2f(32.0f, 8.0f), const Vector2f& distance = Vector2f(64.0f, 4.0f));
        
        virtual void update(float delta) override;
        
    private:
        Vector2f base;
        float cycle = 0.0f;
        Vector2f speed, distance;
        
};

};
};