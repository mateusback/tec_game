#ifndef PLAYER_BODY_H
#define PLAYER_BODY_H

#include <list>

#include "CharacterBody.h"
#include "Item.h"

namespace Entites
{
	class PlayerBody : public CharacterBody {
	protected:
		std::list<Item> inventory;
		int money;
		int key;
		int bomb;
		float experience;
		float defense;
		float life_steal;
		
	public:
		PlayerBody(float x = 0, float y = 0, float w = 50, float h = 50, bool collision = false, bool visible = true)
		: CharacterBody(x, y, w, h, collision, visible), money(0), key(0), bomb(0), experience(0), defense(0), life_steal(0) {}

		void HandleInput(const Uint8* keystates);
		void Update(float deltaTime) override;
		void Attack(float x, float y, float vx, float vy);
		void Render(SDL_Renderer* renderer);
	};
}
#endif
