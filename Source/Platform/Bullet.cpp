#include "Bullet.hpp"

#include <algorithm>

using namespace Roguevania;
using namespace Roguevania::Platform;
using namespace sf;

std::vector<Bullet*> Bullet::bullets;

Bullet::Bullet(const Texture& texture, const IntRect& rectangle, float x, float y, float velocityX, float velocityY)
    : Entity(texture, rectangle) {
    setPosition(x, y);
    setVelocity(velocityX, velocityY);
}

Bullet::Bullet(const Texture& texture, const IntRect& rectangle, const Vector2f& position, const Vector2f& velocity)
    : Bullet(texture, rectangle, position.x, position.y, velocity.x, velocity.y) {
    
}

void Bullet::update(float delta) {
    Vector2f oldVelocity = getVelocity();
    gravityScale = 0.0f;
    Entity::update(delta);
    if (getVelocity().x != oldVelocity.x || getVelocity().y != oldVelocity.y) {
        auto it = std::find(bullets.begin(), bullets.end(), this);
        if (it != bullets.end()) {
            bullets.erase(it);
            delete this; // yeetus yeetus commit self deletus
        }
        return;
    }
}