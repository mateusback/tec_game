#include "../../include/core/Game.h"
#include "../../include/core/GameLoop.h"
#include "../../include/managers/SceneManager.h"
#include "../../include/scenes/GameplayScene.h"
#include "../../include/renders/TextRenderer.h"
#include "../../include/renders/VirtualRendererGlobal.h"
#include "../../include/managers/TextureManagerGlobal.h"
#include "../../include/managers/AudioManagerGlobal.h"

#include <iostream>

namespace Core
{
    Game::Game(const char* title, int width, int height) : window(nullptr), renderer(nullptr), loop(nullptr) {

        #pragma region SDL_Init
        if (SDL_Init(SDL_INIT_VIDEO) < 0) 
        {
            std::cerr << "Erro ao inicializar SDL: " << SDL_GetError() << std::endl;
            return;
        }

        if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) 
        {
            std::cerr << "Erro ao iniciar SDL_image: " << IMG_GetError() << std::endl;
            return;
        }

        if (TTF_Init() < 0) {
            std::cerr << "Erro SDL_ttf: " << TTF_GetError() << std::endl;
            return;
        }    

        if (SDL_Init(SDL_INIT_AUDIO) < 0) {
            std::cout << "Erro SDL_Init: " << SDL_GetError() << "\n";
            return;
        }

        this->window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                width, height, SDL_WINDOW_SHOWN);
        if (!this->window) 
        {
            std::cerr << "Erro ao criar janela: " << SDL_GetError() << std::endl;
            return;
        }

        this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED);
        if (!this->renderer) 
        {
            std::cerr << "Erro ao criar renderer: " << SDL_GetError() << std::endl;
            return;
        }

        #pragma endregion

        Renderer::VirtualRendererGlobal::init(width, height, 1, 1);
        Manager::TextureManagerGlobal::init();
        Manager::AudioManagerGlobal::init();
        Manager::ScoreManagerGlobal::init();

        Manager::SceneManager::setScene(new GameplayScene(renderer, width, height));

        this->loop = new GameLoop(renderer);
    }

    void Game::run() 
    {
        if (!this->loop) 
        {
            std::cerr << "Game loop not initialized." << std::endl;
            return;
        }
        if (this->loop) 
            this->loop->run();
    }
    
    Game::~Game() 
    {
        Renderer::VirtualRendererGlobal::destroy();
        Manager::TextureManagerGlobal::destroy();
        Manager::AudioManagerGlobal::destroy();
        Manager::ScoreManagerGlobal::destroy();
        delete this->loop;
        SDL_DestroyRenderer(this->renderer);
        SDL_DestroyWindow(this->window);
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
    }

}
