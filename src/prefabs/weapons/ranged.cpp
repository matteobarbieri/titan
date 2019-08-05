/*
from entity import Entity

from components.item import Item, ItemType, ItemSubtype  # noqa
from components.equippable import Equippable

from equipment_slots import EquipmentSlots

from render_functions import RenderOrder

import tcod as libtcodpy


def make_pistol():

    # Create the base Item instance
    item_component = Item(
        item_types=[ItemType.WEAPON],
        item_subtypes=[ItemSubtype.RANGED]
    )

    # The equippable component of the entity
    equippable_component = Equippable(
        valid_slots=[EquipmentSlots.MAIN_HAND, EquipmentSlots.OFF_HAND],
        damage_range=[2, 5],
    )

    pistol = Entity(
        None, None,
        ')', libtcodpy.green, "Pistol", blocks=False,
        block_sight=False, render_order=RenderOrder.ITEM,
        components={
            'equippable': equippable_component,
            'item': item_component,
        })

    return pistol
*/
