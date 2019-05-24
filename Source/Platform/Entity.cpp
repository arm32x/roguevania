#include "Entity.hpp"

#include <iostream>

#include "../Collision/CollisionMode.hpp"
#include "../Collision/TilemapCollider.hpp"
#include "../Exceptions/Exception.hpp"
#include "../Program.hpp"

using namespace RoguelikeMetroidvania;
using namespace RoguelikeMetroidvania::Collision;
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

void Entity::accelerate(const Vector2f& value) {
    velocity += value;
}

void Entity::accelerate(float x, float y) {
    accelerate(Vector2f(x, y));
}

Vector2f Entity::alignPosition() {
    Vector2f old = getPosition();
    setPosition(std::round(old.x), std::round(old.y));
    return old;
}

void Entity::update(float delta) {
    // Apply the current velocity and apply gravity.
    accelerate(0.0f, 9.801f / 60 * delta);
    move(getVelocity() * delta);
    onGround = false;
    
    // Detect and handle collisions.
    for (TilemapCollider* collider : TilemapCollider::all) {
        std::vector<Vector2<uint16_t>> tiles = collider->getTilesTouching(*this);
        CollisionMode mode = collider->prioritizeTileModes(tiles);
        switch (mode) {
            case CollisionMode::Undefined: {
                Program::log(Log::Error, "EntityCollision") << "Undefined collision mode." << std::endl;
                throw Exceptions::Exception("Undefined collision mode.");
                break;
            }
            case CollisionMode::SolidFloor:
            case CollisionMode::Solid: {
                constexpr float increment = 0.0625f;
                Vector2f velocity = getVelocity() * delta;
                onGround = true;
                Vector2f amountMoved;
                for (amountMoved.y = 0.0f; (mode == CollisionMode::Solid || mode == CollisionMode::SolidFloor) && std::abs(amountMoved.y) <= std::abs(velocity.y); amountMoved.y += velocity.y >= 0.0f ? -increment : increment) {
                    move(0.0f, velocity.y >= 0.0f ? -increment : increment);
                    setVelocity(velocity.x, 0.0f);
                    mode = collider->prioritizeTileModes(collider->getTilesTouching(*this));
                }
                if (mode == CollisionMode::Solid || mode == CollisionMode::SolidFloor) {
                    // TODO:  Implement this.
                    Program::log(Log::Debug, "EntityCollision") << "Horizontal collision detected." << std::endl;
                    move(0.0f, -amountMoved.y);
                    for (amountMoved.x = 0.0f; (mode == CollisionMode::Solid || mode == CollisionMode::SolidFloor) && std::abs(amountMoved.x) <= std::abs(velocity.x); amountMoved.x += velocity.x >= 0.0f ? -increment : increment) {
                        move(velocity.x >= 0.0f ? -increment : increment, 0.0f);
                        setVelocity(0.0f, velocity.y);
                        mode = collider->prioritizeTileModes(collider->getTilesTouching(*this));
                    }
                    for (amountMoved.y = 0.0f; (mode == CollisionMode::Solid || mode == CollisionMode::SolidFloor) && std::abs(amountMoved.y) <= std::abs(velocity.y); amountMoved.y += velocity.y >= 0.0f ? -increment : increment) {
                        move(0.0f, velocity.y >= 0.0f ? -increment : increment);
                        setVelocity(velocity.x, 0.0f);
                        mode = collider->prioritizeTileModes(collider->getTilesTouching(*this));
                    }
                }
                break;
            }
        }
    }
    
    // Handle movement controls.
    /**/ if (Keyboard::isKeyPressed(Keyboard::D)) accelerate(( 2.0f - getVelocity().x) / 2.0f * delta, 0.0f);
    else if (Keyboard::isKeyPressed(Keyboard::A)) accelerate((-2.0f - getVelocity().x) / 2.0f * delta, 0.0f);
    else /**************************************/ accelerate(getVelocity().x / -4.0f * delta, 0.0f);
}

void Entity::event(const Event& e) {
    switch (e.type) {
        case Event::KeyPressed:
            if (e.key.code == Keyboard::Space && onGround) {
                setVelocity(getVelocity().x, -4.25f);
                break;
            }
            break;
        default:
            break;
    }
}