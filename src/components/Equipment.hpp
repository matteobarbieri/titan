#ifndef ROGUE_20177_EQUIPMENT
#define ROGUE_20177_EQUIPMENT 

#include "../nlohmann/json.hpp"

using json = nlohmann::json;

// Forward declarations
class Entity;
enum class EquipmentSlot;

/*
 * Exception raised whenever it is impossible to equip a piece of equipment
 * for whatever reason.
 */
class UnableToEquipException : public std::exception
{
};


class Equipment
{

    private:


    public:
        Equipment();

        // The equipment slots
        std::map<EquipmentSlot, Entity *> slots;

        /**
         * Creates a json representation of the component
         */
        json to_json();

        static Equipment * from_json(json j);

        EquipmentSlot equip(Entity * item);
        void unequip(Entity * item);
        //void unequip(Entity * item);

};


#endif /* ifndef ROGUE_20177_EQUIPMENT */

/*
from game_messages import Message

import tcod as libtcod

from equipment_slots import SLOT_NAMES


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
