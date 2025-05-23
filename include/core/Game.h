#ifndef GAME_H
#define GAME_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "GameLoop.h"

namespace Core
{
    class Game 
    {
    public:
        Game();
        ~Game();
        
        bool init(const char* title, int width, int height);
        void run();
        void shutdown();
        
    private:
        SDL_Window* window;
        SDL_Renderer* renderer;
        GameLoop* loop;
    };
    
}

#endif