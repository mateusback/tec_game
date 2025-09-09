#ifndef GAMEPLAY_SCENE_H
#define GAMEPLAY_SCENE_H

#include <list>
#include <memory>
#include "../core/Scene.h"
#include "../entities/PlayerBody.h"
#include "../managers/ItemManager.h"
#include "../managers/EnemyManager.h"
#include "../map/Floor.h"
#include "../map/TileSet.h"
#include "../renders/VirtualRenderer.h"
#include "../renders/HudRenderer.h"
#include "../managers/RoomManager.h"
#include "../renders/MiniMapRenderer.h"
#include "../handlers/BombHandler.h"
#include "../handlers/NotificationHandler.h"

class GameplayScene : public Core::Scene {
private:
    Entities::PlayerBody* player;
    bool debugMode = false;
    bool isPaused = false;
    Manager::ItemManager itemManager;
    Manager::EnemyManager enemyManager;
    Map::Floor floor;
    Manager::EntityManager entityManager;
    Renderer::HudRenderer* hudRenderer = nullptr;
    Manager::RoomManager* roomManager = nullptr;
    Manager::BossManager bossManager;
    Renderer::MiniMapRenderer* miniMapRenderer = nullptr;
    TileSet tileSet;
    std::unique_ptr<BombHandler> bombHandler;
    NotificationHandler notificationHandler;
    SDL_Renderer* renderer;

    void setupRoomManager();
    void generateFloorAndLoadStartRoom();
    void setupHandlersAndRenderers();

    bool checkIfPlayerIsDead();
    void updateBombs(float deltaTime);
    void updateTilesAndHandlePlayerCollision();
    void updateItemsAndPickup(float deltaTime);
    void updateEnemies(float deltaTime);
    void updateBosses(float deltaTime);
    void updateAttacks(float deltaTime);
    void updateEffects(float deltaTime);

    void clearScreen(SDL_Renderer* renderer);
    void renderEntities(SDL_Renderer* renderer);
    void renderPlayer(SDL_Renderer* renderer);
    void renderEnemyHealthBars(SDL_Renderer* renderer);
    void renderBossHealthBars(SDL_Renderer* renderer);
    void renderDebugColliders(SDL_Renderer* renderer);

    void loadResources(SDL_Renderer* renderer);
    void loadAudios();
    void loadStaticSprites(SDL_Renderer* renderer);
    void loadAnimations(SDL_Renderer* renderer);

    void addDestroyEffect(Vector2f position, Vector2f scale);
    void addSplashEffect(Vector2f position, Vector2f scale);

public:
    GameplayScene(SDL_Renderer* renderer, int screenWidth, int screenHeight);
    ~GameplayScene() override;

    void update(float deltaTime, const Manager::PlayerInput& input);
    void handleEvent(const SDL_Event& event) override;
    void render(SDL_Renderer* renderer) override;
    
    void setItemManager(const Manager::ItemManager& itemManager) { this->itemManager = itemManager; }
    Manager::ItemManager getItemManager() { return itemManager; }
};

#endif
