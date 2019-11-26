#include "Equipment.hpp"
#include "Equippable.hpp"
#include "Item.hpp"
#include "Inventory.hpp"

#include "../EquipmentSlots.hpp"
#include "../Entity.hpp"

Equipment::Equipment()
{
    slots[EquipmentSlot::MAIN_HAND] = nullptr;
    slots[EquipmentSlot::OFF_HAND] = nullptr;
}


json Equipment::to_json()
{
    json j;

    // TODO just so that the json is not empty
    j["null"] = false;

    return j;
}

Equipment * Equipment::from_json(json j)
{
    Equipment * c = new Equipment();

    return c;
}


void Equipment::unequip(Entity * item)
{

    EquipmentSlot slot = EquipmentSlot::NONE;

    // Loop through equipment slots
    std::map<EquipmentSlot, Entity *>::iterator it;
    for (it=slots.begin(); it!=slots.end(); ++it)
    {
        if (it->second == item)
        {
            slot = it->first;
            break;
        }
    }

    if (slot != EquipmentSlot::NONE)
    {
        slots[slot] = nullptr;
        item->item->equipped = false;
        
        // Add it back to the inventory items.
        owner->inventory->items.push_back(item);
    }
    else
    {
    }

}

EquipmentSlot Equipment::equip(Entity * item)
{

    EquipmentSlot free_slot = EquipmentSlot::NONE;

    // Loop through equipment slots
    std::map<EquipmentSlot, Entity *>::iterator it;
    for (it=slots.begin(); it!=slots.end(); ++it)
    {
        if ((it->first & item->equippable->valid_slots) != EquipmentSlot::NONE && // right slot
            it->second == nullptr) // slot empty
        {
            free_slot = it->first;
            break;
        }
    }

    if (free_slot != EquipmentSlot::NONE)
    {
        slots[free_slot] = item;
        item->item->equipped = true;

        // Also remove from list of items in the inventory (required to simplify
        // serialization).
        owner->inventory->remove_item(item);
    }
    else
    {
    }

    return free_slot;
}

/*
from game_messages import Message

import tcod as libtcod

from equipment_slots import SLOT_NAMES

class UnableToEquipException(Exception):
    """
    Exception raised whenever it is impossible to equip a piece of equipment
    for whatever reason.
    """
    pass


class Equipment:
    """
    This class describes the equipment currently 'active' (i.e., worn/wielded,
    whatever) on the player.
    """

    def __init__(self, available_slots=[]):

        # Initialize the list of slots
        self.available_slots = available_slots

        # Initialize the `slots` property, which contains the list of all
        # equipped items
        self.slots = dict()

    def stat_bonus(self, stat_name):
        """
        Returns the total equipment stat bonus for a specific stat
        """

        # TODO can be optimized (cached) in order to not recompute each bonus
        # every time a stat is checked

        total_bonus = 0

        # Search for all possible equipment slots
        for _, item in self.slots.items():

            if item:
                total_bonus += item.equippable.stat_bonus(stat_name)

        return total_bonus

    @property
    def max_hp_bonus(self):
        # TODO to rework
        return 0

        # bonus = 0

        # if self.main_hand and self.main_hand.c['equippable']:
            # bonus += self.main_hand.c['equippable'].max_hp_bonus

        # if self.off_hand and self.off_hand.c['equippable']:
            # bonus += self.off_hand.c['equippable'].max_hp_bonus

        # return bonus

    @property
    def power_bonus(self):
        # TODO to rework
        return 0

        # bonus = 0

        # if self.main_hand and self.main_hand.c['equippable']:
            # bonus += self.main_hand.c['equippable'].power_bonus

        # if self.off_hand and self.off_hand.c['equippable']:
            # bonus += self.off_hand.c['equippable'].power_bonus

        # return bonus

    @property
    def defense_bonus(self):
        # TODO to rework
        return 0

        # bonus = 0

        # if self.main_hand and self.main_hand.c['equippable']:
            # bonus += self.main_hand.c['equippable'].defense_bonus

        # if self.off_hand and self.off_hand.c['equippable']:
            # bonus += self.off_hand.c['equippable'].defense_bonus

        # return bonus

    def unequip(self, equippable_entity):
        """
        Unequip a currently equipped item.
        """

        messages = list()

        # Search for the item among equipped items
        for slot, item in self.slots.items():
            if item == equippable_entity:
                item.item.equipped = False
                # self.slots[slot] = None
                del self.slots[slot]

                messages.append(Message(
                    "{} unequipped (was in {})".format(
                        equippable_entity, SLOT_NAMES[slot]),
                    libtcod.white))
                return messages
            pass
        else:
            # TODO should not really happen actually...
            pass


    @property
    def melee_weapons(self):
        """
        Return the list of melee weapons currently equipped
        """
        weapons = list()

        for _, equipped_entity in self.slots.items():
            if equipped_entity and equipped_entity.item.is_melee():
                weapons.append(equipped_entity)

        # TODO DEBUG remove
        # print("Melee weapons:")
        # print(weapons)

        return weapons
*/
