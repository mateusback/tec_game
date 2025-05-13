#ifndef PLAYER_BODY_H
#define PLAYER_BODY_H

#include <list>
#include <memory>

#include "CharacterBody.h"
#include "ItemBody.h"
#include "../items/Item.h"
#include "../managers/InputManager.h"
#include "../handlers/WeaponHandler.h"
#include "../managers/EntityManager.h"
#include "../weapons/MagicHandsWeapon.h"

namespace Entities
{
	class PlayerBody : public CharacterBody {
	protected:
		std::list<Items::Item> inventory;
		uint8_t coins;
		uint8_t keys;
		uint8_t bombs;
		float bombCooldown = 5.0f;
		float experience;
		EDirection currentDirection = EDirection::Down;
		WeaponHandler weaponHandler;

	public:
		#pragma region Constructors
		PlayerBody(Vector4f collider, Manager::EntityManager* entityManager, bool collision = false, bool visible = true)
		: CharacterBody(collider.x, collider.y, collider.z, collider.w, collision, visible), 
		coins(0), keys(0), bombs(0), experience(0),
		weaponHandler(entityManager)
		{
			this->loadAnimations();
			this->setAttackRate(1.0f);
			this->setAttackSpeed(3.5f);
			this->loadAnimations();
			this->setAcceleration(virtualRenderer()->normalizeValue(3));
			this->setHitboxMargin(0.7f, 0.3f);
			this->setBombs(3);

			auto weapon = std::make_shared<MagicHandsWeapon>();
			weapon->setEntityManager(entityManager);
			weaponHandler.setWeapon(weapon);
			weaponHandler.getWeapon()->setOwner(this);
		}

		#pragma endregion
	
		void takeDamage(float damage) override;
		void onCollision(Body* other) override;
		void update(float deltaTime) override;
		void loadAnimations() override;
		
		void handleInput(const Manager::PlayerInput& input);
		void attack(Vector2f direction);
		void pickUpItem(Entities::ItemBody* item);
		void tryPlaceBomb();
		void consumeBomb();

		#pragma region Getters
		const std::list<Items::Item>& getInventory() const { return this->inventory; }
		uint8_t getCoins() const { return this->coins; }
		uint8_t getKeys() const { return this->keys; }
		uint8_t getBombs() const { return this->bombs; }
		float getExperience() const { return this->experience; }
		float getBombCooldown() const { return this->bombCooldown; }
		#pragma endregion

		#pragma region Setters
		void setCoins(uint8_t value) { this->coins = value; }
		void setKeys(uint8_t value) { this->keys = value; }
		void setBombs(uint8_t value) { this->bombs = value; }
		void setBombCooldown(float cooldown) { this->bombCooldown = cooldown; }
		void setWeapon(std::shared_ptr<Weapon> weapon, Manager::EntityManager* entityManager);
		#pragma endregion
	};
}
#endif
