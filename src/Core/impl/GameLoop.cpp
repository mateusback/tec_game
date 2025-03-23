#include "../Interface/GameLoop.h"
#include "../Interface/SceneManager.h"
#include <iostream>

GameLoop::GameLoop(SDL_Renderer* renderer) : renderer(renderer), isRunning(true), lastFrameTime(0) {}

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

        Scene* scene = SceneManager::GetCurrentScene();
        if (scene) scene->HandleEvent(event);
    }
}

void GameLoop::Update(float deltaTime) {
    Scene* scene = SceneManager::GetCurrentScene();
    if (scene) scene->Update(deltaTime);
}

void GameLoop::Render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    Scene* scene = SceneManager::GetCurrentScene();
    if (scene) scene->Render(renderer);

    SDL_RenderPresent(renderer);
}