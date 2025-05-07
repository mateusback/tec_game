#include "../../include/scenes/MenuScene.h"
#include "../../include/managers/SceneManager.h"
#include "../../include/scenes/GameplayScene.h"
#include "../../include/renders/TextRenderer.h"
#include "../../include/utils/GlobalAccess.h"
#include "../../include/managers/FontManager.h"

#include <SDL2/SDL.h>
#include <iostream>

MenuScene::MenuScene(SDL_Renderer* renderer, int width, int height)
    : width(width), height(height) {
    Manager::FontManager::load("menu", "assets/fonts/Roboto-Regular.ttf", 26);
    textures()->Load(renderer, "logo", "assets/images/logo.png");

    this->logoTexture = textures()->Get("logo");
    this->font = Manager::FontManager::get("menu");

}

MenuScene::~MenuScene() {
    if (this->font) {
        Manager::FontManager::unload("menu");
        this->font = nullptr;
    }
    if (this->logoTexture) {
        textures()->Clear("logo");
        this->logoTexture = nullptr;
    }
}

void MenuScene::handleEvent(const SDL_Event& event) {
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_UP:
                selectedIndex = (selectedIndex - 1 + options.size()) % options.size();
                break;
            case SDLK_DOWN:
                selectedIndex = (selectedIndex + 1) % options.size();
                break;
            case SDLK_RETURN:
                if (options[selectedIndex] == "Iniciar Jogo") {
                    auto renderer = SDL_GetRenderer(SDL_GetWindowFromID(1));
                    Manager::SceneManager::setScene(new GameplayScene(renderer, this->width, this->height));
                } else if (options[selectedIndex] == "Sair") {
                    SDL_Event quit;
                    quit.type = SDL_QUIT;
                    SDL_PushEvent(&quit);
                }
                break;
        }
    }
}

void MenuScene::update(float deltaTime, const Manager::PlayerInput& input) {
    // Nada por enquanto
}

void MenuScene::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    if (this->logoTexture) {
        int originalW, originalH;
        SDL_QueryTexture(this->logoTexture, nullptr, nullptr, &originalW, &originalH);
        
        float logoWidthRatio = 0.65f;
        float logoHeightRatio = 0.35f;
        
        int logoW = static_cast<int>(this->width * logoWidthRatio);
        int logoH = static_cast<int>(this->height * logoHeightRatio);
        
        float originalRatio = static_cast<float>(originalW) / originalH;
        if (static_cast<float>(logoW) / logoH > originalRatio) {
            logoW = static_cast<int>(logoH * originalRatio);
        } else {
            logoH = static_cast<int>(logoW / originalRatio);
        }
        
        SDL_Rect dstRect;
        dstRect.x = (this->width - logoW) / 2;
        dstRect.y = static_cast<int>(this->height * 0.15f);
        dstRect.w = logoW;
        dstRect.h = logoH;
        
        SDL_RenderCopy(renderer, this->logoTexture, nullptr, &dstRect);
    }

    if(this->font){
        
        for (size_t i = 0; i < options.size(); ++i) {
            SDL_Color color = (i == selectedIndex) ? SDL_Color{0, 153, 51} : SDL_Color{214, 214, 194};
            
            int textWidth = 0, textHeight = 0;
            TTF_SizeText(this->font, options[i].c_str(), &textWidth, &textHeight);
            
            int x = (this->width - textWidth) / 2;
            int y = static_cast<int>(this->height * 0.7f) + static_cast<int>(i) * 50;
            
            Core::TextRenderer::render(renderer, this->font, options[i], x, y, color);
        }
    }
        
    SDL_RenderPresent(renderer);
}
