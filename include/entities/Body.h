#ifndef BODY_H
#define BODY_H

#include <SDL2/SDL.h>
#include <my-lib/math-vector.h>
#include <nlohmann/json.hpp>

#include "Entity.h"

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
        SDL_Texture* texture = nullptr;
        bool has_collision;
        bool is_visible;
        
        Body(EBodyType type, float x, float y, float w, float h, bool collision = false, bool visible = true)
        : Entity(type), position(x, y), scale(w, h), has_collision(collision), is_visible(visible) {}

    public:
        #pragma region Constructors
        Body() : Entity(EBodyType::Body), position(0, 0), scale(50, 50), has_collision(false), is_visible(true) {}

        Body(float x = 0, float y = 0, float w = 50, float h = 50, bool collision = false, bool visible = true)
            : Entity(EBodyType::Body), position(x, y), scale(w, h), has_collision(collision), is_visible(visible) {}
        
        Body(Vector pos, Vector scl, bool collision = false, bool visible = true)
            : Entity(EBodyType::Body), position(pos), scale(scl), has_collision(collision), is_visible(visible) {}
        
        Body(Vector4 collider, bool collision = false, bool visible = true)
            : Entity(EBodyType::Body), position(collider.x, collider.y), scale(collider.z, collider.w), has_collision(collision), is_visible(visible) {}
        #pragma endregion
		
        virtual void update(float deltaTime) = 0;
		virtual void render(SDL_Renderer* renderer);

        Vector4 getCollider() const { return Vector4(this->position.x, this->position.y, this->scale.x, this->scale.y); }
        Point getCenterPoint() const { return Point(this->position.x + this->scale.x / 2, this->position.y + this->scale.y / 2); }
        virtual EBodyType getBodyType() const {return EBodyType::Body;}
        
        virtual void onCollision(Body* other) {};

		#pragma region Getters
        bool hasCollision() const { return this->has_collision; }
        bool isVisible() const { return this->is_visible; }
		#pragma endregion

		#pragma region Setters
        void setCollision(bool collision) { this->has_collision = collision; }
        void setVisible(bool visible) { this->is_visible = visible; }
		void setTexture(SDL_Texture* tex) { this->texture = tex; }
        void setPosition(float x, float y) { this->position.x = x; this->position.y = y; }
        void setPosition(Vector pos) { this->position = pos; }
        void setScale(float w, float h) { this->scale.x = w; this->scale.y = h; }
        void setScale(Vector scl) { this->scale = scl; }
		#pragma endregion
    };
}

#endif
