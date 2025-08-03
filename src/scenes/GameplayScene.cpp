#include "../../include/scenes/GameplayScene.h"
#include "../../include/physics/CollisionManager.h"
#include "../../include/entities/EnemyBody.h"
#include "../../include/entities/TileBody.h"
#include "../../include/managers/FontManager.h"
#include "../../include/utils/DebugUtils.h"
#include "../../include/utils/GlobalAccess.h"
#include "../../include/utils/Types.h"
#include "../../include/entities/EffectBody.h"
#include "../../include/entities/BombBody.h"
#include "../../include/managers/EventManager.h"
#include "../../include/entities/LaserAttackBody.h"
#include "../../include/entities/BossBody.h"
#include "../../include/renders/TextRenderer.h"
#include "../../include/scenes/EndScene.h"
#include "../../include/managers/SceneManager.h"

#include <fstream>
#include <chrono>

GameplayScene::GameplayScene(SDL_Renderer* renderer, int screenWidth, int screenHeight) {
    this->renderer = renderer;
    this->debugMode = true;
    this->loadResources(this->renderer);
    notificationHandler.setFont(Manager::FontManager::get("default"));

    this->roomManager = new Manager::RoomManager(this->renderer,
        &this->entityManager, &this->tileSet, 
        &this->itemManager, &this->enemyManager,
        &this->bossManager);

    unsigned randomSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    this->roomManager->generateFloor(1, randomSeed);
    
    this->roomManager->loadRequiredAssets(this->renderer);
    this->roomManager->loadRoomByType(Map::ERoomType::Start);
    this->player = this->roomManager->getPlayer();
    std::cout << "Player position: " << this->player->getPosition().x << ", " << this->player->getPosition().y << std::endl;

    this->bombHandler = std::make_unique<BombHandler>(&this->entityManager, &this->tileSet);
    this->miniMapRenderer = new Renderer::MiniMapRenderer(this->renderer, this->roomManager);
    this->hudRenderer = new Renderer::HudRenderer(this->renderer);
}


void GameplayScene::handleEvent(const SDL_Event& event) {
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
        this->isPaused = !this->isPaused;
    }
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_F3) {
        this->debugMode = !this->debugMode;
    }
}

