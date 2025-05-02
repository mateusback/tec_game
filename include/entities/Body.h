#ifndef BODY_H
#define BODY_H

#include <SDL2/SDL.h>
#include <my-lib/math-vector.h>
#include <nlohmann/json.hpp>
#include "managers/AnimationManager.h"

#include "Entity.h"
#include "renders/Sprite.h"

using Vector = Mylib::Math::Vector<float, 2>;
using Vector4 = Mylib::Math::Vector<float, 4>;
using Point = Vector;
using json = nlohmann::json;

namespace Entities
{
    class Body : public Entity
    {
    protected:
		Vector position;
		Vector scale;
        Vector hitboxOffset = {0.f, 0.f};
        bool has_collision;
        bool is_visible;
        bool is_animated = false;
        SDL_Texture* texture = nullptr;

        Manager::AnimationManager animationManager;
        
        public:
        #pragma region Constructors
        Body() 
            : position(0, 0), 
            scale(50, 50), 
            has_collision(false), 
            is_visible(true) {}

        Body(float x = 0, float y = 0, float w = 50, float h = 50, bool collision = false, bool visible = true)
            : position(x, y), 
            scale(w, h), 
            has_collision(collision), 
            is_visible(visible) {}
        
        Body(Vector pos, Vector scl, bool collision = false, bool visible = true)
            : position(pos), scale(scl), 
            has_collision(collision), 
            is_visible(visible) {}
        
        Body(Vector4 collider, bool collision = false, bool visible = true)
            : position(collider.x, collider.y), 
            scale(collider.z, collider.w), 
            has_collision(collision), 
            is_visible(visible) {}
        #pragma endregion
		
        virtual void update(float deltaTime);

        virtual void render(SDL_Renderer* renderer);

        virtual void onCollision(Body* other) {};
        virtual void loadAnimations() {};

        Vector4 getFullSize() const { return Vector4(this->position.x, this->position.y, this->scale.x, this->scale.y); }
        Point getCenterPoint() const { return Point(this->position.x + this->scale.x / 2, this->position.y + this->scale.y / 2); } 
        Vector4 getHitbox() const {
            if (this->hitboxOffset == Vector{0.f, 0.f}) return getFullSize();
        
            float marginX = scale.x * hitboxOffset.x * 0.5f;
            float marginY = scale.y * hitboxOffset.y * 0.5f;
        
            float newW = scale.x - (2 * marginX);
            float newH = scale.y - (2 * marginY);
        
            return Vector4(position.x + marginX, position.y + marginY, newW, newH);
        }
        
        
		#pragma region Getters
        bool hasCollision() const { return this->has_collision; }
        Vector getPosition() const { return this->position; }
        Vector getScale() const { return this->scale; }
        bool isVisible() const { return this->is_visible; }
        Manager::AnimationManager& getAnimationManager() { return this->animationManager; }
        const Manager::AnimationManager& getAnimationManager() const { return this->animationManager; }
        inline SDL_FRect getRect() const { return { this->position.x, this->position.y, this->scale.x, this->scale.y }; }
        SDL_Texture* getTexture() const { return this->texture; }
		#pragma endregion

		#pragma region Setters
        void setCollision(bool collision) { this->has_collision = collision; }
        void setVisible(bool visible) { this->is_visible = visible; }
        void setPosition(float x, float y) { this->position.x = x; this->position.y = y; }
        void setPosition(Vector pos) { this->position = pos; }
        void setScale(float w, float h) { this->scale.x = w; this->scale.y = h; }
        void setScale(Vector scl) { this->scale = scl; }
        void setHitboxMargin(float marginX, float marginY) { this->hitboxOffset.x = marginX;this->hitboxOffset.y = marginY; }
        void setHitboxMargin(Vector margin) { this->hitboxOffset = margin; }
        void setTexture(SDL_Texture* texture) { this->texture = texture; }
		#pragma endregion
    };
}

#endif
