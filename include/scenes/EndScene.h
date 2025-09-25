#ifndef END_SCENE_H
#define END_SCENE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../core/Scene.h"
#include "../managers/ScoreManager.h"

namespace Scenes {

    enum class EndSceneState {
        ShowingCredits,
        EnteringName,
        ShowingScores
    };

    class EndScene : public Core::Scene {
    public:
        EndScene(SDL_Renderer* renderer, int width, int height, int finalScore);
        ~EndScene();

        void handleEvent(const SDL_Event& event) override;
        void update(float deltaTime, const Manager::PlayerInput& input) override;
        void render(SDL_Renderer* renderer) override;

    private:
        int width, height;
        int finalScore = 0;

        TTF_Font* titleFont = nullptr;
        TTF_Font* textFont = nullptr;
        TTF_Font* smallFont = nullptr;
        EndSceneState state = EndSceneState::ShowingCredits;

        std::string playerName;
        std::vector<Manager::ScoreEntry> scores;
        bool scoreSaved = false;
        
        int getCenteredX(const char* text, TTF_Font* font);
    };
}

#endif // END_SCENE_H
