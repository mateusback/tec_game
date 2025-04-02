#ifndef ITEM_BODY_H
#define ITEM_BODY_H

#include "Body.h"
#include "Item.h"
//usar enum classe 
enum EItemPool {Boss, Floor, Chest};
//enum EItemType {Passive, Active};

class ItemBody : public Entities::Body {
protected:
	EItemPool pool;
    EItemType type;

public:
    ItemBody(SDL_FRect hitbox, bool collision = true, bool visible = true, EItemPool pool = EItemPool::Floor, EItemType type = EItemType::Passive)
    : Entities::Body(hitbox, collision, visible), pool(pool), type(type) {};

    void setPool(EItemPool pool) {
        this->pool = pool;
    }

    EItemPool getPool() const {
        return this->pool;
    }

    void setType(EItemType type) {
        this->type = type;
    }

    EItemType getType() const {
        return this->type;
    }

    void update(float deltaTime) override {};

};

#endif
