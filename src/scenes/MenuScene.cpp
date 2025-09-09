#include "../../include/scenes/MenuScene.h"
#include "../../include/managers/SceneManager.h"
#include "../../include/scenes/GameplayScene.h"
#include "../../include/renders/TextRenderer.h"
#include "../../include/utils/GlobalAccess.h"
#include "../../include/managers/FontManager.h"
#include "../../include/scenes/CreditsScene.h"
#include "../../include/scenes/ScoresScene.h"
#include "../../include/scenes/OptionsScene.h"

#include <SDL2/SDL.h>
#include <iostream>

MenuScene::MenuScene(SDL_Renderer* renderer, int width, int height)
    : width(width), height(height)
{
    audio()->loadMusicIfNotLoaded("bg_menu", "assets/music/bg_menu.mp3");
    audio()->playMusicIfNotPlaying("bg_menu", -1);

    this->logoTexture = textures()->Get("logo");
    this->font = Manager::FontManager::get("menu");
}

MenuScene::~MenuScene()
{
    if (this->font)
    {
        Manager::FontManager::unload("menu");
        this->font = nullptr;
    }
    if (this->logoTexture)
    {
        textures()->Clear("logo");
        this->logoTexture = nullptr;
    }
}

void MenuScene::handleEvent(const SDL_Event& event)
{
    if (event.type == SDL_KEYDOWN)
    {
        switch (event.key.keysym.sym)
        {
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
                } else if (options[selectedIndex] == "Ver Pontuação") {
                    auto renderer = SDL_GetRenderer(SDL_GetWindowFromID(1));    
                    Manager::SceneManager::setScene(new Scenes::ScoresScene(renderer, this->width, this->height));
                } else if (options[selectedIndex] == "Ver Créditos") {
                    auto renderer = SDL_GetRenderer(SDL_GetWindowFromID(1));
                    Manager::SceneManager::setScene(new Scenes::CreditsScene(renderer, this->width, this->height));
                } else if (options[selectedIndex] == "Opções") {
                    auto renderer = SDL_GetRenderer(SDL_GetWindowFromID(1));
                    Manager::SceneManager::setScene(new OptionsScene(renderer, this->width, this->height));
                } else if (options[selectedIndex] == "Sair") {
                    SDL_Event quit;
                    quit.type = SDL_QUIT;
                    SDL_PushEvent(&quit);
                }
                break;
        }
    }
}

void MenuScene::update(float deltaTime, const Manager::PlayerInput& input){}

void MenuScene::render(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    if (!this->font) [[unlikely]]
    {
        std::cerr << "Font not loaded!" << std::endl;
        return;
    }

    if (!this->logoTexture) [[unlikely]]
    {
        std::cerr << "Logo texture not loaded!" << std::endl;
        return;
    }

    this->renderLogo(renderer);
    this->renderFont(renderer);

    SDL_RenderPresent(renderer);
}


void MenuScene::renderLogo(SDL_Renderer* renderer)
{
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

void MenuScene::renderFont(SDL_Renderer* renderer)
{
    const int spacing = 50;
    const int offsetAfterLogo = 90;

    const int startY = static_cast<int>(this->height * 0.5f) + offsetAfterLogo;

    for (size_t i = 0; i < options.size(); ++i)
    {
        SDL_Color color = (i == selectedIndex) ? SDL_Color{0, 153, 51} : SDL_Color{214, 214, 194};

        SDL_Surface* surface = TTF_RenderUTF8_Blended(this->font, options[i].c_str(), color);
        if (!surface) continue;

        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (!texture) {
            SDL_FreeSurface(surface);
            continue;
        }

        SDL_Rect dst;
        dst.w = surface->w;
        dst.h = surface->h;
        dst.x = (this->width - dst.w) / 2;
        dst.y = startY + static_cast<int>(i) * spacing;

        SDL_RenderCopy(renderer, texture, nullptr, &dst);

        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }
}
