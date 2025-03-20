#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "src/Entities/Interface/PlayerBody.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

using namespace std;
using Vector = Mylib::Math::Vector<float, 2>;

int main(int argc, char **argv) {
    cout << "hello world\n";
    SDL_Window* window = nullptr;
    SDL_Surface* winSurface = nullptr;

    SDL_Init(SDL_INIT_EVERYTHING);

    window = SDL_CreateWindow("Jogo real oficial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_SHOWN);
    winSurface = SDL_GetWindowSurface(window);    
    SDL_FillRect(winSurface, NULL, SDL_MapRGB(winSurface->format, 0, 255, 255));

    PlayerBody player(100, 100, 50, 50);
    player.setVelocity(40.0f);

    float speed = 40.0f;

    bool running = true;
    SDL_Event event;
    Uint32 oldTime = SDL_GetTicks();

    while (running) {
        Uint32 newTime = SDL_GetTicks();
        float delta = (newTime - oldTime) / 1000.0f;
        oldTime = newTime;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        const Uint8* state = SDL_GetKeyboardState(NULL);

        
        if (state[SDL_SCANCODE_W] || state[SDL_SCANCODE_UP])    player.move(EDirection::UP, delta);
        if (state[SDL_SCANCODE_S] || state[SDL_SCANCODE_DOWN])  player.move(EDirection::DOWN, delta);
        if (state[SDL_SCANCODE_A] || state[SDL_SCANCODE_LEFT])  player.move(EDirection::LEFT, delta);
        if (state[SDL_SCANCODE_D] || state[SDL_SCANCODE_RIGHT]) player.move(EDirection::RIGHT, delta);

        SDL_FillRect(winSurface, NULL, SDL_MapRGB(winSurface->format, 255, 255, 255));

        SDL_Rect rect = { static_cast<int>(player.getRect().x), static_cast<int>(player.getRect().y),
                          static_cast<int>(player.getRect().w), static_cast<int>(player.getRect().h) };
        
        SDL_FillRect(winSurface, &rect, SDL_MapRGB(winSurface->format, 0, 0, 0));

        SDL_UpdateWindowSurface(window);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
