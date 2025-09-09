#ifndef GAME_H
#define GAME_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "GameLoop.h"

namespace Core {
    class Game {
    private:
        SDL_Window* window;
        SDL_Renderer* renderer;
        GameLoop* loop;
        int screenWidth;
        int screenHeight;
        
    public:
        Game(const char* title, int width, int height);
        ~Game();
        
        void run();
    };
}

#endif