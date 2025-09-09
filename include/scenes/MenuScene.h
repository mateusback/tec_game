#ifndef MENU_SCENE_H
#define MENU_SCENE_H

#include "../core/Scene.h"
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <string>

class MenuScene : public Core::Scene {
private:
    std::vector<std::string> options = {
        "Iniciar Jogo",
        "Ver Pontuação",
        "Ver Créditos",
        "Opções",
        "Sair"
    };
    size_t selectedIndex = 0;
    TTF_Font* font = nullptr;
    SDL_Texture* logoTexture = nullptr;
    int width, height;

    void renderLogo(SDL_Renderer* renderer);
    void renderFont(SDL_Renderer* renderer);
public:
    MenuScene(SDL_Renderer* renderer, int width, int height);
    ~MenuScene();

    void handleEvent(const SDL_Event& event) override;
    void update(float deltaTime, const Manager::PlayerInput& input) override;
    void render(SDL_Renderer* renderer) override;
};

#endif
