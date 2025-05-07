#include "../../include/core/GameLoop.h"
#include "../../include/managers/SceneManager.h"
#include "../../include/managers/FontManager.h"
#include "../../include/renders/TextRenderer.h"
#include <iostream>

namespace Core
{
    GameLoop::GameLoop(SDL_Renderer* renderer) : isRunning(true), lastFrameTime(0), renderer(renderer) {
        this->inputManager = Manager::InputManager();
        Manager::FontManager::load("default", "assets/fonts/Montserrat-Bold.ttf", 16);
        this->fpsSamples.reserve(fpsSampleSize);
        if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER) < 0) {
            std::cerr << "Erro ao inicializar SDL GameController: " << SDL_GetError() << std::endl;
        }
        std::cout << "Numero de contoles" << SDL_NumJoysticks() << std::endl;
        for (int i = 0; i < SDL_NumJoysticks(); ++i) {
            if (SDL_IsGameController(i)) {
                SDL_GameController* controller = SDL_GameControllerOpen(i);
                if (controller) {
                    std::cout << "Controle conectado: " << SDL_GameControllerName(controller) << std::endl;
                    inputManager.setController(controller);
                } else {
                    std::cerr << "Erro ao abrir controle: " << SDL_GetError() << std::endl;
                }
                break;
            }
        }
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

        float fps = (deltaTime > 0.0f) ? 1.0f / deltaTime : 0.0f;
        fpsSamples.push_back(fps);
        if (fpsSamples.size() > fpsSampleSize)
            fpsSamples.erase(fpsSamples.begin());
        
        float fpsAvg = 0.0f;
        for (float f : fpsSamples) fpsAvg += f;
        fpsAvg /= fpsSamples.size();
        
        fpsDisplayAccumulator += deltaTime;
        if (fpsDisplayAccumulator >= 0.25f) {
            smoothedFPS = static_cast<int>(fpsAvg);
            fpsDisplayAccumulator = 0.0f;
        }

        TTF_Font* font = Manager::FontManager::get("default");
        if (font) {
            std::string fpsText = "FPS: " + std::to_string(smoothedFPS);
            Core::TextRenderer::render(renderer, font, fpsText, 10, 10);
        }

        SDL_RenderPresent(renderer);
    }
}
