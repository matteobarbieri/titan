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

#include "../map/GameMap.hpp"
#include "../map/Tile.hpp"

#include "../EquipmentSlots.hpp"

#include "../GameMessages.hpp"

// Prototypes for auxiliary functions
bool weapon_in_range(Entity * attacker, Entity * target, Entity * weapon, WeaponAttack * weapon_attack);
bool ammo_check(Entity * weapon);

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

bool Fighter::roll_to_hit_ranged(Entity * target, WeaponAttack * weapon_attack, GameMap * level)
{
    // TODO implement defense bonus from shield
    // TODO implement modifiers from weapon attack
    // TODO implement modifiers from [de]buffs
    
    // TODO should use actual values modified from equipment and such
    // Base accuracy
    int acc = _accuracy;

    // Determine from where the shot is coming from
    int dx = owner->x - target->x;
    int dy = owner->y - target->y;

    // Compute l2 distance from target
    float target_distance = sqrt(dx*dx + dy*dy);

    // Compute the coordinates of the tiles to take into consideration for cover
    
    // The tile to the left/right of the target
    float modifier_tile_h = 0;
    if (abs(dx) > 1)
    {
        int tileh_x = target->x + dx/(abs(dx));
        int tileh_y = target->y;

        // TODO
        // Modify using trigonometric function
        modifier_tile_h = level->get_tile_at(tileh_x, tileh_y)->cover_level;
    }

    // The tile just above/below the target
    float modifier_tile_v = 0;
    if (abs(dy) > 1)
    {
        int tilev_x = target->x;
        int tilev_y = target->y + dy/(abs(dy));

        // TODO
        // Modify using trigonometric function
        modifier_tile_v = level->get_tile_at(tilev_x, tilev_y)->cover_level;
    }

    //acc -= modifier_tile_h; 
    //acc -= modifier_tile_v; 

    DEBUG("Final accuracy: " << acc);
    DEBUG("Modifer from horizontal tile: " << modifier_tile_h);
    DEBUG("Modifer from vertical tile: " << modifier_tile_v);

    float cover_multiplier = 1.0 -
        (abs(dx)/target_distance) * (modifier_tile_h/100.0) -
        (abs(dy)/target_distance) * (modifier_tile_v/100.0);

    DEBUG("Cover multiplier: " << cover_multiplier);

    // Determine chance to hit using a sigmoid function
    int chance_to_hit = floor(100/(1+exp(-acc/10)));

    DEBUG("Unmodified chance to hit: " << chance_to_hit);

    // Apply cover multiplier to chance to hit
    chance_to_hit *= cover_multiplier;

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


bool weapon_in_range(Entity * attacker, Entity * target, Entity * weapon, WeaponAttack * weapon_attack)
{

    // Build message
    std::ostringstream stringStream;

    // Check distance from target
    float target_distance = l2(attacker->x, attacker->y, target->x, target->y);

    if (target_distance > weapon_attack->range)
    {
        stringStream << "Target out of range [" << weapon->name << "]";

        // Add message to message log
        MessageLog::singleton().add_message(
            {stringStream.str(), TCODColor::yellow});

        return false;
    }
    else
    {
        return true;
    }
}


bool ammo_check(Entity * weapon)
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
    return true;
}


bool Fighter::attack_with_melee_weapon(Entity * target, Entity * weapon, WeaponAttack * weapon_attack)
{

    // Use stram to build messages
    std::ostringstream stringStream;

    // Check if weapon in range
    if (!weapon_in_range(owner, target, weapon, weapon_attack))
    {
        return false;
    }

    // Check for ammo (if the weapon uses them)
    if (!ammo_check(weapon))
    {
        return false;
    }

    if (roll_to_hit_melee(target, weapon_attack))
    {
        // Determine amount
        int amount = (
            rand() % (weapon_attack->dmg_delta() + 1)) + weapon_attack->dmg_min;

        amount = target->fighter->apply_damage_reduction(amount);

        target->fighter->take_damage(amount);

        // Add message to message log
        stringStream << target->name << " was hit for " << amount << " damage [" << weapon->name << "]";
        
        MessageLog::singleton().add_message(
            {stringStream.str(), TCODColor::white});
    }
    else
    {
        stringStream << target->name << " missed [" << weapon->name << "]";

        // Add message to message log
        MessageLog::singleton().add_message(
            {stringStream.str(), TCODColor::white});
    }

    return true;
}

bool Fighter::attack_with_ranged_weapon(Entity * target, Entity * weapon, WeaponAttack * weapon_attack, GameMap * level)
{

    // Build message
    std::ostringstream stringStream;

    // Check if weapon in range
    if (!weapon_in_range(owner, target, weapon, weapon_attack))
    {
        return false;
    }

    // Check for ammo (if the weapon uses them)
    if (!ammo_check(weapon))
    {
        return false;
    }

    // TODO Must check cover, thus include map
    if (roll_to_hit_ranged(target, weapon_attack, level))
    {
        // Determine amount
        int amount = (
            rand() % (weapon_attack->dmg_delta() + 1)) + weapon_attack->dmg_min;

        // Reduce amount based on target's armour
        amount = target->fighter->apply_damage_reduction(amount);

        // Apply damage to target
        target->fighter->take_damage(amount);

        // Add message to message log
        stringStream << target->name << " was hit for " << amount << " damage [" << weapon->name << "]";

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

bool Fighter::attack(Entity * target, GameMap * level)
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
                    attack_with_ranged_weapon(
                        target, it->second, it->second->equippable->weapon_attack,
                        level);
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
