#ifndef ROGUE_20177_WEAPON_ATTACK
#define ROGUE_20177_WEAPON_ATTACK

#include <vector>

#include "../nlohmann/json.hpp"

// Forward declaration
class Entity;
class GameMap;

using json = nlohmann::json;

// Some kind of effect caused by the attack
class AttackEffect
{

    public:

        void apply(Entity * attacker, Entity * target, GameMap * level);

        /**
         * Creates a json representation of the weapon attack
         */
        virtual json to_json() = 0;

        static AttackEffect * from_json(json j);

    protected:

        virtual void _apply(Entity * attacker, Entity * target, GameMap * level);

};

// Represents an attack from a weapon
class WeaponAttack
{

    public:

        float range;
        int dmg_min;
        int dmg_max;

        // The effects applied by the attack if it hits
        std::vector<AttackEffect *> on_hit_effects;

        // The effects applied by the attack after this has been performed,
        // whether the attack has hit or not
        std::vector<AttackEffect *> after_attack_effects;

        // Simple constructor which sets damage range.
        WeaponAttack(float, int, int);

        int dmg_delta() const;

        // Generate a simple attack simulating unarmed combat
        //static WeaponAttack unarmed_attack(int=1, int=2);

        /**
         * Creates a json representation of the weapon attack
         */
        json to_json();

        static WeaponAttack * from_json(json j);
};

/**
 * An attack effect which stuns the attacker for a number of turns when
 * performed.
 */
class StunSelfAttackEffect : public AttackEffect
{
    public:

        int duration;

        StunSelfAttackEffect(int duration);

        void _apply(Entity * attacker, Entity * target, GameMap * level);

        json to_json();

        static StunSelfAttackEffect * from_json(json j);
};

#endif /* ifndef ROGUE_20177_WEAPON_ATTACK */
