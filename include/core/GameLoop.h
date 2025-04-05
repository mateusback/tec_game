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
        float getDeltaTime() const { return this->deltaTime; }
        void setDeltaTime(float deltaTime) { this->deltaTime = deltaTime; }
        
    private:
        bool isRunning;
        Uint32 lastFrameTime;
        SDL_Renderer* renderer;
        float deltaTime;
        
        void processInput();
        void update();
        void render();
    };
    
}

#endif