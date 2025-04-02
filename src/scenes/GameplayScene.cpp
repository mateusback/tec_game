#include "../../include/scenes/GameplayScene.h"
#include <SDL2/SDL_image.h>

GameplayScene::GameplayScene(SDL_Renderer* renderer) {
    player = new Entities::PlayerBody(100, 100, 32, 32, true, true);

    SDL_Texture* tex = IMG_LoadTexture(renderer, "assets/player.png");
    if (!tex) {
        std::printf("Erro no psrite do player do item: %s", IMG_GetError());
    } else {
        player->setTexture(tex);
    }

    ItemBody* item = new ItemBody({200, 200, 32, 32}, true, true, EItemPool::Floor, EItemType::Passive);

    SDL_Texture* itemTexture = IMG_LoadTexture(renderer, "assets/item.png");
    if (!itemTexture) {
        std::printf("Erro na sprite do item do item: %s", IMG_GetError());
    } else {
        item->setTexture(itemTexture);
    }
    items.push_back(item);

}

void GameplayScene::handleEvent(const SDL_Event& event) {
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
        std::printf("TESTEEEEEEEE");
    }
}

void GameplayScene::update(float deltaTime) {

    const Uint8* keys = SDL_GetKeyboardState(nullptr);
    player->handleInput(keys);
    player->update(deltaTime);
}

void GameplayScene::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    player->render(renderer);
    
    for (auto& item : items) {
        item->render(renderer);
    }

    SDL_RenderPresent(renderer);
}
