#ifndef ITEM_BODY_H
#define ITEM_BODY_H

#include "Body.h"
#include "Item.h"
enum EItemPool {Boss, Floor, Chest};

namespace Entities
{
    class ItemBody : public Body 
    {
    protected:
        EItemPool pool;
        EItemType type;
        Item item;
        
    public:
        ItemBody(SDL_FRect hitbox, const Item& item, bool collision = true, bool visible = true, EItemPool pool = EItemPool::Floor)
        : Body(hitbox, collision, visible), pool(pool), type(item.getType()), item(item) {}
    
    
        void setPool(EItemPool pool) { this->pool = pool; }
    
        EItemPool getPool() const { return this->pool; }
    
        void setType(EItemType type) { this->type = type;}
    
        EItemType getType() const { return this->type; }
        const Item& getItem() const { return item; }
    
        void update(float deltaTime) override {};
        void onCollision(Body* other);
    };
}


#endif
