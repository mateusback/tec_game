#include "../include/managers/InputManager.h"
#include <cmath>

namespace Manager {

    void InputManager::update() {
        SDL_PumpEvents();

        updateKeyboardState();
        updateControllerState();
    }

    void InputManager::updateKeyboardState() {
        keyboard = SDL_GetKeyboardState(nullptr);

        Vector playerDirection = {0.f, 0.f};
        Vector shootDirection = {0.f, 0.f};
        this->currentInput.shoot = false;
        this->currentInput.putBomb = false;

        if (keyboard[SDL_SCANCODE_W]) playerDirection.y -= 1;
        if (keyboard[SDL_SCANCODE_S]) playerDirection.y += 1;
        if (keyboard[SDL_SCANCODE_A]) playerDirection.x -= 1;
        if (keyboard[SDL_SCANCODE_D]) playerDirection.x += 1;

        if (keyboard[SDL_SCANCODE_UP]) shootDirection.y = -1;
        if (keyboard[SDL_SCANCODE_DOWN]) shootDirection.y = 1;
        if (keyboard[SDL_SCANCODE_LEFT]) shootDirection.x = -1;
        if (keyboard[SDL_SCANCODE_RIGHT]) shootDirection.x = 1;


        if (keyboard[SDL_SCANCODE_E]) this->currentInput.putBomb = true;

        if (shootDirection.x != 0 || shootDirection.y != 0)
        {
            float length = sqrtf(shootDirection.x * shootDirection.x + shootDirection.y * shootDirection.y);
            shootDirection.x /= length;
            shootDirection.y /= length;
            this->currentInput.shoot = true;
        }

        if (playerDirection.x != 0 && playerDirection.y != 0)
        {
            float invRaiz = 1.0f / sqrtf(2.0f);
            playerDirection.x *= invRaiz;
            playerDirection.y *= invRaiz;
        }

        this->currentInput.moveDirection = playerDirection;
        this->currentInput.shootDirection = shootDirection;
    }

    void InputManager::updateControllerState() {
        if (!controller || !SDL_GameControllerGetAttached(controller)) return;
    
        Vector playerDirection = {0.f, 0.f};
        Vector shootDirection = {0.f, 0.f};
        this->currentInput.shoot = false;
        this->currentInput.putBomb = false;
    
        float lx = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX) / 32768.0f;
        float ly = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY) / 32768.0f;
    
        if (std::abs(lx) >= 0.2f) playerDirection.x = lx;
        if (std::abs(ly) >= 0.2f) playerDirection.y = ly;
    
        float rx = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTX) / 32768.0f;
        float ry = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTY) / 32768.0f;
    
        if (std::abs(rx) >= 0.2f) shootDirection.x = rx;
        if (std::abs(ry) >= 0.2f) shootDirection.y = ry;
    
        if (playerDirection.x != 0.f && playerDirection.y != 0.f) {
            float invLen = 1.0f / std::sqrt(playerDirection.x * playerDirection.x + playerDirection.y * playerDirection.y);
            playerDirection.x *= invLen;
            playerDirection.y *= invLen;
        }
    
        if (shootDirection.x != 0.f || shootDirection.y != 0.f) {
            float length = std::sqrt(shootDirection.x * shootDirection.x + shootDirection.y * shootDirection.y);
            shootDirection.x /= length;
            shootDirection.y /= length;
            this->currentInput.shoot = true;
        }

        if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_A) == SDL_PRESSED) this->currentInput.putBomb = true;
    
        this->currentInput.moveDirection = playerDirection;
        this->currentInput.shootDirection = shootDirection;
    }

    void InputManager::handleEvent(const SDL_Event& event) {
        if (event.type == SDL_CONTROLLERDEVICEADDED) {
            setController(SDL_GameControllerOpen(event.cdevice.which));
        }
    }

    const PlayerInput& InputManager::getInput() const {
        return this->currentInput;
    }

    void InputManager::setController(SDL_GameController* ctrl) {
        this->controller = ctrl;
    }

}
