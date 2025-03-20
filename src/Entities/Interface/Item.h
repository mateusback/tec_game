#ifndef ITEM_H
#define ITEM_H

#include <string>

enum EItemType{
	Passive = 1,
	Active = 2
};

class Item {
	std::string name;
	std::string description;
	int quality;
	EItemType type;
	//TODO - list<ESala> salas;
};

#endif
