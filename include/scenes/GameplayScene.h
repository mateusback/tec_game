#ifndef GAMEPLAY_SCENE_H
#define GAMEPLAY_SCENE_H

#include <list>
#include <memory>
#include "../../include/core/Scene.h"
#include "../../include/entities/PlayerBody.h"
#include "../../include/entities/ItemBody.h"
#include "../../include/managers/ItemManager.h"

class GameplayScene : public Core::Scene {
private:
    Entities::PlayerBody* player;
    ItemManager itemManager;

public:
    GameplayScene(SDL_Renderer* renderer);
    void update(float deltaTime) override;
    void handleEvent(const SDL_Event& event) override;
    void render(SDL_Renderer* renderer) override;
    ItemManager getItemManager() { return itemManager; }
    void setItemManager(const ItemManager& itemManager) { this->itemManager = itemManager; }
};

#endif
