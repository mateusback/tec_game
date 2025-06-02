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
        ItemBody(Vector4f collider, const Item& item, bool collision = true, bool visible = true)
            : Body(collider, collision, visible),
            type(item.getType()),
            item(item) {}

        ItemBody(Vector2f pos, Vector2f scl, const Item& item, bool collision = true, bool visible = true)
            : ItemBody(Vector4f(pos.x, pos.y, scl.x, scl.y), item, collision, visible) {}

        // ItemBody(const ItemBody& other)
        //     : Body(other),
        //     type(other.type),
        //     item(other.item) {
        //     this->setTexture(other.getTexture());
        //     this->setPosition(other.getPosition());
        // }
        #pragma endregion
    
        #pragma region Getters
        EItemType getType() const { return this->type; }
        const Item& getItem() const { return item; }
        #pragma endregion

        #pragma region Setters
        void setType(EItemType type) { this->type = type;}
        void setItem(const Item& item) { this->item = item; }
        #pragma endregion
    
        void update(float deltaTime) override;
        void onCollision(Body* other) override;
        void loadAnimations() override;
    };
}


#endif
