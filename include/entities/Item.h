#ifndef ITEM_H
#define ITEM_H

#include <string>
	enum class EItemType {
		Passive, Active
	};
namespace Entities
{

	
	class Item {
	protected:
		std::string name;
		std::string description;
		int quality;
		EItemType type;
		//TODO - list<ESala> salas;
	public:
		Item(std::string name = "", std::string description = "", int quality = 0, EItemType type = EItemType::Passive)
			: name(name), description(description), quality(quality), type(type) {}

		 const std::string& getName() const {
			return this->name;
		}

		std::string getDescription() const {
			return this->description;
		}

		int getQuality() const {
			return this->quality;
		}

		EItemType getType() const {
			return this->type;
		}
	};
} 
#endif
