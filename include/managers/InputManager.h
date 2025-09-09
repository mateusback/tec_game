#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <SDL2/SDL.h>
#include "../utils/Types.h"

namespace Manager {
    struct PlayerInput {
        Vector2f moveDirection = {0.f, 0.f};
        Vector2f shootDirection = {0.f, 0.f};
        bool shoot = false;
        bool pause = false;
        bool putBomb = false;
    };

    class InputManager {
        public:
            void update();
            const PlayerInput& getInput() const;
    
            void handleEvent(const SDL_Event& event);
            void setController(SDL_GameController* controller);

            PlayerInput getState() const {
                return currentInput;
            }
    
            void updateKeyboardState();
            void updateControllerState();

        private:
            PlayerInput currentInput;
            const Uint8* keyboard = nullptr;
            SDL_GameController* controller = nullptr;
        };

}

#endif
