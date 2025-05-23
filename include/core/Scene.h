#ifndef SCENE_H
#define SCENE_H
#include <SDL2/SDL.h>

namespace Core 
{
    class Scene 
    {
    public:
        virtual ~Scene() = default;
        virtual void handleEvent(const SDL_Event& event) = 0;
        virtual void update(float deltaTime) = 0;
        virtual void render(SDL_Renderer* renderer) = 0;
    };
    
}

#endif