#include "../../include/scenes/OptionsScene.h"
#include "../../include/scenes/MenuScene.h"
#include "../../include/managers/AudioManager.h"
#include "../../include/managers/SceneManager.h"
#include "../../include/utils/GlobalAccess.h"
#include "../../include/managers/FontManager.h"

#include <iostream>

OptionsScene::OptionsScene(SDL_Renderer* renderer, int width, int height)
    : width(width), height(height)
{
    Manager::FontManager::load("options", "assets/fonts/Roboto-Regular.ttf", 24);
    this->font = Manager::FontManager::get("options");
}

OptionsScene::~OptionsScene()
{
    if (this->font) {
        Manager::FontManager::unload("options");
        this->font = nullptr;
    }
}

void OptionsScene::handleEvent(const SDL_Event& event)
{
    if (event.type != SDL_KEYDOWN) return;

    switch (event.key.keysym.sym) {
        case SDLK_UP:
            selectedIndex = (selectedIndex - 1 + options.size()) % options.size();
            break;
        case SDLK_DOWN:
            selectedIndex = (selectedIndex + 1) % options.size();
            break;
        case SDLK_LEFT:
        case SDLK_RIGHT: {
            const std::string& selected = options[selectedIndex];
            int step = (event.key.keysym.sym == SDLK_RIGHT) ? 8 : -8;

            if (selected == "Volume da Música") {
                int vol = Mix_VolumeMusic(-1);
                Mix_VolumeMusic(std::clamp(vol + step, 0, 128));
            } else if (selected == "Volume dos Efeitos") {
                int vol = Mix_Volume(-1, -1);
                Mix_Volume(-1, std::clamp(vol + step, 0, 128));
            }
            break;
        }
        case SDLK_RETURN: {
            if (options[selectedIndex] == "Voltar") {
                auto renderer = SDL_GetRenderer(SDL_GetWindowFromID(1));
                Manager::SceneManager::setScene(new MenuScene(renderer, this->width, this->height));
            }
            break;
        }
    }
}

void OptionsScene::update(float deltaTime, const Manager::PlayerInput&) {}

void OptionsScene::render(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 10, 10, 10, 255);
    SDL_RenderClear(renderer);

    renderOptions(renderer);
    SDL_RenderPresent(renderer);
}

void OptionsScene::renderOptions(SDL_Renderer* renderer)
{
    const int spacing = 60;
    const int startY = static_cast<int>(this->height * 0.3f);
    const int barWidth = 200;
    const int barHeight = 14;

    for (size_t i = 0; i < options.size(); ++i) {
        const std::string& option = options[i];
        SDL_Color color = (i == selectedIndex) ? SDL_Color{255, 255, 0} : SDL_Color{200, 200, 200};

        SDL_Surface* surface = TTF_RenderUTF8_Blended(this->font, option.c_str(), color);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

        SDL_Rect dst {
            .x = (this->width - surface->w) / 2,
            .y = startY + static_cast<int>(i * spacing),
            .w = surface->w,
            .h = surface->h
        };

        SDL_RenderCopy(renderer, texture, nullptr, &dst);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);

        if (option == "Volume da Música" || option == "Volume dos Efeitos") {
            int volume = (option == "Volume da Música") ? Mix_VolumeMusic(-1) : Mix_Volume(-1, -1);
            int filledW = static_cast<int>((volume / 128.0f) * barWidth);

            SDL_Rect barBg = {
                (this->width - barWidth) / 2,
                dst.y + dst.h + 8,
                barWidth,
                barHeight
            };
            SDL_Rect barFill = barBg;
            barFill.w = filledW;

            SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
            SDL_RenderFillRect(renderer, &barBg);

            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            SDL_RenderFillRect(renderer, &barFill);
        }
    }
}
