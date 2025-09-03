#include "../../include/entities/BossBody.h"

#include <iostream>
#include <cstdlib>
#include <set>
#include <cmath>

#include "../../include/managers/TextureManager.h"
#include "../../include/managers/AnimationLoader.h"
#include "../../include/entities/EffectBody.h"
#include "../../include/utils/GlobalAccess.h"

namespace Entities {

    BossBody::BossBody(Vector4f collider, const Enemies::BossData& data, Manager::EntityManager& entityManager)
        : CharacterBody(collider, true, true, 0.f),
          bossData(data),
          entityManager(entityManager)
    {
        this->setTexture(textures()->Get(data.getSpritePath()));
        this->setHealth(data.getHealth());
        this->setMaxHealth(data.getMaxHealth());
        this->setAttackDamage(data.getAttackDamage());
        this->setAttackRate(data.getAttackRate());
        this->setAttackRange(data.getAttackRange());
        this->setAggroRange(data.getAggroRange());
        
        // Salvar valores originais
        this->originalSpeed = bossData.getAcceleration();
        this->originalAttackRate = data.getAttackRate();
        
        this->hitboxOffset = Vector2f(0.1f, 0.1f);
        this->hitboxSize = Vector2f(0.8f, 0.8f);
        this->setState(EntityState::Idle);
        this->loadAnimations();
        this->setScale(virtualRenderer()->getTileSize() * 3, virtualRenderer()->getTileSize() * 3);
        
        // Inicializar
        this->currentPhase = BossPhase::Normal;
        this->attackPattern = 0;
        this->lastPlayerPosition = Vector2f(0, 0);
    }

    void BossBody::update(float deltaTime) {
        if (!this->isActive()) return;

        this->checkPhaseTransition();
        this->applyBossBehavior(deltaTime);

        if (this->is_animated) {
            this->animationManager.update(deltaTime);
        }

        // Processar ataques agendados (mantido igual)
        for (auto it = scheduledAttacks.begin(); it != scheduledAttacks.end(); ) {
            it->timer -= deltaTime;
            if (it->timer <= 0.f) {
                float tile = virtualRenderer()->getTileSize();
                Vector2f pos = it->position;

                auto attack = std::make_unique<AttackBody>(
                    pos.x - tile / 2.f,
                    pos.y - tile / 2.f,
                    tile,
                    tile,
                    false,
                    true,
                    this->getAttackDamage(),
                    this->getAttackRange(),
                    2.0f,
                    0.1f,
                    2.0f,
                    1.5f
                );
                
                audio()->playSoundEffect("chain", 0);
                attack->setSpeed({0, 0});
                attack->setTexture(textures()->Get("chain"));
                attack->setOrigin(this);

                entityManager.add(std::move(attack));
                it = scheduledAttacks.erase(it);
            } else {
                ++it;
            }
        }
        
        // Atualizar timers
        patternTimer += deltaTime;
        if (moveTimer > 0.f) moveTimer -= deltaTime;
    }

