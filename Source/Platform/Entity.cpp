#include "Entity.hpp"

#include <iostream>

#include "../Collision/TilemapCollider.hpp"
#include "../Exceptions/Exception.hpp"
#include "../Maps/Tilemap.hpp"
#include "../Utilities/clamp.hpp"
#include "../Program.hpp"

using namespace Roguevania;
using namespace Roguevania::Collision;
using namespace Roguevania::Platform;
using namespace sf;

Entity::Entity(const Texture& texture)
    : Sprite(texture) {
    all.push_back(this);
}

Entity::Entity(const Texture& texture, const IntRect& rectangle)
    : Sprite(texture, rectangle) {
    all.push_back(this);
}

Entity::~Entity() {
    all.erase(std::remove(all.begin(), all.end(), this), all.end());
}

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

CollisionMode Entity::getCollisionMode() const {
    return collisionMode;
}

void Entity::setCollisionMode(CollisionMode mode) {
    collisionMode = mode;
}

Vector2f Entity::alignPosition() {
    Vector2f old = getPosition();
    setPosition(std::round(old.x), std::round(old.y));
    return old;
}

void Entity::update(float delta) {
    // Apply the current velocity and apply gravity.
    accelerate(0.0f, 0.25f * delta * gravityScale);
    Utilities::clamp(&velocity.x, -TerminalVelocity, TerminalVelocity);
    Utilities::clamp(&velocity.y, -TerminalVelocity, TerminalVelocity);
    move(getVelocity() * delta);
    onGround = false;
    
    // Detect and handle collisions.
    // TODO:  Implement slopes.
    for (TilemapCollider* collider : TilemapCollider::all) {
        std::vector<Vector2<uint16_t>> tiles = collider->getTilesTouching(*this);
        CollisionMode mode = collider->prioritizeTileModes(tiles);
        constexpr float increment = 0.0625f;
        switch (mode) {
            case CollisionMode::None: {
                break;
            }
            case CollisionMode::Solid: {
                Vector2f velocity = getVelocity();
                onGround = true;
                for (float amountMoved = 0.0f; (mode == CollisionMode::Solid) && std::abs(amountMoved) <= std::abs(velocity.y * delta); amountMoved += velocity.y >= 0.0f ? -increment : increment) {
                    move(0.0f, velocity.y >= 0.0f ? -increment : increment);
                    setVelocity(velocity.x, 0.0f);
                    mode = collider->prioritizeTileModes(collider->getTilesTouching(*this));
                }
                if (mode == CollisionMode::Solid) {
                    onGround = false;
                    mode = collider->prioritizeTileModes(collider->getTilesTouching(*this));
                    for (float amountMoved = 0.0f; (mode == CollisionMode::Solid) && std::abs(amountMoved) <= std::abs(velocity.x * delta); amountMoved += velocity.x >= 0.0f ? -increment : increment) {
                        move(velocity.x >= 0.0f ? -increment : increment, 0.0f);
                        setVelocity(0.0f, velocity.y);
                        mode = collider->prioritizeTileModes(collider->getTilesTouching(*this));
                    }
                    move(0.0f, velocity.y);
                    mode = collider->prioritizeTileModes(collider->getTilesTouching(*this));
                    if (mode == CollisionMode::Solid) {
                        onGround = true;
                        for (float amountMoved = 0.0f; (mode == CollisionMode::Solid && std::abs(amountMoved) <= std::abs(velocity.y * delta)); amountMoved += velocity.y >= 0.0f ? -increment : increment) {
                            move(0.0f, velocity.y >= 0.0f ? -increment : increment);
                            setVelocity(velocity.x, 0.0f);
                            mode = collider->prioritizeTileModes(collider->getTilesTouching(*this));
                        }
                    } else if (mode == CollisionMode::SemiSolid && (velocity.y > 0.0f && !Keyboard::isKeyPressed(Keyboard::S))) {
                        onGround = true;
                        float amountMoved;
                        for (amountMoved = 0.0f; (mode == CollisionMode::SemiSolid) && std::abs(amountMoved) <= std::abs(velocity.y * delta); amountMoved += velocity.y >= 0.0f ? -increment : increment) {
                            move(0.0f, velocity.y >= 0.0f ? -increment : increment);
                            setVelocity(velocity.x, 0.0f);
                            mode = collider->prioritizeTileModes(collider->getTilesTouching(*this));
                        }
                        if (mode == CollisionMode::SemiSolid) {
                            onGround = false;
                            move(0.0f, -amountMoved);
                            setVelocity(velocity.x, velocity.y);
                        }   
                    }
                }
                break;
            }
            // TODO:  Fix falling through semisolid platforms when rubbing against solid walls.
            case CollisionMode::SemiSolid: {
                Vector2f velocity = getVelocity();
                if (velocity.y > 0.0f) {
                    onGround = true;
                    float amountMoved;
                    for (amountMoved = 0.0f; (mode == CollisionMode::SemiSolid) && std::abs(amountMoved) <= std::abs(velocity.y * delta); amountMoved += velocity.y >= 0.0f ? -increment : increment) {
                        move(0.0f, velocity.y >= 0.0f ? -increment : increment);
                        setVelocity(velocity.x, 0.0f);
                        mode = collider->prioritizeTileModes(collider->getTilesTouching(*this));
                    }
                    if (mode == CollisionMode::SemiSolid) {
                        onGround = false;
                        move(0.0f, -amountMoved);
                        setVelocity(velocity.x, velocity.y);
                    }
                }
                if (mode != CollisionMode::SemiSolid && Keyboard::isKeyPressed(Keyboard::S)) {
                    setVelocity(velocity.x, 0.0f);
                }
                break;
            }
#if FALSE
            case CollisionMode::SlopeTL: {
                auto getRelativePosition = [this]() -> Vector2f {
                    return Vector2f(std::fmod(getPosition().x + 16, 16), 16 - std::fmod(getPosition().y + 30, 16));
                };
                Vector2f relativePosition = getRelativePosition();
                Program::log(Log::Debug, "EntityCollision") << "Relative position:  (" << relativePosition.x << ", " << relativePosition.y << ")." << std::endl;
                if (relativePosition.x < relativePosition.y) {
                    onGround = true;
                    for (float amountMoved = 0.0f; (mode == CollisionMode::SlopeTL && relativePosition.x < relativePosition.y) && std::abs(amountMoved) <= std::abs(velocity.y * delta) + 16; amountMoved += velocity.y >= 0.0f ? -increment : increment) {
                        move(0.0f, velocity.y >= 0.0f ? -increment : increment);
                        setVelocity(velocity.x, 0.0f);
                        mode = collider->prioritizeTileModes(collider->getTilesTouching(*this));
                        relativePosition = getRelativePosition();
                    }
                }
                break;
            }
#endif
            case CollisionMode::NotTouching: {
                break;
            }
            default: {
                Program::log(Log::Error, "EntityCollision") << "Undefined collision mode " << mode << "." << std::endl;
                throw Exceptions::Exception("Undefined collision mode.");
                break;
            }
        }
    }
}

void Entity::event(const Event& e) {
    
}

std::vector<Entity*> Entity::all;