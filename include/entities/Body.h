#ifndef BODY_H
#define BODY_H

#include <SDL2/SDL.h>
#include <my-lib/math-vector.h>

enum class BodyType { Body, Player, Item, Enemy, Attack };
using Vector = Mylib::Math::Vector<float, 2>;
using Vector4 = Mylib::Math::Vector<float, 4>;
using Point = Vector;

namespace Entities
{
    class Body 
    {
    protected:
        //TODO - Possivel reaftoração para usar Vector
        SDL_FRect rect;
        SDL_Texture* texture = nullptr;
        bool has_collision;
        bool is_visible;

    public:
        void setCollision(bool collision) { this->has_collision = collision; }
        void setVisible(bool visible) { this->is_visible = visible; }

        bool hasCollision() const { return this->has_collision; }
        bool isVisible() const { return this->is_visible; }

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

        virtual void update(float deltaTime) = 0;
        virtual void render(SDL_Renderer* renderer);

        SDL_FRect getCollider() const { return this->rect; }

        SDL_Rect getIntRect() const {
            return { 
                static_cast<int>(rect.x), 
                static_cast<int>(rect.y), 
                static_cast<int>(rect.w), 
                static_cast<int>(rect.h)
            };
        }

        void setTexture(SDL_Texture* tex) {
            this->texture = tex;
        }

        Point getCenterPoint() const {
            return Point(this->rect.x + this->rect.w / 2, this->rect.y + this->rect.h / 2);
        }

        virtual void onCollision(Body* other) {};
        virtual BodyType getBodyType() const {return BodyType::Body;}

    };
}

#endif