    void BossBody::applyBossBehavior(float deltaTime) {
        if (!target || !target->isActive()) return;

        auto toPlayer = target->getCenterPoint() - this->getCenterPoint();
        float distance = toPlayer.length();

        // Movimento melhorado
        if (shouldMoveToCenter && moveTimer <= 0.f) {
            // Mover para uma posição estratégica
            Vector2f centerOfArena = Vector2f(0, 0); // Ajuste conforme sua arena
            Vector2f toCenter = centerOfArena - this->getCenterPoint();
            
            if (toCenter.length() > 1.0f) {
                this->speed = toCenter * originalSpeed * 1.5f;
                this->move(deltaTime);
            } else {
                shouldMoveToCenter = false;
            }
        } else if (distance > 1.0f && !shouldMoveToCenter) {
            // Movimento normal em direção ao jogador (mais inteligente)
            Vector2f direction = {toPlayer.x, toPlayer.y};
            
            // Velocidade baseada na fase
            float currentSpeed = originalSpeed;
            if (currentPhase == BossPhase::Enraged) {
                currentSpeed *= 1.5f; // 50% mais rápido quando enfurecido
            }
            
            this->speed = direction * currentSpeed;
            this->move(deltaTime);
        } else {
            this->speed = Vector2f(0, 0);
        }

        // Salvar posição do jogador para previsão
        lastPlayerPosition = target->getCenterPoint();

        // Máquina de estados melhorada
        switch (state) {
            case EntityState::Idle:
                this->stateTimer -= deltaTime;
                if (this->stateTimer <= 0.f) {
                    this->state = EntityState::Attacking;
                    this->stateTimer = 1.0f; // Tempo de ataque
                    this->switchAttackPattern(); // Escolher padrão
                }
                break;

            case EntityState::Attacking:
                this->stateTimer -= deltaTime;
                
                // Executar ataque baseado no padrão atual
                if (this->stateTimer <= 0.f) {
                    switch (attackPattern) {
                        case 0:
                            this->summonAttacksAroundPlayer();
                            break;
                        case 1:
                            this->summonCircularAttack();
                            break;
                        case 2:
                            this->summonFollowingAttacks();
                            break;
                    }
                    
                    this->state = EntityState::CoolingDown;
                    
                    // Cooldown baseado na fase
                    float cooldown = this->getAttackRate();
                    if (currentPhase == BossPhase::Enraged) {
                        cooldown *= 0.7f; // 30% mais rápido
                    }
                    this->stateTimer = cooldown;
                }
                break;

            case EntityState::CoolingDown:
                this->stateTimer -= deltaTime;
                if (this->stateTimer <= 0.f) {
                    this->state = EntityState::Idle;
                    this->stateTimer = 0.5f;
                    
                    if (rand() % 100 < 20) { 
                        shouldMoveToCenter = true;
                        moveTimer = 2.0f;
                    }
                }
                break;

            default:
                break;
        }

        this->setAnimationByState();
    }

    void BossBody::checkPhaseTransition() {
        float healthPercent = (float)getHealth() / (float)getMaxHealth();
        
        // Transição para fase enfurecida quando vida < 50%
        if (healthPercent < 0.5f && currentPhase == BossPhase::Normal) {
            currentPhase = BossPhase::Enraged;
            
            // Efeito visual simples usando sprites existentes
            auto effect = std::make_unique<EffectBody>(
                getCenterPoint() - Vector2f(virtualRenderer()->getTileSize(), virtualRenderer()->getTileSize()),
                Vector2f(virtualRenderer()->getTileSize() * 2, virtualRenderer()->getTileSize() * 2),
                textures()->Get("portal_spawn"), // Reutilizar sprite existente
                2.0f
            );
            effect->setAnimationInfo({ "spin", 0, 8 });
            effect->loadAnimations();
            entityManager.add(std::move(effect));
            
            audio()->playSoundEffect("portal"); // Som existente
            
            // Aumentar dano
            this->setAttackDamage(this->getAttackDamage() + 10);
        }
    }

    void BossBody::switchAttackPattern() {
        if (currentPhase == BossPhase::Normal) {
            // Fase normal: apenas padrões 0 e 1
            attackPattern = rand() % 2;
        } else {
            // Fase enfurecida: todos os padrões
            attackPattern = rand() % 3;
        }
        patternTimer = 0.f;
    }

    void BossBody::setAnimationByState() {
        switch (state) {
            case EntityState::Idle:
                this->animationManager.setAnimation("idle");
                break;
            case EntityState::Attacking:
                this->animationManager.setAnimation("attack");
                break;
            case EntityState::CoolingDown:
                this->animationManager.setAnimation("cooldown");
                break;
            default:
                this->animationManager.setAnimation("idle");
        }
    }

