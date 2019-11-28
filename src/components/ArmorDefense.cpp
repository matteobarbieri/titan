#include "ArmorDefense.hpp"

//////////////////////////////////////
/////////// WEAPON ATTACK ////////////
//////////////////////////////////////

ArmorDefense::ArmorDefense(int armor_value) : armor_value(armor_value)
{
}

json ArmorDefense::to_json()
{
    json j;

    j["armor_value"] = armor_value;

    return j;
}

ArmorDefense * ArmorDefense::from_json(json j)
{
    ArmorDefense * c = new ArmorDefense(j["armor_value"]);

    return c;
}
