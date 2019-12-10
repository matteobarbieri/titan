#include <string>
#include <sstream>

#include <stdlib.h>     /* srand, rand */

#include "../libtcod.hpp"

#include "../Entity.hpp"

#include "../utils.hpp"

#include "Fighter.hpp"
#include "Equipment.hpp"
#include "Equippable.hpp"
#include "Reloadable.hpp"
#include "Item.hpp"
#include "WeaponAttack.hpp"
#include "ArmorDefense.hpp"
#include "Ai.hpp"

#include "../EquipmentSlots.hpp"

#include "../GameMessages.hpp"

Fighter::Fighter(int max_hp, int hp, int _fighting, int _accuracy) :
    _max_hp(max_hp), _hp(hp), _fighting(_fighting), _accuracy(_accuracy)
{
}

Fighter::Fighter(int max_hp, int hp) : Fighter(max_hp, hp, 0, 0)
{
}

Fighter::Fighter(int max_hp) : Fighter(max_hp, max_hp)
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
    j["_fighting"] = _fighting;
    j["_accuracy"] = _accuracy;

    return j;
}

Fighter * Fighter::from_json(json j)
{
    Fighter * c = new Fighter(
        j["_max_hp"], j["_hp"],
        j["_fighting"], j["_accuracy"]);

    return c;
}

bool Fighter::roll_to_hit_ranged(Entity * target, WeaponAttack * weapon_attack)
{
    // TODO implement defense bonus from shield
    // TODO implement modifiers from weapon attack
    // TODO implement modifiers from [de]buffs
    
    // TODO should use actual values modified from equipment and such
    int df = _accuracy;

    // Determine chance to hit using a sigmoid function
    int chance_to_hit = floor(100/(1+exp(-df/10)));

    // Roll to hit
    int roll = rand() % 101;

    DEBUG("Chance to hit (RANGED) was " << chance_to_hit << "%, rolled a " << roll);

    return roll < chance_to_hit;
}

bool Fighter::roll_to_hit_melee(Entity * target, WeaponAttack * weapon_attack)
{
    // TODO implement defense bonus from shield
    // TODO implement modifiers from weapon attack
    // TODO implement modifiers from [de]buffs
    
    // TODO should use actual values modified from equipment and such
    int df = _fighting - target->fighter->_fighting;

    // Determine chance to hit using a sigmoid function
    int chance_to_hit = floor(100/(1+exp(-df/10)));

    // Roll to hit
    int roll = rand() % 101;

    DEBUG("Chance to hit (MELEE) was " << chance_to_hit << "%, rolled a " << roll);

    return roll < chance_to_hit;
}

bool Fighter::attack_with_melee_weapon(Entity * target, Entity * weapon, WeaponAttack * weapon_attack)
{

    // Build message
    std::ostringstream stringStream;

    // Consume ammo
    if (weapon->equippable->reloadable != nullptr)
    {
        if (!weapon->equippable->reloadable->consume_ammo())
        {

            stringStream << "No ammo in " << weapon->name << "!";
            // Add message to message log
            MessageLog::singleton().add_message(
                {stringStream.str(), TCODColor::yellow});

            return false;
        }
    }

    if (roll_to_hit_melee(target, weapon_attack))
    {
        // Determine amount
        int amount = (
            rand() % (weapon_attack->dmg_delta() + 1)) + weapon_attack->dmg_min;

        // TODO reduce amount based on target's armour
        amount = target->fighter->apply_damage_reduction(amount);

        target->fighter->take_damage(amount);

        // TODO
        // Change message based on who's attacking (Player or NPC)
        stringStream << "You punch the " << 
            target->name << " in the face for " << amount << " damage!";

        // Add message to message log
        MessageLog::singleton().add_message(
            {stringStream.str(), TCODColor::white});
    }
    else
    {
        stringStream << "You miss the " << target->name << "!";

        // Add message to message log
        MessageLog::singleton().add_message(
            {stringStream.str(), TCODColor::white});
    }

    return true;
}

