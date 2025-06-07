#ifndef BODY_H
#define BODY_H

#include <SDL2/SDL.h>
#include <my-lib/math-vector.h>
#include <nlohmann/json.hpp>
#include "managers/MultiAnimatorManager.h"

#include "Entity.h"
#include "../core/GameConstants.h"
#include "../renders/Sprite.h"
#include "../utils/Types.h"

using json = nlohmann::json;

namespace Entities
{
    class Body : public Entity
    {
    protected:
		Vector2f position;
		Vector2f scale;
        Vector2f hitboxOffset = {0.f, 0.f};
        Vector2f hitboxSize = {0.f, 0.f};
        bool has_collision;
        bool is_visible;
        bool is_animated = false;
        SDL_Texture* texture = nullptr;

        MultiAnimator animator;
        public:
        #pragma region Constructors
        Body(Vector2f pos, Vector2f scl, bool collision = GameConstants::Body::DEFAULT_COLLISION, bool visible = GameConstants::Body::DEFAULT_VISIBLE)
            : position(pos), scale(scl),
            has_collision(collision),
            is_visible(visible) {}

        Body(Vector4f collider, bool collision = GameConstants::Body::DEFAULT_COLLISION, bool visible = GameConstants::Body::DEFAULT_VISIBLE)
            : Body(Vector2f(collider.x, collider.y), Vector2f(collider.z, collider.w), collision, visible) {}
        #pragma endregion
		
        virtual void update(float deltaTime);
        virtual void render(SDL_Renderer* renderer);

        virtual void onCollision(Body* other) {};
        virtual void loadAnimations() {};

        Vector4f getFullSize() const { return Vector4f(this->position.x, this->position.y, this->scale.x, this->scale.y); }
        Pointf getCenterPoint() const { return Pointf(this->position.x + this->scale.x / 2, this->position.y + this->scale.y / 2); } 
        Vector4f getHitbox() const {
            if (this->hitboxOffset == Vector2f{0.f, 0.f}) return getFullSize();

            float offsetX = std::min(hitboxOffset.x, 0.98f);
            float offsetY = std::min(hitboxOffset.y, 0.98f);

            float marginX = scale.x * offsetX * 0.5f;
            float marginY = scale.y * offsetY * 0.5f;

            float newW = scale.x - (2 * marginX);
            float newH = scale.y - (2 * marginY);

            return Vector4f(position.x + marginX, position.y + marginY, newW, newH);
        }

        Vector4f getHurtbox() const { return this->getHitbox(); }

        
		#pragma region Getters
        bool hasCollision() const { return this->has_collision; }
        Vector2f getPosition() const { return this->position; }
        Vector2f getScale() const { return this->scale; }
        bool isVisible() const { return this->is_visible; }
        bool isAnimated() const { return this->is_animated; }
        MultiAnimator& getAnimationManager() { return this->animator; }
        const MultiAnimator& getAnimationManager() const { return this->animator; }
        inline SDL_FRect getRect() const { return { this->position.x, this->position.y, this->scale.x, this->scale.y }; }
        SDL_Texture* getTexture() const { return this->texture; }
		#pragma endregion

		#pragma region Setters
        void setCollision(bool collision) { this->has_collision = collision; }
        void setVisible(bool visible) { this->is_visible = visible; }
        void setPosition(float x, float y) { this->position.x = x; this->position.y = y; }
        void setPosition(Vector2f pos) { this->position = pos; }
        void setScale(float w, float h) { this->scale.x = w; this->scale.y = h; }
        void setScale(Vector2f scl) { this->scale = scl; }
        void setHitboxMargin(float marginX, float marginY) { this->hitboxOffset.x = marginX;this->hitboxOffset.y = marginY; }
        void setHitboxMargin(Vector2f margin) { this->hitboxOffset = margin; }
        void setTexture(SDL_Texture* texture) { this->texture = texture; }
        void setAnimated(bool animated) { this->is_animated = animated; }
		#pragma endregion
    };
}

#endif