    // Padrão original (melhorado)
    void BossBody::summonAttacksAroundPlayer() {
        Vector2f playerCenter = this->target->getCenterPoint();
        float tile = virtualRenderer()->getTileSize();

        // Mais ataques na fase enfurecida
        int attackCount = (currentPhase == BossPhase::Enraged) ? 9 : 6;
        
        for (int i = 0; i < attackCount; ++i) { 
            float offsetX = ((rand() % 5) - 2) * tile;
            float offsetY = ((rand() % 5) - 2) * tile;
            Vector2f centerPos = playerCenter + Vector2f(offsetX, offsetY);

            // Efeito de spawn usando sprite existente
            auto effect = std::make_unique<EffectBody>(
                centerPos - Vector2f(tile / 2.f, tile / 2.f),
                Vector2f(tile, tile),
                textures()->Get("portal_spawn"),
                1.0f
            );
            effect->setAnimationInfo({ "spin", 0, 8 });
            effect->loadAnimations();
            entityManager.add(std::move(effect));

            // Delay menor na fase enfurecida
            float delay = (currentPhase == BossPhase::Enraged) ? 0.3f : 0.5f;
            scheduledAttacks.push_back({ centerPos, delay });
        }

        audio()->playSoundEffect("portal");
    }

    // Novo padrão: ataques em círculo
    void BossBody::summonCircularAttack() {
        Vector2f bossCenter = this->getCenterPoint();
        float tile = virtualRenderer()->getTileSize();
        float radius = 4.0f * tile;
        int attackCount = 8;

        for (int i = 0; i < attackCount; ++i) {
            float angle = (2.0f * 3.14f * i) / attackCount;
            Vector2f pos = bossCenter + Vector2f(cos(angle) * radius, sin(angle) * radius);

            auto effect = std::make_unique<EffectBody>(
                pos - Vector2f(tile / 2.f, tile / 2.f),
                Vector2f(tile, tile),
                textures()->Get("portal_spawn"),
                0.8f
            );
            effect->setAnimationInfo({ "spin", 0, 8 });
            effect->loadAnimations();
            entityManager.add(std::move(effect));

            scheduledAttacks.push_back({ pos, 0.8f });
        }
        
        audio()->playSoundEffect("portal");
    }

    // Novo padrão: ataques que seguem o jogador
    void BossBody::summonFollowingAttacks() {
        Vector2f playerPos = target->getCenterPoint();
        Vector2f playerVelocity = playerPos - lastPlayerPosition;
        float tile = virtualRenderer()->getTileSize();
        
        // Prever onde o jogador vai estar
        Vector2f predictedPos = playerPos + playerVelocity * 30.0f; // Multiplicador de predição

        int attackCount = (currentPhase == BossPhase::Enraged) ? 5 : 3;
        
        for (int i = 0; i < attackCount; ++i) {
            Vector2f targetPos = predictedPos + Vector2f(
                ((rand() % 3) - 1) * tile, // Pequena variação
                ((rand() % 3) - 1) * tile
            );

            auto effect = std::make_unique<EffectBody>(
                targetPos - Vector2f(tile / 2.f, tile / 2.f),
                Vector2f(tile, tile),
                textures()->Get("portal_spawn"),
                1.2f
            );
            effect->setAnimationInfo({ "spin", 0, 8 });
            effect->loadAnimations();
            entityManager.add(std::move(effect));

            scheduledAttacks.push_back({ targetPos, 1.2f + i * 0.2f });
        }
        
        audio()->playSoundEffect("portal");
    }

    void BossBody::loadAnimations() {
        this->is_animated = true;

        SDL_Texture* texture = Manager::TextureManager::Get(this->bossData.getSpritePath());

        // Manter animações simples com suas sprites
        Manager::AnimationLoader::loadStaticAnimations(texture, {
            {"idle", 0, 0},
            {"attack", 0, 0},
            {"cooldown", 0, 0},
        }, this->animationManager, 96, 96);

        this->animationManager.setAnimation("idle");
    }

    void BossBody::onCollision(Body* other) {
        if (!other || !other->isActive()) return;

        auto* player = dynamic_cast<PlayerBody*>(other);
        if (player) {
            int damage = this->getAttackDamage();
            
            // Dano extra na fase enfurecida
            if (currentPhase == BossPhase::Enraged) {
                damage = (int)(damage * 1.2f);
            }
            
            player->takeDamage(damage);
            
            // Empurrar jogador para longe
            Vector2f direction = (player->getCenterPoint() - this->getCenterPoint());
            float knockbackForce = (currentPhase == BossPhase::Enraged) ? 150.0f : 100.0f;
            player->setSpeed(direction * knockbackForce);
        }
    }
}