#ifndef PLAYER_BODY_H
#define PLAYER_BODY_H

#include <list>
#include <memory>

#include "CharacterBody.h"
#include "ItemBody.h"
#include "../items/Item.h"
#include "../managers/InputManager.h"

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

	public:
		#pragma region Constructors
		PlayerBody(float x = 0, float y = 0, float w = 50, float h = 50, bool collision = false, bool visible = true)
		: CharacterBody(x, y, w, h, collision, visible), 
		coins(0), 
		keys(0), 
		bombs(0), 
		experience(0) {}

		PlayerBody(Vector2f pos, Vector2f scl, bool collision = false, bool visible = true)
		: CharacterBody(pos.x, pos.y, scl.x, scl.y, collision, visible), 
		coins(0), 
		keys(0), 
		bombs(0), 
		experience(0) {}

		PlayerBody(Vector4f collider, bool collision = false, bool visible = true)
		: CharacterBody(collider.x, collider.y, collider.z, collider.w, collision, visible), 
		coins(0), 
		keys(0), 
		bombs(0), 
		experience(0) {}
		#pragma endregion
	

		void onCollision(Body* other) override;
		void update(float deltaTime) override;
		void loadAnimations() override;
		
		void handleInput(const Manager::PlayerInput& input);
		std::unique_ptr<Entities::AttackBody> attack(Pointf characterCenter, Vector2f direction);
		void pickUpItem(Entities::ItemBody* item);
		void updateDirectionSprite(const Vector2f& direction);

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
		#pragma endregion
	};
}
#endif
