#ifndef SCENE_H
#define SCENE_H

#include <SDL2/SDL.h>
#include "../managers/EntityManager.h"
#include "../managers/InputManager.h"

namespace Core {
    
    class Scene {
    protected:
        Manager::EntityManager entityManager;
    public:
        virtual ~Scene() = default;
        virtual void handleEvent(const SDL_Event& event) = 0;
        virtual void update(float deltaTime, const Manager::PlayerInput& input) = 0;
        virtual void render(SDL_Renderer* renderer) = 0;
    };    
}

#endif