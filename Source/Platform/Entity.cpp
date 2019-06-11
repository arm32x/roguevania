#include "Entity.hpp"

#include <iostream>

#include "../Collision/CollisionMode.hpp"
#include "../Collision/TilemapCollider.hpp"
#include "../Exceptions/Exception.hpp"
#include "../Utilities/clamp.hpp"
#include "../Program.hpp"

using namespace Roguevania;
using namespace Roguevania::Collision;
using namespace Roguevania::Platform;
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
    accelerate(0.0f, 0.25f * delta);
    Utilities::clamp(&velocity.x, -TerminalVelocity, TerminalVelocity);
    Utilities::clamp(&velocity.y, -TerminalVelocity, TerminalVelocity);
    move(getVelocity() * delta);
    onGround = false;
    
    // Detect and handle collisions.
    // TODO:  Fix this.
    auto getCollisionMode = [this]() {
        std::vector<CollisionMode> modes;
        for (TilemapCollider* collider : TilemapCollider::all) {
            std::vector<Vector2<uint16_t>> tiles = collider->getTilesTouching(*this);
            CollisionMode mode = collider->prioritizeTileModes(tiles);
            modes.push_back(mode);
        }
        return CollisionMode::prioritize(modes);
    };
    CollisionMode mode = getCollisionMode();
    switch (mode) {
        case CollisionMode::None: {
            break;
        }
        case CollisionMode::Solid: {
            constexpr float increment = 0.0625f;
            Vector2f velocity = getVelocity() * delta;
            onGround = true;
            for (float amountMoved = 0.0f; (mode == CollisionMode::Solid) && std::abs(amountMoved) <= std::abs(velocity.y); amountMoved += velocity.y >= 0.0f ? -increment : increment) {
                move(0.0f, velocity.y >= 0.0f ? -increment : increment);
                setVelocity(velocity.x, 0.0f);
                mode = getCollisionMode();
            }
            if (mode == CollisionMode::Solid) {
                onGround = false;
                mode = getCollisionMode();
                for (float amountMoved = 0.0f; (mode == CollisionMode::Solid) && std::abs(amountMoved) <= std::abs(velocity.x); amountMoved += velocity.x >= 0.0f ? -increment : increment) {
                    move(velocity.x >= 0.0f ? -increment : increment, 0.0f);
                    setVelocity(0.0f, velocity.y);
                    mode = getCollisionMode();
                }
                move(0.0f, velocity.y);
                mode = getCollisionMode();
                if (mode == CollisionMode::Solid) {
                    onGround = true;
                    for (float amountMoved = 0.0f; (mode == CollisionMode::Solid) && std::abs(amountMoved) <= std::abs(velocity.y); amountMoved += velocity.y >= 0.0f ? -increment : increment) {
                        move(0.0f, velocity.y >= 0.0f ? -increment : increment);
                        setVelocity(velocity.x, 0.0f);
                        mode = getCollisionMode();
                    }
                }
            }
            break;
        }
        case CollisionMode::NotTouching: {
            break;
        }
        default: {
            Program::log(Log::Error, "EntityCollision") << "Undefined collision mode " << std::uppercase << std::hex << +mode << std::dec << std::nouppercase << "." << std::endl;
            throw Exceptions::Exception("Undefined collision mode.");
            break;
        }
    }
}

void Entity::event(const Event& e) {
    
}