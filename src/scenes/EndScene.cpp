#include "../../include/scenes/EndScene.h"
#include "../../include/renders/TextRenderer.h"
#include "../../include/managers/FontManager.h"
#include "../../include/managers/SceneManager.h"
#include "../../include/scenes/MenuScene.h"
#include "../../include/utils/GlobalAccess.h"

#include <algorithm>
#include <iostream>

namespace Scenes {

    int EndScene::getCenteredX(const char* text, TTF_Font* font) {
        int textWidth, textHeight;
        TTF_SizeText(font, text, &textWidth, &textHeight);
        return (width - textWidth) / 2;
    }

    EndScene::EndScene(SDL_Renderer* renderer, int width, int height, int finalScore)
        : width(width), height(height), finalScore(finalScore)
    {
        Manager::FontManager::load("end_title", "assets/fonts/Montserrat-Bold.ttf", 48);
        Manager::FontManager::load("end_text", "assets/fonts/Roboto-Regular.ttf", 24);
        Manager::FontManager::load("end_small", "assets/fonts/Roboto-Regular.ttf", 18);
        
        this->titleFont = Manager::FontManager::get("end_title");
        this->textFont = Manager::FontManager::get("end_text");
        this->smallFont = Manager::FontManager::get("end_small");
        SDL_StartTextInput();
    }

    EndScene::~EndScene() {
        SDL_StopTextInput();
        Manager::FontManager::unload("end_title");
        Manager::FontManager::unload("end_text");
        Manager::FontManager::unload("end_small");
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
        for (int y = 0; y < height; y++) {
            int r = 5 + (y * 15) / height;
            int g = 10 + (y * 20) / height; 
            int b = 40 + (y * 35) / height;
            SDL_SetRenderDrawColor(renderer, r, g, b, 255);
            SDL_RenderDrawLine(renderer, 0, y, width, y);
        }

        SDL_Color gold = {255, 215, 0, 255};
        SDL_Color lightGreen = {144, 238, 144, 255};
        SDL_Color lightBlue = {173, 216, 230, 255};
        SDL_Color white = {255, 255, 255, 255};
        SDL_Color lightGray = {200, 200, 200, 255};

        if (!titleFont || !textFont || !smallFont) return;

        int centerX = width / 2;

        switch (state) {
            case EndSceneState::ShowingCredits:
                {
                    int startY = height / 3;
                    
                    const char* victory = "PARABÉNS!";
                    Core::TextRenderer::render(renderer, titleFont, victory, 
                                             getCenteredX(victory, titleFont), startY, gold);
                    
                    SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255);
                    SDL_RenderDrawLine(renderer, centerX - 120, startY + 70, centerX + 120, startY + 70);
                    
                    const char* winMsg = "Você completou o jogo!";
                    Core::TextRenderer::render(renderer, textFont, winMsg, 
                                             getCenteredX(winMsg, textFont), startY + 100, lightGreen);
                    
                    std::string scoreText = "Pontuação Final: " + std::to_string(finalScore);
                    Core::TextRenderer::render(renderer, textFont, scoreText.c_str(), 
                                             getCenteredX(scoreText.c_str(), textFont), startY + 140, white);
                    
                    const char* instruction = "Pressione ENTER para continuar";
                    Core::TextRenderer::render(renderer, smallFont, instruction, 
                                             getCenteredX(instruction, smallFont), startY + 200, lightGray);
                }
                break;

            case EndSceneState::EnteringName:
                {
                    int startY = height / 3;
                    
                    const char* title = "SALVAR PONTUAÇÃO";
                    Core::TextRenderer::render(renderer, titleFont, title, 
                                             getCenteredX(title, titleFont), startY, lightBlue);
                    
                    SDL_SetRenderDrawColor(renderer, 173, 216, 230, 255);
                    SDL_RenderDrawLine(renderer, centerX - 150, startY + 70, centerX + 150, startY + 70);
                    
                    const char* instruction = "Digite seu nome:";
                    Core::TextRenderer::render(renderer, textFont, instruction, 
                                             getCenteredX(instruction, textFont), startY + 100, white);
                    
                    SDL_Rect inputBox = {centerX - 150, startY + 140, 300, 40};
                    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
                    SDL_RenderFillRect(renderer, &inputBox);
                    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
                    SDL_RenderDrawRect(renderer, &inputBox);
                    
                    std::string displayName = playerName + "_";
                    Core::TextRenderer::render(renderer, textFont, displayName.c_str(), 
                                             getCenteredX(displayName.c_str(), textFont), startY + 150, white);
                    
                    const char* confirm = "Pressione ENTER para confirmar";
                    Core::TextRenderer::render(renderer, smallFont, confirm, 
                                             getCenteredX(confirm, smallFont), startY + 220, lightGray);
                }
                break;

            case EndSceneState::ShowingScores:
                {
                    int startY = height / 6;
                    
                    const char* title = "RANKING";
                    Core::TextRenderer::render(renderer, titleFont, title, 
                                             getCenteredX(title, titleFont), startY, gold);
                    
                    SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255);
                    SDL_RenderDrawLine(renderer, centerX - 100, startY + 70, centerX + 100, startY + 70);
                    
                    const char* header = "TOP 5 PONTUAÇÕES";
                    Core::TextRenderer::render(renderer, textFont, header, 
                                             getCenteredX(header, textFont), startY + 100, lightBlue);
                    
                    for (size_t i = 0; i < std::min(scores.size(), size_t(5)); ++i) {
                        const auto& entry = scores[i];
                        
                        std::string position = std::to_string(i + 1) + ".";
                        Core::TextRenderer::render(renderer, textFont, position.c_str(), 
                                                 centerX - 200, startY + 150 + static_cast<int>(i) * 40, 
                                                 i == 0 ? gold : (i < 3 ? lightGreen : white));
                        
                        Core::TextRenderer::render(renderer, textFont, entry.name.c_str(), 
                                                 centerX - 150, startY + 150 + static_cast<int>(i) * 40, 
                                                 i == 0 ? gold : (i < 3 ? lightGreen : white));
                        
                        std::string scoreStr = std::to_string(entry.score);
                        Core::TextRenderer::render(renderer, textFont, scoreStr.c_str(), 
                                                 centerX + 50, startY + 150 + static_cast<int>(i) * 40, 
                                                 i == 0 ? gold : (i < 3 ? lightGreen : white));
                    }
                    
                    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
                    SDL_RenderDrawLine(renderer, centerX - 200, startY + 360, centerX + 200, startY + 360);
                    
                    const char* instruction = "Pressione ENTER para voltar ao menu";
                    Core::TextRenderer::render(renderer, smallFont, instruction, 
                                             getCenteredX(instruction, smallFont), startY + 380, lightGray);
                }
                break;
        }

        SDL_RenderPresent(renderer);
    }

}
