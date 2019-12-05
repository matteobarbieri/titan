#include "../../libtcod.hpp"

#include "../../Symbols.h"

#include "../../Entity.hpp"

#include "../../components/Item.hpp"
#include "../../components/WeaponAttack.hpp"
#include "../../components/Reloadable.hpp"
#include "../../components/Equippable.hpp"

Entity * make_pistol(int x, int y)
{

    // Create entity object
    Entity * e = new Entity(
        x, y,
        SYMBOL_PISTOL, TCODColor::sky, "Pistol", ITEM,
        false, false);

    // Create item component
    Item * item_component = new Item(
        ItemType::WEAPON, ItemSubtype::RANGED);

    // Attach item component to entity
    e->item = item_component;
    item_component->owner = e;

    // Create equippable component
    Equippable * equippable_component = new Equippable(
        EquipmentSlot::MAIN_HAND | EquipmentSlot::OFF_HAND);

    // Specify weapon attack
    equippable_component->weapon_attack = new WeaponAttack(3, 3);
    
    // Specify reloadable subcomponent
    equippable_component->reloadable = new Reloadable(6);

    e->equippable = equippable_component;
    equippable_component->owner = e;

    return e;
}

Entity * make_pistol()
{
    return make_pistol(-1, -1);
}
