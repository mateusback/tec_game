#include "../Interface/GameLoop.h"
#include <iostream>

GameLoop::GameLoop(SDL_Renderer* renderer) : isRunning(true), lastFrameTime(0) {
}

void GameLoop::Run() {
    while (isRunning) {
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastFrameTime) / 1000.0f;
        lastFrameTime = currentTime;

        ProcessInput();
        Update(deltaTime);
        Render();

        SDL_Delay(1); 
    }
}

void GameLoop::ProcessInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            isRunning = false;
        }
    }
}

void GameLoop::Update(float deltaTime) {
}

void GameLoop::Render() {

}
