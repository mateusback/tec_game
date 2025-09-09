#ifndef OPTIONSSCENE_H
#define OPTIONSSCENE_H

#include "../core/Scene.h"
#include <SDL2/SDL_ttf.h>

class OptionsScene : public Core::Scene {
public:
    OptionsScene(SDL_Renderer* renderer, int width, int height);
    ~OptionsScene();

    void handleEvent(const SDL_Event& event) override;
    void update(float deltaTime, const Manager::PlayerInput& input) override;
    void render(SDL_Renderer* renderer) override;

private:
    TTF_Font* font = nullptr;
    int width, height;
    size_t selectedIndex = 0;

    std::vector<std::string> options = {
        "Volume da Música",
        "Volume dos Efeitos",
        "Voltar"
    };

    void renderOptions(SDL_Renderer* renderer);
};

#endif
