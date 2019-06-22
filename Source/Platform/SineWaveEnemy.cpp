#include "SineWaveEnemy.hpp"

#include <cmath>

using namespace Roguevania;
using namespace Roguevania::Platform;
using namespace sf;

SineWaveEnemy::SineWaveEnemy(const Texture& texture, const IntRect& rectangle, float x, float y, const Vector2f& speed, const Vector2f& distance)
    : Enemy(texture, rectangle), base(x, y), speed(speed), distance(distance) {
    setPosition(x, y);
}

SineWaveEnemy::SineWaveEnemy(const Texture& texture, const IntRect& rectangle, const Vector2f& position, const Vector2f& speed, const Vector2f& distance)
    : SineWaveEnemy(texture, rectangle, position.x, position.y, speed, distance) {

}

SineWaveEnemy::SineWaveEnemy(const Texture& texture, float x, float y, const Vector2f& speed, const Vector2f& distance)
    : Enemy(texture), base(x, y), speed(speed), distance(distance) {
    setPosition(x, y);
}

SineWaveEnemy::SineWaveEnemy(const Texture& texture, const Vector2f& position, const Vector2f& speed, const Vector2f& distance)
    : SineWaveEnemy(texture, position.x, position.y, speed, distance) {

}

void SineWaveEnemy::update(float delta) {
    Vector2f newPosition(base.x + std::sin(cycle / speed.x) * distance.x, base.y + std::sin(cycle / speed.y) * distance.y);
    setVelocity(newPosition - getPosition());
    
    cycle += delta;
    
    gravityScale = 0.0f;
    Enemy::update(delta);
}