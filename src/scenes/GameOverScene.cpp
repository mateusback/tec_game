#include "../../include/scenes/GameOverScene.h"
#include "../../include/renders/TextRenderer.h"
#include "../../include/managers/FontManager.h"
#include "../../include/managers/SceneManager.h"
#include "../../include/scenes/MenuScene.h"

namespace Scenes {

    GameOverScene::GameOverScene(SDL_Renderer* renderer, int width, int height)
        : width(width), height(height)
    {
        Manager::FontManager::load("gameover", "assets/fonts/Roboto-Regular.ttf", 64);
        this->font = Manager::FontManager::get("gameover");
    }

    GameOverScene::~GameOverScene() {
        Manager::FontManager::unload("gameover");
    }

    void GameOverScene::handleEvent(const SDL_Event& event) {
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) {
            returnToMenu = true;
        }
    }

    void GameOverScene::update(float, const Manager::PlayerInput&) {
        if (returnToMenu) {
            Manager::SceneManager::setScene(new MenuScene(SDL_GetRenderer(SDL_GetWindowFromID(1)), width, height));
        }
    }

    void GameOverScene::render(SDL_Renderer* renderer) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        if (!font) return;

        SDL_Color red = {180, 0, 0, 255};
        SDL_Color white = {255, 255, 255, 255};

        Core::TextRenderer::render(renderer, font, "Você morreu!", width / 2, height / 2 - 80, red, true);
        Core::TextRenderer::render(renderer, font, "Pressione Enter para voltar ao menu", width / 2, height / 2 + 40, white, true);

        SDL_RenderPresent(renderer);
    }

}
