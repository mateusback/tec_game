#ifndef GAMELOOP_H
#define GAMELOOP_H
#include <SDL2/SDL.h>

class GameLoop {
public:
    GameLoop(SDL_Renderer* renderer);
    void Run();

private:
    bool isRunning;
    Uint32 lastFrameTime;

    void ProcessInput();
    void Update(float deltaTime);
    void Render();
};

#endif