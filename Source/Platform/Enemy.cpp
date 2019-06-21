#include "Enemy.hpp"

#include "../Platform/Bullet.hpp"

using namespace Roguevania;
using namespace Roguevania::Collision;
using namespace Roguevania::Platform;
using namespace sf;

void Enemy::collide(CollisionMode mode) {
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