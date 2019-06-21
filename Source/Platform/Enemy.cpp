#include "Enemy.hpp"

#include "../Platform/Bullet.hpp"
#include "../Program.hpp"

using namespace Roguevania;
using namespace Roguevania::Collision;
using namespace Roguevania::Platform;
using namespace sf;

Enemy::Enemy(const Texture& texture, const IntRect& rectangle)
    : Entity(texture, rectangle) {
    setCollisionMode(CollisionMode::Damaging);
}

Enemy::Enemy(const Texture& texture)
    : Entity(texture) {
    setCollisionMode(CollisionMode::Damaging);
}

void Enemy::collide(CollisionMode mode) {
    Program::log(Log::Debug, "Enemy") << "Enemy collide called with mode " << mode << "." << std::endl;
    if (mode == CollisionMode::Bullet) {
        setActive(false);
        for (Entity* entity : Entity::all) {
            if (getTouchingMode(*entity) == CollisionMode::NotTouching) continue;
            Bullet* bullet = dynamic_cast<Bullet*>(entity);
            if (bullet != nullptr) {
                bullet->setActive(false);
            }
        }
    }
}