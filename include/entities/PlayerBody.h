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
		int money;
		int key;
		int bomb;
		float experience;
		float defense;
		
	public:
		PlayerBody(float x = 0, float y = 0, float w = 50, float h = 50, bool collision = false, bool visible = true)
		: CharacterBody(x, y, w, h, collision, visible), money(0), key(0), bomb(0), experience(0), defense(0) {}
	
		void handleCollision();
		void handleInput(const Manager::PlayerInput& input);
		void handleInput();
		void update(float deltaTime) override;
		void attack(Point characterCenter, Vector direction);
		void onCollision(Body* other) override;
		void pickUpItem(Entities::ItemBody* item);

		EBodyType getBodyType() const override { return EBodyType::Player; }


		#pragma region Getters
		std::list<Items::Item> getInventory() { return this->inventory; }
		int getMoney() { return this->money; }
		int getKeys() { return this->key; }
		int getBombs() { return this->bomb; }
		float getExperience() { return this->experience; }
		float getDefense() { return this->defense; }
		#pragma endregion

		#pragma region Setters
		#pragma endregion
	};
}
#endif
