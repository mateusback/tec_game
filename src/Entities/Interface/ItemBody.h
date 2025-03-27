#ifndef ITEM_BODY_H
#define ITEM_BODY_H

#include "Body.h"
#include "Item.h"
//usar enum classe 
enum EItemPool {Boss, Floor, Chest};
//enum EItemType {Passive, Active};


class ItemBody : public Entites::Body {
protected:
	EItemPool pool;
    EItemType type;

public:
    ItemBody(SDL_FRect hitbox, bool collision = true, bool visible = true, EItemPool pool = EItemPool::Floor, EItemType type = EItemType::Passive)
    : Entites::Body(hitbox, collision, visible), pool(pool), type(type) {};

    void SetPool(EItemPool pool) {
        this->pool = pool;
    }

    EItemPool GetPool() const {
        return this->pool;
    }

    void SetType(EItemType type) {
        this->type = type;
    }

    EItemType GetType() const {
        return this->type;
    }

    void Update(float deltaTime) override {};

};

#endif
