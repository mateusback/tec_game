#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <SDL2/SDL.h>
#include <my-lib/math-vector.h>

using Vector = Mylib::Math::Vector<float, 2>;
using Vector4 = Mylib::Math::Vector<float, 4>;

namespace Manager {
    struct PlayerInput {
        Vector moveDirection = {0.f, 0.f};
        Vector shootDirection = {0.f, 0.f};
        bool shoot = false;
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
