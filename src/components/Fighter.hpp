#ifndef ROGUE_2077_FIGHTER
#define ROGUE_2077_FIGHTER

#include "../nlohmann/json.hpp"

using json = nlohmann::json;

// Forward declarations
class Entity;

class Fighter
{

    private:

        int _max_hp;
        int _hp;
        

    public:

        // Reference to entity
        Entity * owner;

        Fighter(int max_hp);
        Fighter(int max_hp, int hp);

        ~Fighter();

        int max_hp() const;
        int hp() const;

        /**
         * Creates a json representation of the component
         */
        json to_json();

        void attack_melee(Entity * other);

        void shoot(Entity * other);

        void take_damage(int amount);

        bool is_dead() const;

        // Used to update status of the entity after taking damage, etc.
        void update_status();

        void die();

        static Fighter * from_json(json j);

};

#endif /* ROGUE_2077_FIGHTER */

/*
import libtcodpy as libtcod

from game_messages import Message

from render_functions import RenderOrder

from .item import ItemType, ItemSubtype


class NoMeleeWeaponsEquippedException(Exception):
    pass


class NoRangedWeaponsEquippedException(Exception):
    pass


class Fighter:
    def __init__(self, hp, defense, power, xp=0,
                 STR=0, DEX=0, INT=0):

        # TODO Old stuff, to change/update/remove
        self.base_max_hp = hp
        self.hp = hp
        self.base_defense = defense
        self.base_power = power
        self.xp = xp

        ###################################
        ############ STATS ################
        ###################################

        self.stats = {
            'base_str': STR,
            'base_dex': DEX,
            'base_int': INT,
        }

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

*/
