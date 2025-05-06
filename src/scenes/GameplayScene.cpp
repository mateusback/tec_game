#include "../../include/scenes/GameplayScene.h"
#include "../../include/physics/CollisionManager.h"
#include "../../include/renders/TextRenderer.h"
#include "../../include/managers/FontManager.h"
#include "../../include/serializers/FloorSerialization.h"
#include "../../include/utils/DebugUtils.h"
#include "../../include/utils/GlobalAccess.h"
#include "../../include/utils/Types.h"
#include "../../include/entities/EffectBody.h"
#include "../../include/entities/BombBody.h"

#include <SDL2/SDL_image.h>
#include <fstream>

GameplayScene::GameplayScene(SDL_Renderer* renderer, int screenWidth, int screenHeight) {
    Manager::AudioManagerGlobal::init();
    this->loadResources(renderer);

    this->roomManager = new Manager::RoomManager(renderer, 
        &this->entityManager, &this->tileSet, 
        &this->itemManager, &this->enemyManager);


    this->roomManager->loadFloor(1);
    this->roomManager->loadRoomByType(Map::ERoomType::Start);
    this->player = this->roomManager->getPlayer();
    std::cout << "Player position: " << this->player->getPosition().x << ", " << this->player->getPosition().y << std::endl;

    this->hudRenderer = new Renderer::HudRenderer(renderer);
}


void GameplayScene::handleEvent(const SDL_Event& event) {
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
        //TODO - Implement pause menu
    }
}

void GameplayScene::update(float deltaTime, const Manager::PlayerInput& input) {
    this->player->handleInput(input);

    if (input.shootDirection.x != 0 || input.shootDirection.y != 0)
    {
        auto attack = player->attack(player->getCenterPoint(), input.shootDirection);
        if(attack){
            entityManager.add(std::move(attack));
        }
    }

    //TODO - Depois passar para o player
    if (input.putBomb && player->getBombCooldown() <= 0.0f) {
        Vector2f pos = player->getPosition();
        auto bomb = std::make_unique<Entities::BombBody>(
            Vector4f{
                pos.x,
                pos.y,
                virtualRenderer()->getTileSize(),
                virtualRenderer()->getTileSize()  
            },
            true,
            true,
            2.0f,
            virtualRenderer()->getTileSize() * 3.0f,
            60.0f,
            entityManager,
            tileSet
        );
        player->setBombCooldown(5.0f);
        bomb->setTexture(Manager::TextureManager::Get("bomb"));
        entityManager.add(std::move(bomb));
    }

    
    this->player->update(deltaTime);
    //this->entityManager.updateAll(deltaTime);

    auto items = entityManager.getEntitiesByType<Entities::ItemBody>();
    auto tiles = entityManager.getEntitiesByType<Entities::TileBody>();
    auto enemies = entityManager.getEntitiesByType<Entities::EnemyBody>();
    auto attacks = entityManager.getEntitiesByType<Entities::AttackBody>();
    auto effects = entityManager.getEntitiesByType<Entities::EffectBody>();
    auto bombs = entityManager.getEntitiesByType<Entities::BombBody>();
    Vector2f totalDisplacement = {0.f, 0.f};

    for (auto* bomb : bombs) {
        bomb->update(deltaTime);
    }

    for (auto* tile : tiles) {
        if (tile->hasCollision() &&
            Physics::CollisionManager::checkCollision(player->getHitbox(), tile->getHitbox())) {
            player->onCollision(tile);
            break;
        }
    }

    player->setPosition(player->getPosition().x + totalDisplacement.x, player->getPosition().y + totalDisplacement.y);
    
    for (auto* item : items) {
        if (item->hasCollision() &&
            Physics::CollisionManager::checkCollision(player->getHitbox(), item->getHitbox())) {
            player->onCollision(item);
        }
    }
    
    for (auto* attack : attacks) {
        attack->update(deltaTime);

        if (attack->getOrigin() != this->player && player->hasCollision() && attack->hasCollision() &&
        Physics::CollisionManager::checkCollision(attack->getHitbox(), player->getHitbox())) {
            player->takeDamage(attack->getAttackDamage());
            audio()->playSoundEffect("hit-player", 0);
            attack->setActive(false);

            addDestroyEffect(attack->getPosition(), attack->getScale());
            continue;
        }

        if (attack->getOrigin() == this->player) {
            for (auto* enemy : enemies) {
                if (enemy->hasCollision() && attack->hasCollision() &&
                    Physics::CollisionManager::checkCollision(enemy->getHitbox(), attack->getHitbox())) {
    
                    enemy->takeDamage(attack->getAttackDamage());
                    audio()->playSoundEffect("hit-enemy", 0);
                    attack->setActive(false);
    
                    addDestroyEffect(attack->getPosition(), attack->getScale());
                    if (enemy->getHealth() <= 0)
                        enemy->setActive(false);
                    break;
                }
            }
        }

        for (auto* tile : tiles) {
            if (tile->hasCollision() &&
                Physics::CollisionManager::checkCollision(attack->getHitbox(), tile->getHitbox())) {
    
                attack->setActive(false);
                addDestroyEffect(attack->getPosition(), attack->getScale());
                break;
            }
        }
    }

    for (auto* enemy : enemies) {
        enemy->update(deltaTime);
    }

    for (auto* effect : effects) {
        effect->update(deltaTime);
    }

    this->entityManager.removeInactive();
    this->entityManager.addAll();
    
    this->roomManager->update(deltaTime);
    this->roomManager->checkAndMovePlayerBetweenRooms();

    if(!this->player->isActive()) {
        //TODO - Implementar tela de game over
    }
}

