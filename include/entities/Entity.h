#ifndef ENTITY_H
#define ENTITY_H

#include <SDL2/SDL.h>
#include "EntitiesTypes.h"

namespace Entities {
	class Entity {
	protected:
		bool active = true;
	public:
		virtual ~Entity() = default;
		virtual void update(float deltaTime) = 0;
		virtual void render(SDL_Renderer* renderer);

		#pragma region Getters
		bool isActive() { return this->active; }
		#pragma endregion

		#pragma region Setters
		void setActive(bool active) {this->active = active;}
		#pragma endregion
	
	}
};

#endif
