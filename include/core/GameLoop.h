#ifndef GAMELOOP_H
#define GAMELOOP_H
#include <SDL2/SDL.h>

namespace Core
{
    class GameLoop 
    {
    public:
        GameLoop(SDL_Renderer* renderer);
        void run();
        
    private:
        bool isRunning;
        Uint32 lastFrameTime;
        SDL_Renderer* renderer;
        
        void processInput();
        void update(float deltaTime);
        void render();
    };
    
}

#endif