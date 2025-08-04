#ifndef GAME_OVER_SCENE_H
#define GAME_OVER_SCENE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../core/Scene.h"

namespace Scenes {

    class GameOverScene : public Core::Scene {
    public:
        GameOverScene(SDL_Renderer* renderer, int width, int height);
        ~GameOverScene();

        void handleEvent(const SDL_Event& event) override;
        void update(float deltaTime, const Manager::PlayerInput& input) override;
        void render(SDL_Renderer* renderer) override;

    private:
        int width, height;
        TTF_Font* font = nullptr;
        bool returnToMenu = false;
    };

}

#endif // GAME_OVER_SCENE_H