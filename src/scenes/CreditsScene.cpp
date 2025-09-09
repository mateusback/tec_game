#include "../../include/scenes/CreditsScene.h"
#include "../../include/utils/GlobalAccess.h"
#include "../../include/managers/FontManager.h"
#include "../../include/managers/SceneManager.h"
#include "../../include/scenes/MenuScene.h"
#include "../../include/renders/TextRenderer.h"

namespace Scenes {

    CreditsScene::CreditsScene(SDL_Renderer* renderer, int width, int height)
        : width(width), height(height)
    {
        Manager::FontManager::load("credits", "assets/fonts/Roboto-Regular.ttf", 28);
        font = Manager::FontManager::get("credits");
    }

    CreditsScene::~CreditsScene() {
        Manager::FontManager::unload("credits");
    }

    void CreditsScene::handleEvent(const SDL_Event& event) {
        if (event.type == SDL_KEYDOWN && (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_ESCAPE)) {
            auto renderer = SDL_GetRenderer(SDL_GetWindowFromID(1));
            Manager::SceneManager::setScene(new MenuScene(renderer, width, height));
        }
    }

    void CreditsScene::update(float, const Manager::PlayerInput&) {
        // Nada
    }

    void CreditsScene::render(SDL_Renderer* renderer) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_Color white = {255, 255, 255};

        Core::TextRenderer::render(renderer, font, "Créditos", width / 2 - 60, height / 2 - 100, white);
        Core::TextRenderer::render(renderer, font, "Jogo desenvolvido por Mateus Back", width / 2 - 180, height / 2 - 50, white);
        Core::TextRenderer::render(renderer, font, "Feito com SDL2 e C++", width / 2 - 110, height / 2, white);
        Core::TextRenderer::render(renderer, font, "Pressione Enter ou ESC para voltar", width / 2 - 200, height / 2 + 80, white);

        SDL_RenderPresent(renderer);
    }

}
