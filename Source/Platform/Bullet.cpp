#include "Bullet.hpp"

#include <algorithm>

#include "../Collision/CollisionMode.hpp"

using namespace Roguevania;
using namespace Roguevania::Collision;
using namespace Roguevania::Platform;
using namespace sf;

std::vector<Bullet> Bullet::bullets;

Bullet::Bullet(const Texture& texture, const IntRect& rectangle, float x, float y, float velocityX, float velocityY)
    : Entity(texture, rectangle) {
    setPosition(x, y);
    setVelocity(velocityX, velocityY);
    setCollisionMode(CollisionMode::Bullet);
}

Bullet::Bullet(const Texture& texture, const IntRect& rectangle, const Vector2f& position, const Vector2f& velocity)
    : Bullet(texture, rectangle, position.x, position.y, velocity.x, velocity.y) {
    
}

void Bullet::update(float delta) {
    Vector2f oldVelocity = getVelocity();
    gravityScale = 0.0f;
    Entity::update(delta);
    if (getVelocity().x != oldVelocity.x || getVelocity().y != oldVelocity.y) {
        setActive(false);
    }
}