#ifndef GAMEPLAY_SCENE_H
#define GAMEPLAY_SCENE_H

#include <list>
#include <memory>
#include "../core/Scene.h"
#include "../entities/PlayerBody.h"
#include "../entities/ItemBody.h"
#include "../entities/TileBody.h"
#include "../entities/EnemyBody.h"
#include "../managers/ItemManager.h"
#include "../managers/EnemyManager.h"
#include "../map/Floor.h"
#include "../map/TileSet.h"
#include "../renders/VirtualRenderer.h"
#include "../renders/HudRenderer.h"
#include "../managers/RoomManager.h"


class GameplayScene : public Core::Scene {
private:
    Entities::PlayerBody* player;
    Manager::ItemManager itemManager;
    Manager::EnemyManager enemyManager;
    Map::Floor floor;
    Map::Room* currentRoom = nullptr;
    Manager::EntityManager entityManager;
    bool debugMode = false;
    Renderer::HudRenderer* hudRenderer = nullptr;
    Manager::RoomManager* roomManager = nullptr;

    TileSet tileSet;

public:
    GameplayScene(SDL_Renderer* renderer, int screenWidth, int screenHeight);
    ~GameplayScene() override;

    void update(float deltaTime, const Manager::PlayerInput& input);
    void handleEvent(const SDL_Event& event) override;
    void render(SDL_Renderer* renderer) override;
    void addDestroyEffect(Vector2f position, Vector2f scale);
    void loadResources(SDL_Renderer* renderer);
    
    void setItemManager(const Manager::ItemManager& itemManager) { this->itemManager = itemManager; }
    Manager::ItemManager getItemManager() { return itemManager; }
};

#endif
