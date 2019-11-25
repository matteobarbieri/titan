#include <vector>

#include "Item.hpp"
#include "../Entity.hpp"

#include "../ui/menus.hpp"

//////////////////////////////////////
/////// ITEM TYPE OPERATORS //////////
//////////////////////////////////////

// Enable bitwise comparison for ItemType
ItemType operator |(ItemType lhs, ItemType rhs)
{
    return static_cast<ItemType> (
        static_cast<std::underlying_type<ItemType>::type>(lhs) |
        static_cast<std::underlying_type<ItemType>::type>(rhs)
    );
}

ItemType operator &(ItemType lhs, ItemType rhs)
{
    return static_cast<ItemType> (
        static_cast<std::underlying_type<ItemType>::type>(lhs) &
        static_cast<std::underlying_type<ItemType>::type>(rhs)
    );
}

ItemType operator ^(ItemType lhs, ItemType rhs)
{
    return static_cast<ItemType> (
        static_cast<std::underlying_type<ItemType>::type>(lhs) ^
        static_cast<std::underlying_type<ItemType>::type>(rhs)
    );
}

ItemType operator ~(ItemType rhs)
{
    return static_cast<ItemType> (
        ~static_cast<std::underlying_type<ItemType>::type>(rhs)
    );
}

ItemType& operator &=(ItemType &lhs, ItemType rhs)
{
    lhs = static_cast<ItemType> (
        static_cast<std::underlying_type<ItemType>::type>(lhs) &
        static_cast<std::underlying_type<ItemType>::type>(rhs)
    );

    return lhs;
}

//////////////////////////////////////
////// ITEM SUBTYPE OPERATORS ////////
//////////////////////////////////////

// Enable bitwise comparison for ItemSubtype
ItemSubtype operator |(ItemSubtype lhs, ItemSubtype rhs)
{
    return static_cast<ItemSubtype> (
        static_cast<std::underlying_type<ItemSubtype>::type>(lhs) |
        static_cast<std::underlying_type<ItemSubtype>::type>(rhs)
    );
}

ItemSubtype operator &(ItemSubtype lhs, ItemSubtype rhs)
{
    return static_cast<ItemSubtype> (
        static_cast<std::underlying_type<ItemSubtype>::type>(lhs) &
        static_cast<std::underlying_type<ItemSubtype>::type>(rhs)
    );
}

ItemSubtype operator ^(ItemSubtype lhs, ItemSubtype rhs)
{
    return static_cast<ItemSubtype> (
        static_cast<std::underlying_type<ItemSubtype>::type>(lhs) ^
        static_cast<std::underlying_type<ItemSubtype>::type>(rhs)
    );
}

ItemSubtype operator ~(ItemSubtype rhs)
{
    return static_cast<ItemSubtype> (
        ~static_cast<std::underlying_type<ItemSubtype>::type>(rhs)
    );
}

ItemSubtype& operator &=(ItemSubtype &lhs, ItemSubtype rhs)
{
    lhs = static_cast<ItemSubtype> (
        static_cast<std::underlying_type<ItemSubtype>::type>(lhs) &
        static_cast<std::underlying_type<ItemSubtype>::type>(rhs)
    );

    return lhs;
}

//////////////////////////////////////
//////// ITEM CLASS METHODS //////////
//////////////////////////////////////

Item::Item(ItemType item_type, ItemSubtype item_subtype, bool equipped, unsigned int key_id) :
    item_type(item_type), item_subtype(item_subtype),
    equipped(equipped), key_id(key_id)
{
    // Owner must be set manually
    owner = nullptr;
}

json Item::to_json()
{
    json j;

    j["item_letter"] = item_letter;
    j["item_type"] = item_type;
    j["item_subtype"] = item_subtype;
    j["equipped"] = equipped;
    j["key_id"] = key_id;

    return j;
}

Item * Item::from_json(json j)
{
    Item * c = new Item(
        j["item_type"], j["item_subtype"],
        j["equipped"], j["key_id"]
    );

    c->item_letter = j["item_letter"];

    return c;
}


std::vector<MenuOption> Item::item_inventory_options()
{
    std::vector<MenuOption> subitem_options;

    // Every item can be dropped
    subitem_options.push_back({'d', "Drop"});

    // Only for actually equippable items
    if (owner->equippable != nullptr)
    {
        // Change option displayed based on "equipped" status
        if (equipped) {
            subitem_options.push_back({'e', "UnEquip"});
        } else {
            subitem_options.push_back({'e', "Equip"});
        }
    }

    // Only usable items can be used (no shit!)
    if (owner->usable != nullptr)
    {
        subitem_options.push_back({'u', "Use"});
    }

    return subitem_options;
}
