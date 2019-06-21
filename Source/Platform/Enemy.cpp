#include "Enemy.hpp"

using namespace Roguevania;
using namespace Roguevania::Collision;
using namespace Roguevania::Platform;
using namespace sf;

void Enemy::collide(CollisionMode mode) {
    if (mode == CollisionMode::Bullet) setActive(false);
}