#include "../libtcod.hpp"

#include "../Symbols.h"

#include "../Entity.hpp"

#include "../components/Item.hpp"
#include "../components/Equippable.hpp"
#include "../components/ArmorDefense.hpp"

#include "armor.hpp"

Entity * make_armor(int x, int y)
{

    // Create entity object
    Entity * armor = new Entity(
        x, y,
        SYMBOL_CHEST_ARMOR, TCODColor::green, "Kevlar jacket", ITEM,
        false, false);

    // Create item component
    Item * item_component = new Item(
        ItemType::ARMOR);

    // Attach item component to entity
    armor->item = item_component;
    item_component->owner = armor;

    // Create equippable component
    Equippable * equippable_component = new Equippable(
        EquipmentSlot::CHEST);

    // Specify armor defense
    equippable_component->armor_defense = new ArmorDefense(5);

    armor->equippable = equippable_component;
    equippable_component->owner = armor;

    return armor;
}
