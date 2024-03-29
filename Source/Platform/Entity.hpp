#pragma once

#include <cstdint>

#include <stx/optional.hpp>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>

#include "../Collision/CollisionMode.hpp"

namespace Roguevania {
namespace Platform {
using namespace Roguevania::Collision;
using namespace sf;
using stx::nullopt;

class Entity : public Sprite {
        
    public:
        Entity(const Texture& texture);
        Entity(const Texture& texture, const IntRect& rectangle);
        ~Entity();
        
        ////////////////////////////////////////////////////////////////////////
        /// Get the velocity of this entity.
        ////////////////////////////////////////////////////////////////////////
        const Vector2f& getVelocity() const;
        
        ////////////////////////////////////////////////////////////////////////
        /// Set the velocity of this entity.
        ////////////////////////////////////////////////////////////////////////
        void setVelocity(const Vector2f& value);
        
        ////////////////////////////////////////////////////////////////////////
        /// Set the velocity of this entity.
        ////////////////////////////////////////////////////////////////////////
        void setVelocity(float x, float y);
        
        ////////////////////////////////////////////////////////////////////////
        /// Change the velocity of this entity.
        ////////////////////////////////////////////////////////////////////////
        void accelerate(const Vector2f& value);
        
        ////////////////////////////////////////////////////////////////////////
        /// Change the velocity of this entity.
        ////////////////////////////////////////////////////////////////////////
        void accelerate(float x, float y);
        
        ////////////////////////////////////////////////////////////////////////
        /// Gets this entity’s collision mode.
        ////////////////////////////////////////////////////////////////////////
        CollisionMode getCollisionMode() const;
        
        ////////////////////////////////////////////////////////////////////////
        /// Sets this entity’s collision mode.
        ////////////////////////////////////////////////////////////////////////
        void setCollisionMode(CollisionMode mode);
        
        ////////////////////////////////////////////////////////////////////////
        /// Align this entity’s position to the nearest pixel.
        /// 
        /// @return
        ///     The position before alignment.
        ////////////////////////////////////////////////////////////////////////
        Vector2f alignPosition();
        
        ////////////////////////////////////////////////////////////////////////
        /// A multiplier for the amount of gravity that is applied to this
        /// entity.
        ////////////////////////////////////////////////////////////////////////
        float gravityScale = 1.0f;
        
        ////////////////////////////////////////////////////////////////////////
        /// Called on every frame to update the entity and do game logic.
        /// 
        /// @param delta
        ///     A multiplier value calculated based on the frames per second.
        ///     1.0 is 60fps, 0.5 is 30fps, and 2.0 is 120fps.
        ////////////////////////////////////////////////////////////////////////
        virtual void update(float delta);
        
        ////////////////////////////////////////////////////////////////////////
        /// Allows entities to handle events taken from the window event loop.
        ////////////////////////////////////////////////////////////////////////
        virtual void event(const Event& e);
        
        ////////////////////////////////////////////////////////////////////////
        /// Used so that subclasses can handle collisions.
        ////////////////////////////////////////////////////////////////////////
        virtual void collide(CollisionMode mode) { };
        
        ////////////////////////////////////////////////////////////////////////
        /// Contains all of the entities that currently exist.
        ////////////////////////////////////////////////////////////////////////
        static std::vector<Entity*> all;
        
        ////////////////////////////////////////////////////////////////////////
        /// A spritesheet for entities.  Is initialized and used outside of this
        /// class.
        ////////////////////////////////////////////////////////////////////////
        static Texture spritesheet;
        
        ////////////////////////////////////////////////////////////////////////
        bool getActive() const;
        
        ////////////////////////////////////////////////////////////////////////
        void setActive(bool value);
        
        ////////////////////////////////////////////////////////////////////////
        /// Gets the collision mode of another entity if that entity is touching
        /// this one, otherwise returns NotTouching.
        ////////////////////////////////////////////////////////////////////////
        CollisionMode getTouchingMode(const Entity& entity) const;
        
    protected:
        ////////////////////////////////////////////////////////////////////////
        /// The velocity of this entity, measured in pixels per frame.
        ////////////////////////////////////////////////////////////////////////
        Vector2f velocity;
        
        ////////////////////////////////////////////////////////////////////////
        /// Whether or not this entity is currently on the ground.  Used in 
        /// subclasses for various purposes.
        ////////////////////////////////////////////////////////////////////////
        bool onGround = false;
        
        ////////////////////////////////////////////////////////////////////////
        /// The maximum velocity that an entity can go in any direction.
        ////////////////////////////////////////////////////////////////////////
        static constexpr float TerminalVelocity = 20.0f;
        
        ////////////////////////////////////////////////////////////////////////
        /// The collision mode that determines how this entity interacts with
        /// other entities.
        ////////////////////////////////////////////////////////////////////////
        CollisionMode collisionMode;
        
        ////////////////////////////////////////////////////////////////////////
        /// Used to determine whether or not to delete an entity.
        ////////////////////////////////////////////////////////////////////////
        bool active = true;
        
};

};
};