#include "../../libtcod.hpp"

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
