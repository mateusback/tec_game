#ifndef SCENE_H
#define SCENE_H
#include <SDL2/SDL.h>

class Scene {
public:
    virtual ~Scene() = default;
    virtual void HandleEvent(const SDL_Event& event) = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void Render(SDL_Renderer* renderer) = 0;
};

#endif