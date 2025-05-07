#ifndef GAMELOOP_H
#define GAMELOOP_H
#include <SDL2/SDL.h>
#include "../managers/InputManager.h"
#include <vector>


namespace Core {
    class GameLoop {
    private:
        bool isRunning;
        Uint32 lastFrameTime;
        SDL_Renderer* renderer;
        float deltaTime;
        std::vector<float> fpsSamples;
        const size_t fpsSampleSize = 30;
        float fpsDisplayAccumulator = 0.0f;
        int smoothedFPS = 0;
        
        Manager::InputManager inputManager;

        void processInput();
        void update();
        void render();

    public:
        GameLoop(SDL_Renderer* renderer);
        void run();

		#pragma region Getters
        float getDeltaTime() const { return this->deltaTime; }
		#pragma endregion

		#pragma region Setters
        void setDeltaTime(float deltaTime) { this->deltaTime = deltaTime; }
		#pragma endregion
    };
    
}

#endif