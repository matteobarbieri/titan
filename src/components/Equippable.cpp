#include "Equippable.hpp"
#include "WeaponAttack.hpp"

//////////////////////////////////////
///// EQUIPMENT SLOT OPERATORS ///////
//////////////////////////////////////

Equippable::Equippable(EquipmentSlot valid_slots) :
    valid_slots(valid_slots)
{
    weapon_attack = nullptr;
}


json Equippable::to_json()
{
    json j;

    j["valid_slots"] = valid_slots;

    if (weapon_attack == nullptr)
    {
        j["weapon_attack"] = nullptr;
    }
    else
    {
        j["weapon_attack"] = weapon_attack->to_json();
    }

    return j;
}

Equippable * Equippable::from_json(json j)
{
    Equippable * c = new Equippable(j["valid_slots"]);

    if (j["weapon_attack"] != nullptr)
    {
        c->weapon_attack = WeaponAttack::from_json(j["weapon_attack"]);
    }

    return c;
}

/*
import random


class Equippable:
    """
    Class describing an item that can be equipped.
    """

    def __init__(self, valid_slots=[], stats_bonuses={}, skills_bonuses={},
                 damage_range=None, armor=None):
        """
        Parameters
        ----------

        valid_slots : list
            A list of all possible slots where the item can be equipped.

        stats_bonuses : dict
            A dictionary containing all bonuses to base stats that this piece
            of equipment grants its owner.

        skills_bonuses : dict
            A dictionary containing all bonuses to skills that this piece of
            equipment grants its owner.

        damage_range : tuple
            For weapons, a tuple [MIN, MAX] that specifies the range of damage
            that the weapon can deal

        armor : int
            The armor value that the piece of equipment provides.
        """

        # TODO on hit triggers?
        # TODO on being hit triggers?

        # Where can this item be equipped?
        self.valid_slots = valid_slots

        # What bonuses does it provide, once equipped?
        self.stats_bonuses = stats_bonuses
        self.skills_bonuses = skills_bonuses

        # Weapon related properties
        self.damage_range = damage_range

        # Armor related properties
        self.armor = armor

    def stat_bonus(self, stat_name):
        """
        The total bonus provided by an item to a specific stat (if equipped).
        """

        if stat_name in self.stats_bonuses.keys():
            return self.stat_bonuses[stat_name]
        else:
            return 0

    def roll_damage(self):

        # TODO improve this
        if self.damage_range is not None:
            return random.randint(*self.damage_range)
*/
