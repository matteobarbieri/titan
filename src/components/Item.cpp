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

Item::Item(ItemType item_type, ItemSubtype item_subtype) :
    item_type(item_type), item_subtype(item_subtype), equipped(false)
{
}

Item::~Item()
{
}

/*
bool Item::equipped()
{
    return _equipped;
}
*/

json Item::to_json()
{
    json j;

    j["item_letter"] = item_letter;
    j["equipped"] = equipped;
    j["item_subtype"] = item_subtype;
    j["item_type"] = item_type;

    return j;
}

Item * Item::from_json(json j)
{
    Item * c = new Item(j["item_type"], j["item_subtype"]);

    // TODO complete with letter and equipped status

    return c;
}


std::vector<MenuOption> Item::item_inventory_options()
{
    std::vector<MenuOption> subitem_options;

    // TODO change options based on item type and condition

    // Every item can be dropped
    subitem_options.push_back({'d', "Drop"});

    // Change option displayed based on "equipped" status
    if (equipped) {
        subitem_options.push_back({'e', "UnEquip"});
    } else {
        subitem_options.push_back({'e', "Equip"});
    }

    // Only usable items can be used (no shit!)
    /*
     *if (owner->usable != nullptr)
     *{
     *    subitem_options.push_back({'u', "Use"});
     *}
     */

    return subitem_options;
}

/*

class Item:
    """
    Class describing an item
    """

    def is_armor(self):
        return ItemType.ARMOR in self.item_types

    def is_weapon(self):
        return ItemType.WEAPON in self.item_types

    def is_device(self):
        return ItemType.DEVICE in self.item_types

    def is_equippable(self):
        return self.is_armor() or self.is_weapon() or self.is_device()

    def is_consumable(self):
        return ItemType.CONSUMABLE in self.item_types

    def is_melee(self):
        return ItemSubtype.MELEE in self.item_subtypes

    def is_ranged(self):
        return ItemSubtype.RANGED in self.item_subtypes
*/
