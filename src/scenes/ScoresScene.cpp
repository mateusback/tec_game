#include "../../include/scenes/ScoresScene.h"
#include "../../include/utils/GlobalAccess.h"
#include "../../include/managers/FontManager.h"
#include "../../include/managers/SceneManager.h"
#include "../../include/scenes/MenuScene.h"
#include "../../include/renders/TextRenderer.h"

namespace Scenes {

    ScoresScene::ScoresScene(SDL_Renderer* renderer, int width, int height)
        : width(width), height(height)
    {
        Manager::FontManager::load("scores", "assets/fonts/Roboto-Regular.ttf", 28);
        font = Manager::FontManager::get("scores");

        scores = score()->load();
    }

    ScoresScene::~ScoresScene() {
        Manager::FontManager::unload("scores");
    }

    void ScoresScene::handleEvent(const SDL_Event& event) {
        if (event.type == SDL_KEYDOWN && (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_ESCAPE)) {
            auto renderer = SDL_GetRenderer(SDL_GetWindowFromID(1));
            Manager::SceneManager::setScene(new MenuScene(renderer, width, height));
        }
    }

    void ScoresScene::update(float, const Manager::PlayerInput&) {
        // Nada
    }

    void ScoresScene::render(SDL_Renderer* renderer) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_Color white = {255, 255, 255};

        Core::TextRenderer::render(renderer, font, "Ranking de Pontuação", width / 2 - 140, height / 2 - 150, white);

        int y = height / 2 - 100;
        size_t max = std::min(scores.size(), size_t(10));

        for (size_t i = 0; i < max; ++i) {
            const auto& entry = scores[i];
            std::string line = std::to_string(i + 1) + ". " + entry.name + " - " + std::to_string(entry.score);
            Core::TextRenderer::render(renderer, font, line, width / 2 - 150, y, white);
            y += 35;
        }

        Core::TextRenderer::render(renderer, font, "Pressione Enter ou ESC para voltar", width / 2 - 200, y + 40, white);

        SDL_RenderPresent(renderer);
    }

}
