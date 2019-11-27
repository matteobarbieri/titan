#ifndef ROGUE_20177_WEAPON_ATTACK
#define ROGUE_20177_WEAPON_ATTACK

#include <vector>

#include "../nlohmann/json.hpp"

// Forward declaration
class Entity;

using json = nlohmann::json;

// Some kind of effect caused by the attack
class AttackEffect
{
};

// Represents an attack from a weapon
class WeaponAttack
{

    public:

        int dmg_min;
        int dmg_max;

        // The effects applied by the attack
        std::vector<AttackEffect *> effects;

        // Simple constructor which sets damage range.
        WeaponAttack(int, int);

        int dmg_delta() const;

        // Generate a simple attack simulating unarmed combat
        static WeaponAttack unarmed_attack(int=1, int=2);

        /**
         * Creates a json representation of the component
         */
        json to_json();

        static WeaponAttack * from_json(json j);
};


#endif /* ifndef ROGUE_20177_WEAPON_ATTACK */
