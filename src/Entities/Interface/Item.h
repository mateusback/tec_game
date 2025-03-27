#ifndef ITEM_H
#define ITEM_H

#include <string>
namespace Entites
{

	enum EItemType{
		Passive = 1,
		Active = 2
	};

	class Item {
	protected:
		std::string name;
		std::string description;
		int quality;
		EItemType type;
		//TODO - list<ESala> salas;
	public:
		Item(std::string name = "", std::string description = "", int quality = 0, EItemType type = Passive)
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
