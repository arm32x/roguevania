#include "Player.hpp"

#include "../Platform/Bullet.hpp"

using namespace Roguevania;
using namespace Roguevania::Platform;
using namespace sf;

void Player::update(float delta) {
    if (jumping && getVelocity().y < 0.0f) {
        gravityScale = 0.3f;
    } else {
        gravityScale = 1.0f;
    }
    Entity::update(delta);
    
    // Handle movement controls.
    if (Keyboard::isKeyPressed(Keyboard::D) && !Keyboard::isKeyPressed(Keyboard::A) && !Keyboard::isKeyPressed(Keyboard::LShift)) {
        accelerate(( 2.0f - getVelocity().x) / 4.0f * delta, 0.0f);
    } else if (Keyboard::isKeyPressed(Keyboard::A) && !Keyboard::isKeyPressed(Keyboard::D) && !Keyboard::isKeyPressed(Keyboard::LShift)) {
        accelerate((-2.0f - getVelocity().x) / 4.0f * delta, 0.0f);
    } else {
        accelerate(getVelocity().x / (-4.0f * delta), 0.0f);
    }
}

void Player::event(const Event& e) {
    switch (e.type) {
        case Event::KeyPressed:
            switch (e.key.code) {
                case Keyboard::Space:
                    if (onGround) {
                        jumping = true;
                        setVelocity(getVelocity().x, -3.75f);
                    }
                    break;
                case Keyboard::Enter: {
                    bool up    = Keyboard::isKeyPressed(Keyboard::W);
                    bool right = Keyboard::isKeyPressed(Keyboard::D);
                    bool down  = Keyboard::isKeyPressed(Keyboard::S);
                    bool left  = Keyboard::isKeyPressed(Keyboard::A);
                    
                    int8_t direction;
                    /**/ if ( up && !down && !left == !right) direction = 0;
                    else if ( up && !down && !left &&  right) direction = 1;
                    else if (!up == !down && !left &&  right) direction = 2;
                    else if (!up &&  down && !left &&  right) direction = 3;
                    else if (!up &&  down && !left == !right) direction = 4;
                    else if (!up &&  down &&  left && !right) direction = 5;
                    else if (!up == !down &&  left && !right) direction = 6;
                    else if ( up && !down &&  left && !right) direction = 7;
                    else break;
                    
                    switch (direction) {
                        case 0: // up
                            Bullet::bullets.push_back(new Bullet(Entity::spritesheet, IntRect(22, 34,  4, 12), getPosition().x + 6.0f, getPosition().y + 0.0f,  0.0f, -1.0f));
                            break;
                        case 1: // up-right
                            Bullet::bullets.push_back(new Bullet(Entity::spritesheet, IntRect(51, 35, 10, 10), getPosition().x + 6.0f, getPosition().y + 0.0f,  1.0f, -1.0f));
                            break;
                        case 2: // right
                            Bullet::bullets.push_back(new Bullet(Entity::spritesheet, IntRect( 2, 38, 12,  4), getPosition().x + 4.0f, getPosition().y + 6.0f,  1.0f,  0.0f));
                            break;
                        case 3: // down-right
                            Bullet::bullets.push_back(new Bullet(Entity::spritesheet, IntRect(35, 35, 10, 10), getPosition().x + 6.0f, getPosition().y + 6.0f,  1.0f,  1.0f));
                            break;
                        case 4: // down
                            Bullet::bullets.push_back(new Bullet(Entity::spritesheet, IntRect(22, 34,  4, 12), getPosition().x + 6.0f, getPosition().y + 4.0f,  0.0f,  1.0f));
                            break;
                        case 5: // down-left
                            Bullet::bullets.push_back(new Bullet(Entity::spritesheet, IntRect(35, 35, 10, 10), getPosition().x + 0.0f, getPosition().y + 6.0f, -1.0f,  1.0f));
                            break;
                        case 6: // left
                            Bullet::bullets.push_back(new Bullet(Entity::spritesheet, IntRect( 2, 38, 12,  4), getPosition().x + 0.0f, getPosition().y + 6.0f, -1.0f,  0.0f));
                            break;
                        case 7: // up-left
                            Bullet::bullets.push_back(new Bullet(Entity::spritesheet, IntRect(35, 35, 10, 10), getPosition().x + 0.0f, getPosition().y + 0.0f, -1.0f, -1.0f));
                            break;
                    }
                    break;
                }
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