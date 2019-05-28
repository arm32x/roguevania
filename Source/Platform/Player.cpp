#include "Player.hpp"

using namespace RoguelikeMetroidvania;
using namespace RoguelikeMetroidvania::Platform;
using namespace sf;

void Player::update(float delta) {
    Entity::update(delta);
    
    // Handle movement controls.
    /**/ if (Keyboard::isKeyPressed(Keyboard::D)) accelerate(( 2.0f - getVelocity().x) / 4.0f * delta, 0.0f);
    else if (Keyboard::isKeyPressed(Keyboard::A)) accelerate((-2.0f - getVelocity().x) / 4.0f * delta, 0.0f);
    else /**************************************/ accelerate(getVelocity().x / -4.0f * delta, 0.0f);
}

void Player::event(const Event& e) {
    switch (e.type) {
        case Event::KeyPressed:
            switch (e.key.code) {
                case Keyboard::Space:
                    if (onGround) {
                        setVelocity(getVelocity().x, -4.25f);
                    }
                    break;
            }
            break;
        default:
            Entity::event(e);
            break;
    }
}