#include "Bullet.hpp"

#include <algorithm>

#include "../Program.hpp"

using namespace Roguevania;
using namespace Roguevania::Platform;
using namespace sf;

std::vector<Bullet*> Bullet::bullets;

Bullet::Bullet(const Texture& texture, const IntRect& rectangle, float x, float y, float velocityX, float velocityY)
    : Entity(texture, rectangle) {
    Program::log(Log::Debug, "Bullet") << "Bullet created." << std::endl;
    setPosition(x, y);
    setVelocity(velocityX, velocityY);
}

Bullet::Bullet(const Texture& texture, const IntRect& rectangle, const Vector2f& position, const Vector2f& velocity)
    : Bullet(texture, rectangle, position.x, position.y, velocity.x, velocity.y) {
    
}

Bullet::~Bullet() {
    Program::log(Log::Debug, "Bullet") << "Bullet destroyed." << std::endl;
}

void Bullet::update(float delta) {
    gravityScale = 0.0f;
    Entity::update(delta);
    if (getVelocity().x == 0.0f || getVelocity().y == 0.0f) {
        auto it = std::find(bullets.begin(), bullets.end(), this);
        if (it != bullets.end()) {
            bullets.erase(it, bullets.end());
            delete this; // yeetus yeetus commit self deletus
        }
        return;
    }
}