bool Fighter::attack_with_ranged_weapon(Entity * target, Entity * weapon, WeaponAttack * weapon_attack)
{

    // TODO change, this is the same one from melee!!!
    // TODO complete with roll to hit
    
    // Build message
    std::ostringstream stringStream;

    // Check distance from target
    float target_distance = l2(owner->x, owner->y, target->x, target->y);

    if (target_distance > weapon_attack->range)
    {
        stringStream << "Target out of range";

        // Add message to message log
        MessageLog::singleton().add_message(
            {stringStream.str(), TCODColor::yellow});

        return false;
    }

    // Consume ammo
    if (weapon->equippable->reloadable != nullptr)
    {
        if (!weapon->equippable->reloadable->consume_ammo())
        {

            stringStream << "No ammo in " << weapon->name << "!";
            // Add message to message log
            MessageLog::singleton().add_message(
                {stringStream.str(), TCODColor::yellow});

            return false;
        }
    }
    

    // Must check cover, thus include map
    if (roll_to_hit_ranged(target, weapon_attack))
    {
        // Determine amount
        int amount = (
            rand() % (weapon_attack->dmg_delta() + 1)) + weapon_attack->dmg_min;

        // Reduce amount based on target's armour
        amount = target->fighter->apply_damage_reduction(amount);

        // Apply damage to target
        target->fighter->take_damage(amount);

        stringStream << target->name << " was hit for " << amount << " damage [" << weapon->name << "]";

        // Add message to message log
        MessageLog::singleton().add_message(
            {stringStream.str(), TCODColor::white});

        return true;
    }
    else
    {
        //stringStream << "You miss the " << target->name << "!";
        stringStream << target->name << " missed [" << weapon->name << "]";

        // Add message to message log
        MessageLog::singleton().add_message(
            {stringStream.str(), TCODColor::white});

        return true;
    }
}

/*
void Fighter::attack_ranged(Entity * target)
{

    // Variable used to determine if a melee weapon is equipped. If not, attack
    // unarmed.
    bool has_ranged_weapon_equipped = false;

    // Loop through equipment slots
    std::map<EquipmentSlot, Entity *>::iterator it;
    for (it=owner->equipment->slots.begin(); it!=owner->equipment->slots.end(); ++it)
    {

        if (
                it->second != nullptr && // Check that it has something equipped in that slot
                it->second->item->is_ranged() // check that it is a melee weapon
           )
        {
            // TODO
            // Check for ammo
            attack_ranged_with_weapon(target, it->second->equippable->weapon_attack);
            has_ranged_weapon_equipped = true;
        }

    }

    if (!has_ranged_weapon_equipped)
    {
        WeaponAttack unarmed_attack = WeaponAttack::unarmed_attack();
        attack_melee_with_weapon(target, &unarmed_attack);
    }

    // Check if target is dead 
    if (target->fighter->is_dead())
    {
        target->die();
    }
    
}
*/

bool Fighter::attack(Entity * target)
{

    bool attack_succeeded = false;

    // Loop through equipment slots
    std::map<EquipmentSlot, Entity *>::iterator it;
    for (it=owner->equipment->slots.begin(); it!=owner->equipment->slots.end(); ++it)
    {

        // Check that it has a weapon equipped in that slot
        if (it->second != nullptr && it->second->item->is_weapon())
        {
            if (it->second->item->is_melee()) // check that it is a melee weapon
            {
                attack_succeeded = attack_succeeded ||
                    attack_with_melee_weapon(target, it->second, it->second->equippable->weapon_attack);
            }

            if (it->second->item->is_ranged()) // check that it is a ranged weapon
            {
                attack_succeeded = attack_succeeded ||
                    attack_with_ranged_weapon(target, it->second, it->second->equippable->weapon_attack);
            }
        }

    }

    // Check if target is dead 
    if (target->fighter->is_dead())
    {
        target->die();
    }

    return attack_succeeded;
    
}

/*
void Fighter::attack_melee(Entity * target)
{

    // Variable used to determine if a melee weapon is equipped. If not, attack
    // unarmed.
    bool has_melee_weapon_equipped = false;

    // Loop through equipment slots
    std::map<EquipmentSlot, Entity *>::iterator it;
    for (it=owner->equipment->slots.begin(); it!=owner->equipment->slots.end(); ++it)
    {

        if (
                it->second != nullptr && // Check that it has something equipped in that slot
                it->second->item->is_melee() // check that it is a melee weapon
           )
        {
            attack_melee_with_weapon(target, it->second->equippable->weapon_attack);
            has_melee_weapon_equipped = true;
        }

    }

    if (!has_melee_weapon_equipped)
    {
        WeaponAttack unarmed_attack = WeaponAttack::unarmed_attack();
        attack_melee_with_weapon(target, &unarmed_attack);
    }

    // Check if target is dead 
    if (target->fighter->is_dead())
    {
        target->die();
    }
    
}
*/

int Fighter::apply_damage_reduction(int amount)
{
    int total_armor = 0;
    
    // Loop through equipment slots, check for armor
    std::map<EquipmentSlot, Entity *>::iterator it;
    for (it=owner->equipment->slots.begin(); it!=owner->equipment->slots.end(); ++it)
    {

        if (
                it->second != nullptr && // Check that it has something equipped in that slot
                it->second->item->is_armor() // check that it is a piece of armor
           )
        {
            // Update the total amount of armor
            total_armor += it->second->equippable->armor_defense->armor_value;
        }

    }

    // Compute the actual amount of damage
    // TODO replace with actual constants
    amount = ceil(amount * 10 / (10 + total_armor));

    return amount;

}

void Fighter::take_damage(int amount)
{
    _hp -= amount;
}

bool Fighter::is_dead() const
{
    return _hp <= 0;
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
*/
