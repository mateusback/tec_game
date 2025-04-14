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
		
		PlayerBody(EBodyType type, float x = 0, float y = 0, float w = 50, float h = 50, bool collision = false, bool visible = true)
		: CharacterBody(type, x, y, w, h, collision, visible), money(0), key(0), bomb(0), experience(0), defense(0) {}
	public:
		#pragma region Constructors
		PlayerBody(float x = 0, float y = 0, float w = 50, float h = 50, bool collision = false, bool visible = true)
		: CharacterBody(EBodyType::Player, x, y, w, h, collision, visible), 
		money(0), 
		key(0), 
		bomb(0), 
		experience(0), 
		defense(0) {}

		PlayerBody(Vector pos, Vector scl, bool collision = false, bool visible = true)
		: CharacterBody(EBodyType::Player, pos.x, pos.y, scl.x, scl.y, collision, visible), 
		money(0), 
		key(0), 
		bomb(0), 
		experience(0), 
		defense(0) {}

		PlayerBody(Vector4 collider, bool collision = false, bool visible = true)
		: CharacterBody(EBodyType::Player, collider.x, collider.y, collider.z, collider.w, collision, visible), 
		money(0), 
		key(0), 
		bomb(0), 
		experience(0), 
		defense(0) {}
		#pragma endregion
	
		void handleCollision();
		void handleInput(const Manager::PlayerInput& input);
		void update(float deltaTime) override;
		std::unique_ptr<Entities::AttackBody> attack(Point characterCenter, Vector direction);
		void onCollision(Body* other) override;
		void pickUpItem(Entities::ItemBody* item);

		#pragma region Getters
		std::list<Items::Item> getInventory() { return this->inventory; }
		int getMoney() { return this->money; }
		int getKeys() { return this->key; }
		int getBombs() { return this->bomb; }
		float getExperience() { return this->experience; }
		float getDefense() { return this->defense; }
		Vector getPosition() const { return this->position; }
		EBodyType getBodyType() const override { this->bodyType; }
		#pragma endregion

		#pragma region Setters
		void setPosition(Vector position) { this->position = position; }
		#pragma endregion
	};
}
#endif
