#include "../../include/scenes/EndScene.h"
#include "../../include/renders/TextRenderer.h"
#include "../../include/managers/FontManager.h"
#include "../../include/managers/SceneManager.h"
#include "../../include/scenes/MenuScene.h"
#include "../../include/utils/GlobalAccess.h"

#include <algorithm>
#include <iostream>

namespace Scenes {

    EndScene::EndScene(SDL_Renderer* renderer, int width, int height, int finalScore)
        : width(width), height(height), finalScore(finalScore)
    {
        Manager::FontManager::load("end", "assets/fonts/Roboto-Regular.ttf", 28);
        this->font = Manager::FontManager::get("end");
        SDL_StartTextInput();
    }

    EndScene::~EndScene() {
        SDL_StopTextInput();
        Manager::FontManager::unload("end");
    }

    void EndScene::handleEvent(const SDL_Event& event) {
        switch (state) {
            case EndSceneState::ShowingCredits:
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) {
                    state = EndSceneState::EnteringName;
                }
                break;

            case EndSceneState::EnteringName:
                if (event.type == SDL_TEXTINPUT) {
                    if (playerName.size() < 20) {
                        playerName += event.text.text;
                    }
                } else if (event.type == SDL_KEYDOWN) {
                    if (event.key.keysym.sym == SDLK_BACKSPACE && !playerName.empty()) {
                        playerName.pop_back();
                    } else if (event.key.keysym.sym == SDLK_RETURN && !playerName.empty()) {
                        if (!scoreSaved) {
                            score()->save({playerName, finalScore}); 
                            scores = score()->load();
                            scoreSaved = true;
                        }
                        state = EndSceneState::ShowingScores;
                    }
                }
                break;

            case EndSceneState::ShowingScores:
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) {
                    auto renderer = SDL_GetRenderer(SDL_GetWindowFromID(1));
                    Manager::SceneManager::setScene(new MenuScene(renderer, width, height));
                }
                break;
        }
    }

    void EndScene::update(float, const Manager::PlayerInput&) {
    }

    void EndScene::render(SDL_Renderer* renderer) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        std::cout << "Rendering End Scene" << std::endl;
        SDL_Color color = {255, 255, 255};

        if (!font) return;

        switch (state) {
            case EndSceneState::ShowingCredits:
                Core::TextRenderer::render(renderer, font, "Você venceu!", width / 2 - 100, height / 2 - 100, color);
                Core::TextRenderer::render(renderer, font, "Pressione Enter para continuar", width / 2 - 160, height / 2 + 10, color);
                break;

            case EndSceneState::EnteringName:
                Core::TextRenderer::render(renderer, font, "Digite seu nome para salvar o score:", width / 2 - 220, height / 2 - 60, color);
                Core::TextRenderer::render(renderer, font, playerName + "_", width / 2 - 100, height / 2, color);
                Core::TextRenderer::render(renderer, font, "Pressione Enter para confirmar", width / 2 - 160, height / 2 + 50, color);
                break;

            case EndSceneState::ShowingScores:
                Core::TextRenderer::render(renderer, font, "Ranking de Pontuação", width / 2 - 140, height / 2 - 120, color);
                for (size_t i = 0; i < std::min(scores.size(), size_t(5)); ++i) {
                    const auto& entry = scores[i];
                    std::string line = std::to_string(i + 1) + ". " + entry.name + " - " + std::to_string(entry.score);
                    Core::TextRenderer::render(renderer, font, line, width / 2 - 150, height / 2 - 70 + static_cast<int>(i) * 35, color);
                }
                Core::TextRenderer::render(renderer, font, "Pressione Enter para voltar ao menu", width / 2 - 180, height / 2 + 120, color);
                break;
        }

        SDL_RenderPresent(renderer);
    }

}
