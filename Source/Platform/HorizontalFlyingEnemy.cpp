#include "HorizontalFlyingEnemy.hpp"

#include <cmath>

using namespace Roguevania;
using namespace Roguevania::Platform;
using namespace sf;

HorizontalFlyingEnemy::HorizontalFlyingEnemy(const Texture& texture, const IntRect& rectangle, float x, float y, float distanceMultiplier)
    : Enemy(texture, rectangle), base(x, y), distanceMultiplier(distanceMultiplier) {
    setPosition(x, y);
}

HorizontalFlyingEnemy::HorizontalFlyingEnemy(const Texture& texture, const IntRect& rectangle, const Vector2f& position, float distanceMultiplier)
    : HorizontalFlyingEnemy(texture, rectangle, position.x, position.y, distanceMultiplier) {

}

HorizontalFlyingEnemy::HorizontalFlyingEnemy(const Texture& texture, float x, float y, float distanceMultiplier)
    : Enemy(texture), base(x, y), distanceMultiplier(distanceMultiplier) {
    setPosition(x, y);
}

HorizontalFlyingEnemy::HorizontalFlyingEnemy(const Texture& texture, const Vector2f& position, float distanceMultiplier)
    : HorizontalFlyingEnemy(texture, position.x, position.y, distanceMultiplier) {

}

void HorizontalFlyingEnemy::update(float delta) {
    Vector2f newPosition(base.x + std::sin(cycle / 32.0f) * distanceMultiplier, base.y + std::sin(cycle / 8.0f) * 4.0f);
    setVelocity(newPosition - getPosition());
    
    cycle += delta;
    
    gravityScale = 0.0f;
    Enemy::update(delta);
}