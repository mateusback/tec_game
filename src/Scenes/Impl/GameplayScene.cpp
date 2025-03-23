#include "../Interface/GameplayScene.h"

GameplayScene::GameplayScene() {
    player = new PlayerBody(100, 100, 32, 32, true, true);
}

void GameplayScene::HandleEvent(const SDL_Event& event) {
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
    }
}

void GameplayScene::Update(float deltaTime) {
    float moveX = 0.0f, moveY = 0.0f;

    const Uint8* keys = SDL_GetKeyboardState(NULL);
    player->HandleInput(keys, moveX, moveY);
    player->Update(deltaTime, moveX, moveY);
}

void GameplayScene::Render(SDL_Renderer* renderer) {
    player->Render(renderer);
}