void GameplayScene::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    //TODO - Isso foi um grande erro, lembrar de separar em camadas depois
    this->entityManager.renderAll(renderer);
    this->player->render(renderer);

    auto enemies = entityManager.getEntitiesByType<Entities::EnemyBody>();
    for (auto* enemy : enemies) {
        SDL_FRect barBg, barFill;
    
        float healthPercent = enemy->getHealthPercent();
        Vector2f pos = enemy->getPosition();
        Vector2f size = enemy->getScale();
    
        barBg = {
            pos.x,
            pos.y - 6.f,
            size.x,
            5.f
        };
    
        barFill = {
            pos.x,
            pos.y - 6.f,
            size.x * healthPercent,
            5.f
        };
    
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        SDL_RenderFillRectF(renderer, &barBg);
    
        SDL_SetRenderDrawColor(renderer, 200, 50, 50, 255);
        SDL_RenderFillRectF(renderer, &barFill);
    }

    if (true) {
        Utils::DebugUtils::drawCollidersOfType<Entities::ItemBody>(renderer, entityManager, {0, 0, 255, 255});
        Utils::DebugUtils::drawCollidersOfType<Entities::TileBody>(renderer, entityManager, {0, 255, 0, 255});
        Utils::DebugUtils::drawCollidersOfType<Entities::AttackBody>(renderer, entityManager, {255, 0, 255, 255});
        Utils::DebugUtils::drawCollidersOfType<Entities::EnemyBody>(renderer, entityManager, {255, 255, 0, 255});
    
        if (player->hasCollision()) {
            SDL_FRect rect = {
                player->getHitbox().x, player->getHitbox().y,
                player->getHitbox().w, player->getHitbox().z
            };
            Utils::DebugUtils::drawCollider(renderer, rect, {255, 0, 0, 255});
        }
    }
    this->hudRenderer->render(renderer, this->player);
    SDL_RenderPresent(renderer);
}


void GameplayScene::loadResources(SDL_Renderer* renderer){
    textures()->Load(renderer, "player_b", "assets/player/personagem_B.png");
    textures()->Load(renderer, "player_f", "assets/player/personagem_F.png");
    textures()->Load(renderer, "player_l", "assets/player/personagem_L.png");
    textures()->Load(renderer, "player_r", "assets/player/personagem_R.png");
    textures()->Load(renderer, "player_sheet", "assets/animations/player.png");

    textures()->Load(renderer, "player_with_item", "assets/player_with_item.png");
    textures()->Load(renderer, "attack", "assets/attack.png");
    textures()->Load(renderer, "attack_destroy", "assets/attack_fade.png");

    textures()->Load(renderer, "shell_hidden", "assets/enemies/shell_hidden.png");
    textures()->Load(renderer, "bomb", "assets/bomb.png");
    textures()->Load(renderer, "bomb_explosion", "assets/bomb_explosion.png");
    textures()->Load(renderer, "hudsheet", "assets/hudsheet.png");

    audio()->loadSoundEffect("bomb_explosion", "assets/audio/explosion.mp3");
    audio()->loadSoundEffect("shoot", "assets/audio/shoot.mp3");
    audio()->loadSoundEffect("hit-enemy", "assets/audio/hit-enemy.mp3");
    audio()->loadSoundEffect("hit-player", "assets/audio/hit-player.mp3");
    audio()->loadSoundEffect("pickup-item", "assets/audio/pickup-item.mp3");

    Manager::FontManager::load("default", "assets/fonts/Montserrat-Bold.ttf", 16);
    this->enemyManager.loadFromFile("assets/data/enemies.json");
    this->tileSet.loadFromFile("assets/data/tileset.json");
    this->itemManager.loadFromFile("assets/data/items.json");
    textures()->Load(renderer, "tileset", tileSet.getSpriteSheetPath());
}

void GameplayScene::addDestroyEffect(Vector2f position, Vector2f scale) {
    auto effect = std::make_unique<Entities::EffectBody>(
        position,
        scale,
        textures()->Get("attack_destroy"),
        0.2f
    );
    entityManager.add(std::move(effect));
}

GameplayScene::~GameplayScene() {
    delete this->hudRenderer;
    this->hudRenderer = nullptr;

    delete this->player;
    this->player = nullptr;

    delete this->roomManager;
    this->roomManager = nullptr;
}
