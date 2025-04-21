#ifndef PLAYER_BODY_H
#define PLAYER_BODY_H

#include <list>

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
		float experience;

	public:
		#pragma region Constructors
		PlayerBody(float x = 0, float y = 0, float w = 50, float h = 50, bool collision = false, bool visible = true)
		: CharacterBody(x, y, w, h, collision, visible), 
		coins(0), 
		keys(0), 
		bombs(0), 
		experience(0) {}

		PlayerBody(Vector pos, Vector scl, bool collision = false, bool visible = true)
		: CharacterBody(pos.x, pos.y, scl.x, scl.y, collision, visible), 
		coins(0), 
		keys(0), 
		bombs(0), 
		experience(0) {}

		PlayerBody(Vector4 collider, bool collision = false, bool visible = true)
		: CharacterBody(collider.x, collider.y, collider.z, collider.w, collision, visible), 
		coins(0), 
		keys(0), 
		bombs(0), 
		experience(0) {}
		#pragma endregion
	

		void onCollision(Body* other) override;
		void update(float deltaTime) override;
		
		void handleInput(const Manager::PlayerInput& input);
		std::unique_ptr<Entities::AttackBody> attack(Point characterCenter, Vector direction);
		void pickUpItem(Entities::ItemBody* item);
		void updateDirectionSprite(const Vector& direction);

		#pragma region Getters
		std::list<Items::Item> getInventory() { return this->inventory; }
		int getMoney() { return this->coins; }
		int getKeys() { return this->keys; }
		int getBombs() { return this->bombs; }
		float getExperience() { return this->experience; }
		#pragma endregion

		#pragma region Setters
		#pragma endregion
	};
}
#endif
