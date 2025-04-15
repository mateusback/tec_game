#ifndef ENTITY_H
#define ENTITY_H

#include <SDL2/SDL.h>
#include "EntitiesTypes.h"

namespace Entities {
	class Entity {
	protected:
		bool active = true;
		const EBodyType bodyType;
	public:
		explicit Entity(EBodyType type) : bodyType(type) {}

		virtual ~Entity() = default;

		#pragma region Getters
		bool isActive() { return this->active; }
		EBodyType getBodyType() const { return this->bodyType; }
		#pragma endregion

		#pragma region Setters
		void setActive(bool active) {this->active = active;}
		#pragma endregion
	
	};
}

#endif
