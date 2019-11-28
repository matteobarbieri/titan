#include "../../libtcod.hpp"

#include "../../Symbols.h"

#include "../../Entity.hpp"

#include "../../components/Item.hpp"
#include "../../components/WeaponAttack.hpp"
#include "../../components/Equippable.hpp"

Entity * make_dagger(int x, int y)
{

    // Create entity object
    Entity * dagger = new Entity(
        x, y,
        '-', TCODColor::sky, "Dagger", ITEM,
        false, false);

    // Create item component
    Item * item_component = new Item(
        ItemType::WEAPON, ItemSubtype::MELEE);

    // Attach item component to entity
    dagger->item = item_component;
    item_component->owner = dagger;

    // Create equippable component
    Equippable * equippable_component = new Equippable(
        EquipmentSlot::MAIN_HAND | EquipmentSlot::OFF_HAND);

    // Specify weapon attack
    equippable_component->weapon_attack = new WeaponAttack(4, 6);

    dagger->equippable = equippable_component;
    equippable_component->owner = dagger;

    return dagger;
}

Entity * make_dagger()
{
    return make_dagger(-1, -1);
}

Entity * make_baton(int x, int y)
{

    // Create entity object
    Entity * baton = new Entity(
        x, y,
        SYMBOL_MACE, TCODColor::lightAzure, "Stun baton", ITEM,
        false, false);

    // Create item component
    Item * item_component = new Item(
        ItemType::WEAPON, ItemSubtype::MELEE);

    // Attach item component to entity
    baton->item = item_component;
    item_component->owner = baton;

    // Create equippable component
    Equippable * equippable_component = new Equippable(
        EquipmentSlot::MAIN_HAND | EquipmentSlot::OFF_HAND);

    // Specify weapon attack
    equippable_component->weapon_attack = new WeaponAttack(4, 6);

    baton->equippable = equippable_component;
    equippable_component->owner = baton;

    return baton;
}

Entity * make_baton()
{
    return make_baton(-1, -1);
}
