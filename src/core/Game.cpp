#include "../../include/core/Game.h"
#include "../../include/core/GameLoop.h"
#include "../../include/core/SceneManager.h"
#include "../../include/scenes/GameplayScene.h"
#include "../../include/core/TextRenderer.h"

#include <iostream>

namespace Core
{

    Game::Game() : window(nullptr), renderer(nullptr), loop(nullptr) {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) 
        {
            std::cerr << "Erro ao inicializar SDL: " << SDL_GetError() << std::endl;
            return false;
        }

        if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) 
        {
            std::cerr << "Erro ao iniciar SDL_image: " << IMG_GetError() << std::endl;
            return false;
        }

        if (TTF_Init() < 0) {
            std::cerr << "Erro SDL_ttf: " << TTF_GetError() << std::endl;
            return 1;
        }    

        window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                width, height, SDL_WINDOW_SHOWN);
        if (!window) 
        {
            std::cerr << "Erro ao criar janela: " << SDL_GetError() << std::endl;
            return false;
        }

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (!renderer) 
        {
            std::cerr << "Erro ao criar renderer: " << SDL_GetError() << std::endl;
            return false;
        }

        SceneManager::setScene(new GameplayScene(renderer));

        loop = new GameLoop(renderer);
        return true;
    }

    void Game::run() 
    {
        if (loop) loop->run();
    }
    
    Game::~Game() 
    {
        delete loop;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
    }

}
