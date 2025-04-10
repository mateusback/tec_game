#ifndef SCENE_H
#define SCENE_H

#include <SDL2/SDL.h>
#include "../managers/EntityManager.h"

namespace Core {
    class Scene {
    protected:
        EntityManager entityManager;
    public:
        virtual ~Scene() = default;
        virtual void handleEvent(const SDL_Event& event) = 0;
        virtual void update(float deltaTime) = 0;
        virtual void render(SDL_Renderer* renderer) = 0;
    };
    
}

#endif