void GameplayScene::update(float deltaTime, const Manager::PlayerInput& input) {

    if (this->isPaused) return;
    if(!this->player->isActive()) return;

    this->player->handleInput(input);

    this->player->update(deltaTime);

    auto items = entityManager.getEntitiesByType<Entities::ItemBody>();
    auto tiles = entityManager.getEntitiesByType<Entities::TileBody>();
    auto enemies = entityManager.getEntitiesByType<Entities::EnemyBody>();
    auto attacks = entityManager.getEntitiesByType<Entities::AttackBody>();
    auto effects = entityManager.getEntitiesByType<Entities::EffectBody>();
    auto bombs = entityManager.getEntitiesByType<Entities::BombBody>();
    auto lasers = entityManager.getEntitiesByType<Entities::LaserAttackBody>();
    auto bosses = entityManager.getEntitiesByType<Entities::BossBody>();

    for (auto* boss : bosses) {
        boss->update(deltaTime);
        if (Physics::isColliding(this->player, boss)) {
            player->onCollision(boss);
            Physics::CollisionManager::resolveCollision(this->player, boss);
        }
        for (auto* tile : tiles) {
            if (Physics::isColliding(boss, tile)) {
                Physics::CollisionManager::resolveCollision(boss, tile);
            }
        }
    }

    for (auto* bomb : bombs) {
        bomb->update(deltaTime);
    }

    for (auto* tile : tiles) {
        if (Physics::isColliding(this->player, tile)) {
            Physics::CollisionManager::resolveCollision(this->player, tile);
        if (this->debugMode) break;
        }
        for (auto* lasers : lasers) {
            if (Physics::isColliding(lasers, tile)) {
                addSplashEffect(lasers->getPosition(), lasers->getScale());
            }          
        }
    }

    for (auto* item : items) {
        item->update(deltaTime);
        if (Physics::isColliding(this->player, item)) {
            notificationHandler.show(item->getItem().getName(), item->getItem().getDescription());
            player->onCollision(item);
        }
    }
    
    for (auto* attack : attacks) {
        attack->update(deltaTime);

        if (attack->getOrigin() != this->player && Physics::isColliding(attack, player)) {
            player->takeDamage(attack->getAttackDamage());
            addDestroyEffect(attack->getPosition(), attack->getScale());
            attack->setActive(false);
            continue;
        }

        if (attack->getOrigin() == this->player) {
            for (auto* enemy : enemies) {
                if (Physics::isColliding(attack, enemy)) {
                    enemy->takeDamage(attack->getAttackDamage());
                    audio()->playSoundEffect("hit-enemy", 0);
                    attack->setActive(false);
    
                    addDestroyEffect(attack->getPosition(), attack->getScale());
                    if (enemy->getHealth() <= 0){
                        score()->add(20);
                        enemy->setActive(false);
                    }
                }
            }
        }

        for (auto* boss : bosses) {
            if (!boss->isActive()) continue;

            if (Physics::isColliding(attack, boss)) {
                boss->takeDamage(attack->getAttackDamage());
                audio()->playSoundEffect("hit-boss", 0);
                attack->setActive(false);
                addDestroyEffect(attack->getPosition(), attack->getScale());

                if (boss->getHealth() <= 0) {
                    score()->add(100);
                    boss->setActive(false);
                    std::cout << "Boss defeated!" << std::endl;
                    Manager::SceneManager::setScene(new Scenes::EndScene(this->renderer, virtualRenderer()->getScreenWidth(), virtualRenderer()->getScreenHeight(), score()->getScore()));
                    return;
                }
                break;
            }
        }

        for (auto* tile : tiles) {
            if (tile->hasCollision() &&
                Physics::CollisionManager::checkCollision(attack->getHitbox(), tile->getHitbox())) {
    
                attack->setActive(false);
                addDestroyEffect(attack->getPosition(), attack->getScale());
            }
        }
    }

    for (auto* enemy : enemies) {
        enemy->update(deltaTime);
        if (Physics::isColliding(this->player, enemy)) {
            player->onCollision(enemy);
            Physics::CollisionManager::resolveCollision(this->player, enemy);
        }
        for (auto* tile : tiles) {
            if (Physics::isColliding(enemy, tile)) {
                Physics::CollisionManager::resolveCollision(enemy, tile);
            }
        }
        for (auto* laser : lasers) {
            if (Physics::isColliding(enemy, laser)) {
                enemy->takeDamage(laser->getDamage());
                audio()->playSoundEffect("hit-enemy", 0);
                enemy->setInvencibleTimer(laser->getDamageTime());
                if (enemy->getHealth() <= 0) {
                    score()->add(20);
                    enemy->setActive(false);
                }
            }
        }

    }

    for (auto* effect : effects) {
        effect->update(deltaTime);
    }

    notificationHandler.update(deltaTime);

    this->entityManager.removeInactive();
    this->entityManager.addAll();
    
    this->roomManager->update(deltaTime);
    this->roomManager->checkAndMovePlayerBetweenRooms();
}

