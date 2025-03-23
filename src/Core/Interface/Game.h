#ifndef GAME_H
#define GAME_H
#include <SDL2/SDL.h>
#include "GameLoop.h"

class Game {
public:
    Game();
    ~Game();

    bool Init(const char* title, int width, int height);
    void Run();
    void Shutdown();

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    GameLoop* loop;
};

#endif