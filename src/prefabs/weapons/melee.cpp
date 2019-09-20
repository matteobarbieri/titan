#include "../../libtcod.hpp"

#include "../../Entity.hpp"

#include "../../components/Item.hpp"
#include "../../components/Equippable.hpp"

Entity * make_dagger()
{

    // Create entity object
    Entity * dagger = new Entity(
        -1, -1,
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

    // TODO Add damage function

    dagger->equippable = equippable_component;
    equippable_component->owner = dagger;

    return dagger;
}
