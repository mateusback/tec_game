#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../core/Scene.h"
#include "../managers/InputManager.h"
#include "../managers/ScoreManager.h"

namespace Scenes {

    class ScoresScene : public Core::Scene {
    public:
        ScoresScene(SDL_Renderer* renderer, int width, int height);
        ~ScoresScene();

        void handleEvent(const SDL_Event& event) override;
        void update(float deltaTime, const Manager::PlayerInput& input) override;
        void render(SDL_Renderer* renderer) override;

    private:
        int width, height;
        TTF_Font* titleFont = nullptr;
        TTF_Font* textFont = nullptr;
        TTF_Font* smallFont = nullptr;
        std::vector<Manager::ScoreEntry> scores;
        
        int getCenteredX(const char* text, TTF_Font* font);
    };
}