void GameplayScene::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
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

    auto bosses = entityManager.getEntitiesByType<Entities::BossBody>();
    for (auto* boss : bosses) {
        if (!boss->isActive()) continue;

        float screenWidth = virtualRenderer()->getScreenWidth();
        float screenHeight = virtualRenderer()->getScreenHeight();

        float barWidth = 300.0f;
        float barHeight = 20.0f;
        float marginBottom = 30.0f;
        float marginTop = 8.0f;

        float healthPercent = boss->getHealthPercent();
        Vector2f barPos = {
            (screenWidth - barWidth) / 2.0f,
            screenHeight - barHeight - marginBottom
        };

        SDL_FRect barBg = {
            barPos.x,
            barPos.y,
            barWidth,
            barHeight
        };

        SDL_FRect barFill = {
            barPos.x,
            barPos.y,
            barWidth * healthPercent,
            barHeight
        };

        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        SDL_RenderFillRectF(renderer, &barBg);

        SDL_SetRenderDrawColor(renderer, 200, 30, 30, 255);
        SDL_RenderFillRectF(renderer, &barFill);

        SDL_Color white = {255, 255, 255, 255};
        std::string name = boss->getBossData().getName();

        Core::TextRenderer::render(
            renderer,
            Manager::FontManager::get("default"),
            name,
            static_cast<int>(barPos.x + barWidth / 2.0f),
            static_cast<int>(barPos.y - marginTop),
            white,
            true
        );
    }

    if (this->debugMode) {
        Utils::DebugUtils::drawCollidersOfType<Entities::ItemBody>(renderer, entityManager, {0, 0, 255, 255});
        Utils::DebugUtils::drawCollidersOfType<Entities::TileBody>(renderer, entityManager, {0, 255, 0, 255});
        Utils::DebugUtils::drawCollidersOfType<Entities::AttackBody>(renderer, entityManager, {255, 0, 255, 255});
        Utils::DebugUtils::drawCollidersOfType<Entities::EnemyBody>(renderer, entityManager, {255, 255, 0, 255});
        Utils::DebugUtils::drawCollidersOfType<Entities::BossBody>(renderer, entityManager, {255, 165, 0, 255});

        if (player->hasCollision()) {
            Vector4f hb = player->getHitbox();
            SDL_FRect rect = {
                hb.x, hb.y,
                hb.z, hb.w 
            };
            Utils::DebugUtils::drawCollider(renderer, rect, {255, 0, 0, 255});
        }
    }

    notificationHandler.render(renderer, virtualRenderer()->getScreenWidth(), virtualRenderer()->getScreenHeight());

    this->hudRenderer->render(renderer, this->player);
    this->miniMapRenderer->render(renderer);
    SDL_RenderPresent(renderer);
}


void GameplayScene::loadResources(SDL_Renderer* renderer){
    textures()->Load(renderer, "player_b", "assets/player/personagem_B.png");
    textures()->Load(renderer, "player_f", "assets/player/personagem_F.png");
    textures()->Load(renderer, "player_l", "assets/player/personagem_L.png");
    textures()->Load(renderer, "player_r", "assets/player/personagem_R.png");
    textures()->Load(renderer, "player_sheet", "assets/animations/personagem_bruxa32x32-Sheet.png");
    textures()->Load(renderer, "swing", "assets/animations/swing.png");
    textures()->Load(renderer, "chain", "assets/animations/chain.png");
    textures()->Load(renderer, "pf", "assets/animations/pf_export.png");

    textures()->Load(renderer, "player_with_item", "assets/player_with_item.png");
    textures()->Load(renderer, "attack", "assets/attack.png");
    textures()->Load(renderer, "attack_destroy", "assets/attack_fade.png");

    textures()->Load(renderer, "bomb", "assets/bomb.png");
    textures()->Load(renderer, "bomb_explosion", "assets/bomb_explosion.png");
    textures()->Load(renderer, "hudsheet", "assets/hudsheet.png");

    textures()->Load(renderer, "door_open_anim", "assets/animations/door_open.png");

    audio()->loadSoundEffect("bomb_explosion", "assets/audio/explosion.mp3");
    audio()->loadSoundEffect("shoot", "assets/audio/shoot.mp3");
    audio()->loadSoundEffect("hit-enemy", "assets/audio/hit-enemy.mp3");
    audio()->loadSoundEffect("hit-player", "assets/audio/hit-player.mp3");
    audio()->loadSoundEffect("pickup-item", "assets/audio/pickup-item.mp3");
    audio()->loadSoundEffect("open-door", "assets/audio/open-door.mp3");

    this->enemyManager.loadFromFile("assets/data/enemies.json");
    this->tileSet.loadFromFile("assets/data/tileset.json");
    this->itemManager.loadFromFile("assets/data/items.json");
    this->bossManager.loadFromFile("assets/data/bosses.json");
    textures()->Load(renderer, "tileset", tileSet.getSpriteSheetPath());
    audio()->loadMusic("bg_game", "assets/music/bg_game.mp3");
    audio()->playMusic("bg_game", -1);
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

void GameplayScene::addSplashEffect(Vector2f position, Vector2f scale) {
    auto effect = std::make_unique<Entities::EffectBody>(
        position,
        scale,
        textures()->Get("splash"),
        0.5f
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

    delete this->miniMapRenderer;
    this->miniMapRenderer = nullptr;
}
