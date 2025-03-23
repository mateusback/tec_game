#include "../Interface/Game.h"
#include <iostream>

Game::Game() : window(nullptr), renderer(nullptr), loop(nullptr) {}

Game::~Game() {
    Shutdown();
}

bool Game::Init(const char* title, int width, int height) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Erro ao inicializar SDL: " << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              width, height, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Erro ao criar janela: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Erro ao criar renderer: " << SDL_GetError() << std::endl;
        return false;
    }

    loop = new GameLoop(renderer);
    return true;
}

void Game::Run() {
    if (loop) loop->Run();
}

void Game::Shutdown() {
    delete loop;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
