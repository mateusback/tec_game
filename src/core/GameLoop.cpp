#include "../../include/core/GameLoop.h"
#include "../../include/managers/SceneManager.h"
#include "../../include/managers/FontManager.h"
#include "../../include/renders/TextRenderer.h"
#include <iostream>

namespace Core
{
    GameLoop::GameLoop(SDL_Renderer* renderer) : isRunning(true), lastFrameTime(0), renderer(renderer) {
        this->inputManager = Manager::InputManager();
    }

    void GameLoop::run() {
        while (this->isRunning) {
            Uint32 currentTime = SDL_GetTicks();
            this->deltaTime = (currentTime - lastFrameTime) / 1000.0f;
            lastFrameTime = currentTime;
            
            processInput();
            update();
            render();

            SDL_Delay(1);
        }
    }

    void GameLoop::processInput() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                this->isRunning = false;
            }
            Scene* scene = Manager::SceneManager::getCurrentScene();
            if (scene) 
                scene->handleEvent(event);
        }
    }

    void GameLoop::update() {
        this->inputManager.updateKeyboardState();
        this->inputManager.updateControllerState();
    
        Scene* scene = Manager::SceneManager::getCurrentScene();
        if (scene)
            scene->update(this->getDeltaTime(), this->inputManager.getState());
    }

    void GameLoop::render() {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        Scene* scene = Manager::SceneManager::getCurrentScene();
        if (scene) 
            scene->render(renderer);

        auto currentFPS = (deltaTime > 0.0f) ? static_cast<int>(1.0f / deltaTime) : 0;
        TTF_Font* font = Manager::FontManager::get("default");
        std::string fpsText = "FPS: " + std::to_string(currentFPS);
        Core::TextRenderer::render(renderer, font, fpsText, 10, 10);

        SDL_RenderPresent(renderer);
    }
}
