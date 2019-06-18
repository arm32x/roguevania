#include "Player.hpp"

using namespace Roguevania;
using namespace Roguevania::Platform;
using namespace sf;

void Player::update(float delta) {
    if (jumping && getVelocity().y < 0.0f) {
        gravityScale = 0.6f;
    } else {
        gravityScale = 1.0f;
    }
    Entity::update(delta);
    
    // Handle movement controls.
    /**/ if (Keyboard::isKeyPressed(Keyboard::D)) accelerate(( 2.0f - getVelocity().x) / 4.0f * delta, 0.0f);
    else if (Keyboard::isKeyPressed(Keyboard::A)) accelerate((-2.0f - getVelocity().x) / 4.0f * delta, 0.0f);
    else /**************************************/ accelerate(getVelocity().x / (-4.0f * delta), 0.0f);
}

void Player::event(const Event& e) {
    switch (e.type) {
        case Event::KeyPressed:
            switch (e.key.code) {
                case Keyboard::Space:
                    if (onGround) {
                        jumping = true;
                        setVelocity(getVelocity().x, -4.25f);
                    }
                    break;
                default:
                    Entity::event(e);
                    break;
            }
            break;
        case Event::KeyReleased:
            switch (e.key.code) {
                case Keyboard::Space:
                    jumping = false;
                    break;
                default:
                    Entity::event(e);
                    break;
            }
        default:
            Entity::event(e);
            break;
    }
}