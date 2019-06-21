#include "SineWaveEnemy.hpp"

#include <cmath>

using namespace Roguevania;
using namespace Roguevania::Platform;
using namespace sf;

SineWaveEnemy::SineWaveEnemy(const Texture& texture, const IntRect& rectangle, float x, float y, float distanceMultiplier)
    : Enemy(texture, rectangle), base(x, y), distanceMultiplier(distanceMultiplier) {
    setPosition(x, y);
}

SineWaveEnemy::SineWaveEnemy(const Texture& texture, const IntRect& rectangle, const Vector2f& position, float distanceMultiplier)
    : SineWaveEnemy(texture, rectangle, position.x, position.y, distanceMultiplier) {

}

SineWaveEnemy::SineWaveEnemy(const Texture& texture, float x, float y, float distanceMultiplier)
    : Enemy(texture), base(x, y), distanceMultiplier(distanceMultiplier) {
    setPosition(x, y);
}

SineWaveEnemy::SineWaveEnemy(const Texture& texture, const Vector2f& position, float distanceMultiplier)
    : SineWaveEnemy(texture, position.x, position.y, distanceMultiplier) {

}

void SineWaveEnemy::update(float delta) {
    Vector2f newPosition(base.x + std::sin(cycle / 32.0f) * distanceMultiplier, base.y + std::sin(cycle / 8.0f) * 4.0f);
    setVelocity(newPosition - getPosition());
    
    cycle += delta;
    
    gravityScale = 0.0f;
    Enemy::update(delta);
}