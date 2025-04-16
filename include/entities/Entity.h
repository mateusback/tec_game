#ifndef ENTITY_H
#define ENTITY_H

#include <SDL2/SDL.h>

namespace Entities {
	class Entity {
	protected:
		bool active = true;
	public:
		#pragma region Getters
		bool isActive() { return this->active; }
		#pragma endregion

		#pragma region Setters
		void setActive(bool active) {this->active = active;}
		#pragma endregion
	
	};
}

#endif
