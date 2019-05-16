#include "Entity.hpp"

using namespace RoguelikeMetroidvania;
using namespace RoguelikeMetroidvania::Platform;
using namespace sf;

const Vector2f& Entity::getVelocity() const {
    return velocity;
}

void Entity::setVelocity(const Vector2f& value) {
    velocity = value;
}

void Entity::setVelocity(float x, float y) {
    setVelocity(Vector2f(x, y));
}

void Entity::update(float delta) {
    setPosition(getPosition() + getVelocity() * delta);
    setVelocity(getVelocity() + Vector2f(0.0f, 9.801f / 60 * delta));
}