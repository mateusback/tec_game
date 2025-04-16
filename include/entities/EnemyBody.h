#ifndef ENEMY_BODY_H
#define ENEMY_BODY_H

#include "CharacterBody.h"
#include "PlayerBody.h"

namespace Entities
{
    struct ProjectileData {
        std::string spritePath;
    };

    struct EnemyBehavior {
		bool collisionDuringAttack = false;
		bool chasePlayer = false;
		float speed = 0.0f;
		ProjectileData projectileData;
	};

    class EnemyBody : public CharacterBody {
    protected:
        PlayerBody* target;
        float aggro_range;
        EnemyBehavior behavior;
        Uint32 stateTimer = 0;

        EnemyBody(float x = 0, float y = 0, float w = 50, float h = 50, bool collision = false, bool visible = true, float acceleration = 100.0f)
            : CharacterBody(x, y, w, h, collision, visible, acceleration), target(nullptr),  aggro_range(100.0f) {}
    public:
        #pragma region Constructors
        EnemyBody(float x = 0, float y = 0, float w = 50, float h = 50,
                    bool collision = false, bool visible = true, float acceleration = 100.0f)
            : CharacterBody(x, y, w, h, collision, visible, acceleration),
            target(nullptr), 
            aggro_range(100.0f) {}

        EnemyBody(Vector pos, Vector scl,
                    bool collision = false, bool visible = true, float acceleration = 100.0f)
            : CharacterBody(pos.x, pos.y, scl.x, scl.y, collision, visible, acceleration),
            target(nullptr), 
            aggro_range(100.0f) {}

        EnemyBody(Vector4 collider,
                    bool collision = false, bool visible = true, float acceleration = 100.0f)
            : CharacterBody(collider.x, collider.y, collider.z, collider.w, collision, visible, acceleration),
            target(nullptr), 
            aggro_range(100.0f) {}
        #pragma endregion

		#pragma region Getters
        PlayerBody* getTarget() const { return target; }
		float getAggroRange() const { return this->aggro_range; }
		#pragma endregion

		#pragma region Setters
        void setTarget(PlayerBody* target) { this->target = target; }
        void setAggroRange(float range) { this->aggro_range = range; }
        void setBehavior(const EnemyBehavior& b) { behavior = b; }
        void setStateTimer(Uint32 timer) { stateTimer = timer; }
		#pragma endregion
    
        void update(float deltaTime) override {
            stateTimer += deltaTime;
        }
    
        void attack() {
    
        }
    
    };
}

#endif
