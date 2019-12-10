#include "WeaponAttack.hpp"

//////////////////////////////////////
/////////// WEAPON ATTACK ////////////
//////////////////////////////////////

WeaponAttack::WeaponAttack(float range, int dmg_min, int dmg_max) : range(range), dmg_min(dmg_min), dmg_max(dmg_max)
{
}

int WeaponAttack::dmg_delta() const
{
    return dmg_max - dmg_min;
}

//WeaponAttack WeaponAttack::unarmed_attack(int dmg_min, int dmg_max)
//{
    //return WeaponAttack(dmg_min, dmg_max);
//}

json WeaponAttack::to_json()
{
    json j;

    j["range"] = range;
    j["dmg_min"] = dmg_min;
    j["dmg_max"] = dmg_max;

    return j;
}

WeaponAttack * WeaponAttack::from_json(json j)
{
    WeaponAttack * c = new WeaponAttack(j["range"], j["dmg_min"], j["dmg_min"]);

    return c;
}
