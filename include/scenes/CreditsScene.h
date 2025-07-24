#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../core/Scene.h"

namespace Scenes {

    class CreditsScene : public Core::Scene {
    public:
        CreditsScene(SDL_Renderer* renderer, int width, int height);
        ~CreditsScene();

        void handleEvent(const SDL_Event& event) override;
        void update(float deltaTime, const Manager::PlayerInput& input) override;
        void render(SDL_Renderer* renderer) override;

    private:
        int width, height;
        TTF_Font* font = nullptr;
    };
}
