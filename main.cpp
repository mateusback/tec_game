#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <SDL2/SDL.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

using namespace std;

int main(int argc, char **argv) {
    cout << "hello world\n";
    SDL_Window* window = nullptr;
    SDL_Surface* winSurface = nullptr;

    SDL_Init(SDL_INIT_EVERYTHING);

    window = SDL_CreateWindow("Testeeee", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_SHOWN);
    winSurface = SDL_GetWindowSurface(window);    
    SDL_FillRect(winSurface, NULL, SDL_MapRGB(winSurface->format, 0, 255, 255));

    SDL_Rect rect = { 0, 0, 100, 100 };

    float speed = 0.2f;

    SDL_FillRect(winSurface, &rect, SDL_MapRGB(winSurface->format, 255, 0, 0));

    bool running = true;
    SDL_Event event;
    int x, y;
    Uint32 oldTime = SDL_GetTicks();
    while (running)
    {
        Uint32 newTime = SDL_GetTicks();
        float delta = (newTime - oldTime) / 1000.0f;
        oldTime = newTime;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        const Uint8* state = SDL_GetKeyboardState(NULL);

        if (state[SDL_SCANCODE_W] || state[SDL_SCANCODE_UP])    rect.y -= speed * delta;
        if (state[SDL_SCANCODE_S] || state[SDL_SCANCODE_DOWN])  rect.y += speed * delta;
        if (state[SDL_SCANCODE_A] || state[SDL_SCANCODE_LEFT])  rect.x -= speed * delta;
        if (state[SDL_SCANCODE_D] || state[SDL_SCANCODE_RIGHT]) rect.x += speed * delta;

        SDL_FillRect(winSurface, NULL, SDL_MapRGB(winSurface->format, 255, 255, 255));
        SDL_FillRect(winSurface, &rect, SDL_MapRGB(winSurface->format, 0, 0, 0));

        SDL_UpdateWindowSurface(window);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
