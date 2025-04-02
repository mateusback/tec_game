#include "../../include/core/GameLoop.h"
#include "../../include/core/SceneManager.h"
#include <iostream>

namespace Core
{
    GameLoop::GameLoop(SDL_Renderer* renderer) : isRunning(true), lastFrameTime(0), renderer(renderer) {}

    void GameLoop::run() {
        while (isRunning) {
            Uint32 currentTime = SDL_GetTicks();
            float deltaTime = (currentTime - lastFrameTime) / 1000.0f;
            lastFrameTime = currentTime;

            processInput();
            update(deltaTime);
            render();

            SDL_Delay(1);
        }
    }

    void GameLoop::processInput() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
            }
            Scene* scene = SceneManager::getCurrentScene();
            if (scene) scene->handleEvent(event);
        }
    }

    void GameLoop::update(float deltaTime) {
        Scene* scene = SceneManager::getCurrentScene();
        if (scene) scene->update(deltaTime);
    }

    void GameLoop::render() {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        Scene* scene = SceneManager::getCurrentScene();
        if (scene) scene->render(renderer);

        SDL_RenderPresent(renderer);
    }
}
