#include "HorizontalFlyingEnemy.hpp"

#include <cmath>

using namespace Roguevania;
using namespace Roguevania::Platform;
using namespace sf;

HorizontalFlyingEnemy::HorizontalFlyingEnemy(const Texture& texture, const IntRect& rectangle, float x, float y)
    : Enemy(texture, rectangle), base(x, y) {
    setPosition(x, y);
}

HorizontalFlyingEnemy::HorizontalFlyingEnemy(const Texture& texture, const IntRect& rectangle, const Vector2f& position)
    : HorizontalFlyingEnemy(texture, rectangle, position.x, position.y) {

}

HorizontalFlyingEnemy::HorizontalFlyingEnemy(const Texture& texture, float x, float y)
    : Enemy(texture), base(x, y) {
    setPosition(x, y);
}

HorizontalFlyingEnemy::HorizontalFlyingEnemy(const Texture& texture, const Vector2f& position)
    : HorizontalFlyingEnemy(texture, position.x, position.y) {

}

void HorizontalFlyingEnemy::update(float delta) {
    setPosition(base.x + std::sin(cycle / 32.0f) * 64.0f, base.y + std::sin(cycle / 16.0f) * 8.0f);
    
    cycle += delta;
    
    gravityScale = 0.0f;
    Enemy::update(delta);
}