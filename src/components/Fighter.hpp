#ifndef ROGUE_2077_FIGHTER
#define ROGUE_2077_FIGHTER

#include "../nlohmann/json.hpp"

using json = nlohmann::json;

// Forward declarations
class Entity;
class WeaponAttack;
class GameMap;

typedef enum 
{
    HALE = 0,
    SCRATCHED = 1,
    INJURED = 2,
    RAVAGED = 3,
    DEAD = 4,
} FighterStatus;

class Fighter
{

    public:

        int _max_hp;
        int _hp;

        ////////////////////////////
        ////////// STATS ///////////
        ////////////////////////////

        // The ability to hit an enemy with melee weapons (or unarmed)
        int _fighting;

        // The ability to hit an enemy with ranged weapons
        int _accuracy;

        // Reference to entity
        Entity * owner;

        Fighter(int max_hp);
        Fighter(int max_hp, int hp);
        Fighter(int max_hp, int hp, int _fighting, int _accuracy);

        int max_hp() const;
        int hp() const;

        /**
         * Returns an indication of how injured/damaged an entity is.
         */
        FighterStatus damage_track() const;

        /**
         * Function which determines if the attacker hits the target.
         *
         * The chance of hitting the target is given by the following formula:
         *
         * c = 100 / (1 + exp(-df/100))
         *
         * where df is the difference between the attacker's fighting stat and
         * the target's.
         *
         * TODO also having a shield equipped should help defending, somehow.
         */
        bool roll_to_hit_melee(Entity *, WeaponAttack *);
        bool roll_to_hit_ranged(Entity *, WeaponAttack *, GameMap *);

        /**
         * Attack a target with all equipped and primed weapons 
         */
        bool attack(Entity *, GameMap *);

        /**
         * Perform the attack with a specific weapon
         */
        bool attack_with_ranged_weapon(Entity * target, Entity * weapon, WeaponAttack *, GameMap *);

        /**
         * Perform the attack with a specific melee weapon
         */
        bool attack_with_melee_weapon(Entity * target, Entity * weapon, WeaponAttack *, GameMap *);

        /**
         * Applies damage reduction based on armor.
         */
        int apply_damage_reduction(int raw_damage);
        
        void take_damage(int amount);

        bool is_dead() const;

        // Used to update status of the entity after taking damage, etc.
        // Returns an int that could trigger other actions (such as changing the
        // entity in case a monster is dead etc.).
        int update_status();

        void die();

        /**
         * Creates a json representation of the component
         */
        json to_json();

        static Fighter * from_json(json j);

};

#endif /* ROGUE_2077_FIGHTER */
