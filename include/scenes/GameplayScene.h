#ifndef GAMEPLAY_SCENE_H
#define GAMEPLAY_SCENE_H

#include <list>
#include <memory>
#include "../../include/core/Scene.h"
#include "../../include/entities/PlayerBody.h"
#include "../../include/entities/ItemBody.h"
#include "../../include/entities/TileBody.h"
#include "../../include/entities/EnemyBody.h"
#include "../../include/managers/ItemManager.h"
#include "../../include/managers/EnemyManager.h"
#include "../../include/map/Floor.h"
#include "../../include/map/TileSet.h"
#include "../../include/renders/VirtualRenderer.h"

class GameplayScene : public Core::Scene {
private:
    Entities::PlayerBody* player;
    Manager::ItemManager itemManager;
    Manager::EnemyManager enemyManager;
    Map::Floor floor;
    Map::Room* currentRoom = nullptr;
    Manager::EntityManager entityManager;
    bool debugMode = false;

    TileSet tileSet;

    void loadFloor(int index);
    void loadCurrentRoom(SDL_Renderer* renderer);

public:
    GameplayScene(SDL_Renderer* renderer, int screenWidth, int screenHeight);

    void update(float deltaTime, const Manager::PlayerInput& input);
    void handleEvent(const SDL_Event& event) override;
    void render(SDL_Renderer* renderer) override;
    void addDestroyEffect(Vector position, Vector scale);
    
    void setItemManager(const Manager::ItemManager& itemManager) { this->itemManager = itemManager; }
    Manager::ItemManager getItemManager() { return itemManager; }
};

#endif
