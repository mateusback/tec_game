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
        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_RETURN:
                case SDLK_ESCAPE:
                case SDLK_SPACE:
                case SDLK_BACKSPACE:
                    {
                        auto renderer = SDL_GetRenderer(SDL_GetWindowFromID(1));
                        Manager::SceneManager::setScene(new MenuScene(renderer, width, height));
                    }
                    break;
            }
        }
        
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            auto renderer = SDL_GetRenderer(SDL_GetWindowFromID(1));
            Manager::SceneManager::setScene(new MenuScene(renderer, width, height));
        }
    }

    void CreditsScene::update(float, const Manager::PlayerInput&) {
    }

    void CreditsScene::render(SDL_Renderer* renderer) {
        if (!renderer || !font) {
            return;
        }

        for (int y = 0; y < height; y++) {
            int r = 10 + (y * 20) / height;
            int g = 15 + (y * 25) / height;
            int b = 30 + (y * 40) / height;
            SDL_SetRenderDrawColor(renderer, r, g, b, 255);
            SDL_RenderDrawLine(renderer, 0, y, width, y);
        }

        // Cores seguras
        SDL_Color gold = {255, 215, 0, 255};
        SDL_Color lightBlue = {173, 216, 230, 255};
        SDL_Color white = {255, 255, 255, 255};
        SDL_Color lightGray = {200, 200, 200, 255};
        SDL_Color darkGray = {150, 150, 150, 255};

        int centerX = width / 2;
        int startY = height / 5;
        int currentY = startY;

        auto getCenteredX = [this](const char* text) -> int {
            int textWidth, textHeight;
            TTF_SizeText(font, text, &textWidth, &textHeight);
            return (width - textWidth) / 2;
        };

        const char* title = "CREDITOS";
        Core::TextRenderer::render(renderer, font, title, 
                                 getCenteredX(title), currentY, gold);
        currentY += 60;

        SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255);
        SDL_RenderDrawLine(renderer, centerX - 100, currentY, centerX + 100, currentY);
        currentY += 50;

        const char* devTitle = "DESENVOLVIDO POR";
        Core::TextRenderer::render(renderer, font, devTitle, 
                                 getCenteredX(devTitle), currentY, lightBlue);
        currentY += 50;

        const char* dev1 = "Mateus Back";
        Core::TextRenderer::render(renderer, font, dev1, 
                                 getCenteredX(dev1), currentY, white);
        currentY += 35;
        
        const char* dev2 = "Gustavo Lopes";
        Core::TextRenderer::render(renderer, font, dev2, 
                                 getCenteredX(dev2), currentY, white);
        currentY += 60;

        const char* techTitle = "TECNOLOGIAS";
        Core::TextRenderer::render(renderer, font, techTitle, 
                                 getCenteredX(techTitle), currentY, lightBlue);
        currentY += 50;

        const char* tech1 = "SDL2 - Graphics & Audio";
        Core::TextRenderer::render(renderer, font, tech1, 
                                 getCenteredX(tech1), currentY, lightGray);
        currentY += 35;
        
        const char* tech2 = "C++ - Programming Language";
        Core::TextRenderer::render(renderer, font, tech2, 
                                 getCenteredX(tech2), currentY, lightGray);
        currentY += 35;
        
        const char* tech3 = "nlohmann/json - Data Handling";
        Core::TextRenderer::render(renderer, font, tech3, 
                                 getCenteredX(tech3), currentY, lightGray);
        currentY += 60;

        const char* thanksTitle = "AGRADECIMENTOS";
        Core::TextRenderer::render(renderer, font, thanksTitle, 
                                 getCenteredX(thanksTitle), currentY, lightBlue);
        currentY += 50;

        const char* thanks1 = "Professores e colegas da faculdade";
        Core::TextRenderer::render(renderer, font, thanks1, 
                                 getCenteredX(thanks1), currentY, lightGray);
        currentY += 35;
        
        const char* thanks2 = "Comunidade SDL2 e C++";
        Core::TextRenderer::render(renderer, font, thanks2, 
                                 getCenteredX(thanks2), currentY, lightGray);

        const char* instruction = "Pressione ENTER ou ESC para voltar ao menu";
        Core::TextRenderer::render(renderer, font, instruction, 
                                 getCenteredX(instruction), height - 60, white);

        const char* copyright = "© 2025";
        Core::TextRenderer::render(renderer, font, copyright, 
                                 getCenteredX(copyright), height - 30, darkGray);

        SDL_RenderPresent(renderer);
    }

}
