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
        Vector4 rect;
        SDL_Texture* texture = nullptr;
        bool has_collision;
        bool is_visible;

    public:
        Body(float x = 0, float y = 0, float w = 0, float h = 0, bool collision = false, bool visible = true)
        : has_collision(collision), is_visible(visible) {
            this->rect.x = x;
            this->rect.y = y;
            this->rect.w = w;
            this->rect.h = h;
        }

        Body(SDL_FRect rect, bool collision = false, bool visible = true)
        : rect(rect), has_collision(collision), is_visible(visible) {}

        Body(SDL_FPoint position, SDL_FPoint size, bool collision = false, bool visible = true)
        : has_collision(collision), is_visible(visible) {
            this->rect.x = position.x;
            this->rect.y = position.y;
            this->rect.w = size.x;
            this->rect.h = size.y;
        }

        SDL_Rect getIntRect() const {
            return { 
                static_cast<int>(rect.x), 
                static_cast<int>(rect.y), 
                static_cast<int>(rect.w), 
                static_cast<int>(rect.h)
            };
        }

        virtual void onCollision(Body* other) {};
        virtual BodyType getBodyType() const {return BodyType::Body;}

		#pragma region Getters
        bool hasCollision() const { return this->has_collision; }
        bool isVisible() const { return this->is_visible; }
        bool isActive() const { return this->is_active; }
		Vector4 getCollider() const { return this->rect; }
		Vector4 getRect() const { return this->rect; }
        Point getCenterPoint() const { return Point(this->rect.x + this->rect.w / 2, this->rect.y + this->rect.h / 2); }
		#pragma endregion

		#pragma region Setters
        void setCollision(bool collision) { this->has_collision = collision; }
        void setVisible(bool visible) { this->is_visible = visible; }
		void setTexture(SDL_Texture* tex) { this->texture = tex; }
		#pragma endregion
    };
}

#endif
