#ifndef BOSSBODY_H
#define BOSSBODY_H
#include "CharacterBody.h"
#include "PlayerBody.h"
#include "AttackBody.h"
#include "../enemies/BossData.h"
#include "../managers/EntityManager.h"

namespace Entities {
    struct PendingAttack {
        Vector2f position;
        float timer;
    };

    enum class BossPhase {
        Normal = 0,
        Enraged = 1
    };
    
    class BossBody : public CharacterBody {
    private:
        // Dados básicos (mantidos)
        Enemies::BossData bossData;
        Manager::EntityManager& entityManager;
        PlayerBody* target = nullptr;
        float aggroRange = 0.f;
        float stateTimer = 0.f;
        EntityState state = EntityState::Idle;
        std::vector<PendingAttack> scheduledAttacks;
        
        // Melhorias simples
        BossPhase currentPhase = BossPhase::Normal;
        int attackPattern = 0; // 0=normal, 1=círculo, 2=seguir jogador
        float patternTimer = 0.f;
        float originalSpeed;
        float originalAttackRate;
        
        // Movimento melhorado
        Vector2f lastPlayerPosition;
        bool shouldMoveToCenter = false;
        float moveTimer = 0.f;
        
    public:
        BossBody(Vector4f collider, const Enemies::BossData& data, Manager::EntityManager& entityManager);
        void applyBossBehavior(float deltaTime);
        void update(float deltaTime) override;
        void loadAnimations();
        void setAnimationByState();
        
        // Padrões de ataque simples
        void summonAttacksAroundPlayer();
        void summonCircularAttack();
        void summonFollowingAttacks();
        
        // Sistema de fases simples
        void checkPhaseTransition();
        void switchAttackPattern();
        
        void onCollision(Body* other) override;
        
        // Getters/Setters (mantidos)
        inline void setTarget(PlayerBody* player) { this->target = player; }
        inline void setAggroRange(float value) { this->aggroRange = value; }
        inline float getAggroRange() const { return this->aggroRange; }
        inline Enemies::BossData& getBossData() { return this->bossData; }
        inline BossPhase getCurrentPhase() const { return currentPhase; }
    };
}
#endif