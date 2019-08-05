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

    // Create equippable component

    return dagger;
}

/*

from components.item import Item, ItemType, ItemSubtype  # noqa
from components.equippable import Equippable

from equipment_slots import EquipmentSlots

from render_functions import RenderOrder

import tcod as libtcodpy


def make_dagger():


    # The equippable component of the entity
    equippable_component = Equippable(
        valid_slots=[EquipmentSlots.MAIN_HAND, EquipmentSlots.OFF_HAND],
        damage_range=[3, 4],
    )


    return dagger
*/
