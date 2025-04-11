#ifndef ITEM_BODY_H
#define ITEM_BODY_H

#include "Body.h"
#include "../items/Item.h"

using namespace Items;

namespace Entities
{
    class ItemBody : public Body 
    {
    protected:
        EItemType type;
        Item item;
        
    public:
        ItemBody(Vector4 hitbox, const Item& item, bool collision = true, bool visible = true)
        : Body(hitbox, collision, visible), type(item.getType()), item(item) {}
    
        void setType(EItemType type) { this->type = type;}
    
        EItemType getType() const { return this->type; }
        const Item& getItem() const { return item; }
    
        void update(float deltaTime) override {};
        void onCollision(Body* other);
    };
}


#endif
