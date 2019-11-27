#include <string>
#include <sstream>

#include <stdlib.h>     /* srand, rand */

#include "../libtcod.hpp"

#include "../Entity.hpp"

#include "Fighter.hpp"
#include "Equipment.hpp"
#include "WeaponAttack.hpp"
#include "Ai.hpp"

#include "../EquipmentSlots.hpp"

#include "../GameMessages.hpp"


Fighter::Fighter(int max_hp, int hp) : _max_hp(max_hp), _hp(hp)
{
}

Fighter::Fighter(int max_hp) : _max_hp(max_hp), _hp(max_hp)
{
}

int Fighter::hp() const
{
    // TODO possibly change this
    return _hp;
}

int Fighter::max_hp() const
{
    // TODO possibly change this
    return _max_hp;
}

json Fighter::to_json()
{
    json j;

    j["_hp"] = _hp;
    j["_max_hp"] = _max_hp;

    return j;
}

Fighter * Fighter::from_json(json j)
{
    Fighter * c = new Fighter(j["_max_hp"], j["_hp"]);

    return c;
}

void Fighter::attack_melee_with_weapon(Entity * target, WeaponAttack * weapon_attack)
{

    //int dmg_min = 1, dmg_max = 2;
    //int dmg_delta = dmg_max - dmg_min;

    //int amount = 5;
    //int amount = (rand() % (dmg_delta + 1)) + dmg_min;
    int amount = (rand() % (weapon_attack->dmg_delta() + 1)) + weapon_attack->dmg_min;

    target->fighter->take_damage(amount);

    // Build message
    std::ostringstream stringStream;

    stringStream << "You punch the " << 
        target->name << " in the face for " << amount << " damage!";

    // Add message to message log
    MessageLog::singleton().add_message(
        {stringStream.str(), TCODColor::white});

}

void Fighter::attack_melee(Entity * target)
{

    /*
     *for weapon in self.owner.equipment.melee_weapons:
     *    self.attack_melee_with_weapon(target, weapon);
     */

    // TODO stub
    
    bool has_melee_weapon_equipped = false;

    // Loop through equipment slots
    std::map<EquipmentSlot, Entity *>::iterator it;
    for (it=owner->equipment->slots.begin(); it!=owner->equipment->slots.end(); ++it)
    {

        // Store value
        // TODO should also check if the item equipped is actually a weapon
        if (it->second != nullptr)
        {
            has_melee_weapon_equipped = true;
            // TODO complete
        }

    }

    if (!has_melee_weapon_equipped)
    {
        WeaponAttack unarmed_attack = WeaponAttack::unarmed_attack();
        attack_melee_with_weapon(target, &unarmed_attack);
    }

    
}

void Fighter::take_damage(int amount)
{
    _hp -= amount;
}

bool Fighter::is_dead() const
{
    return _hp <= 0;
}

void Fighter::shoot(Entity * target)
{
}

/*
from render_functions import RenderOrder

from .item import ItemType, ItemSubtype

class NoMeleeWeaponsEquippedException(Exception):
    pass


class NoRangedWeaponsEquippedException(Exception):
    pass


class Fighter:

    def get_adjusted_stat(self, stat_name):
        base_stat = self.stats['base_{}'.format(stat_name)]

        if self.owner and self.owner.equipment:
            equipment_bonus = self.owner.equipment.stat_bonus(stat_name)
        else:
            equipment_bonus = 0

        return base_stat + equipment_bonus

    @property
    def INT(self):
        return self.get_adjusted_stat('int')

    @property
    def max_hp(self):
        if self.owner and self.owner.equipment:
            bonus = self.owner.equipment.max_hp_bonus
        else:
            bonus = 0

        return self.base_max_hp + bonus

    @property
    def power(self):
        if self.owner and self.owner.equipment:
            bonus = self.owner.equipment.power_bonus
        else:
            bonus = 0

        return self.base_power + bonus

    @property
    def defense(self):
        if self.owner and self.owner.equipment:
            bonus = self.owner.equipment.defense_bonus
        else:
            bonus = 0

        return self.base_defense + bonus

    def heal(self, amount):
        self.hp += amount

        if self.hp > self.max_hp:
            self.hp = self.max_hp

    def shoot(self, target):
        """
        other : Entity
        """

        messages = list()

        equipped_ranged_weapons = list()

        # First check that whoever is shooting actually has a ranged weapon
        # equipped

        for _, w in self.owner.equipment.slots.items():
            if (w and ItemType.WEAPON in w.item.item_types and
                ItemSubtype.RANGED in w.item.item_subtypes):

                # It is an equipped ranged weapon
                equipped_ranged_weapons.append(w)

        # If no ranged weapons are equipped, raise an exception
        if len(equipped_ranged_weapons) == 0:
            raise NoRangedWeaponsEquippedException()

        # Check if weapon has enough ammo to shoot
        # TODO implement

        # Check if enemy is in weapon range
        # TODO implement
        weapons_in_range = equipped_ranged_weapons

        # Shoot with remaining viable weapons
        # TODO implement

        for w in weapons_in_range:
            if not self.roll_to_hit_ranged(target, w):
                # TODO better implement this
                messages.append(Message("Missed!", libtcod.yellow))

            damage = self.calculate_damage(target, w)

            if damage > 0:

                messages.append(
                    Message('{0} shoots {1} for {2} hit points.'.format(
                        self.owner.name.capitalize(), target.name,
                        str(damage)), libtcod.white))

                # In case there are other effects, add extra messages
                messages.extend(target.fighter.take_damage(damage))
            else:

                # Append the no damage dealt messages
                messages.append(
                    Message('{0} attacks {1} but does no damage.'.format(
                        self.owner.name.capitalize(), target.name), libtcod.white))

        return messages

    def roll_to_hit_ranged(self, target, weapon):
        # TODO placeholder, must be implemented. For the time being, always
        # hit!
        return True

    def roll_to_hit_melee(self, target, weapon):
        # TODO placeholder, must be implemented. For the time being, always
        # hit!
        return True

    def calculate_damage(self, target, weapon):

        return weapon.equippable.roll_damage()

        # damage = self.power - target.fighter.defense
        # return 3


    def attack_melee_with_weapon(self, target, weapon):
        """
        Perform a melee attack with a specific weapon
        """

        # TODO DEBUG remove
        # print("Attacking with melee weapon")
        messages = list()

        if not self.roll_to_hit_melee(target, weapon):
            # TODO better implement this
            messages.append(Message("Missed!", libtcod.yellow))

        damage = self.calculate_damage(target, weapon)

        if damage > 0:

            messages.append(
                Message('{0} attacks {1} for {2} hit points.'.format(
                    self.owner.name.capitalize(), target.name,
                    str(damage)), libtcod.white))

            # In case there are other effects, add extra messages
            messages.extend(target.fighter.take_damage(damage))
        else:

            # Append the no damage dealt messages
            messages.append(
                Message('{0} attacks {1} but does no damage.'.format(
                    self.owner.name.capitalize(), target.name), libtcod.white))

        return messages


    def attack_melee(self, target):
        """
        Perform an attack with all equipped melee weapons
        """

*/
