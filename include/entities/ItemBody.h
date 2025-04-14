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
        #pragma region Constructors
        ItemBody(Vector4 collider, const Item& item,
            bool collision = false, bool visible = true)
        : Body(EBodyType::Item, collider.x, collider.y, collider.z, collider.w, collision, visible),
            type(item.getType()), item(item) {}

        ItemBody(Vector pos, Vector scl, const Item& item,
            bool collision = false, bool visible = true)
        : Body(EBodyType::Item, pos.x, pos.y, scl.x, scl.y, collision, visible),
            type(item.getType()), item(item) {}
        #pragma endregion
    
        #pragma region Getters
        EItemType getType() const { return this->type; }
        const Item& getItem() const { return item; }
        #pragma endregion

        #pragma region Setters
        void setType(EItemType type) { this->type = type;}
        void setItem(const Item& item) { this->item = item; }
        #pragma endregion
    
        void update(float deltaTime) override {};
        void onCollision(Body* other);
    };
}


#endif
