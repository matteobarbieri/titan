#include "Reloadable.hpp"

Reloadable::Reloadable(int clip_size, int ammo, int ammo_per_shot, bool need_ammo) :
    clip_size(clip_size), ammo(ammo), ammo_per_shot(ammo_per_shot), need_ammo(need_ammo)
{
}

Reloadable::Reloadable(int clip_size) : Reloadable(clip_size, clip_size, 1, false)
{
}

bool Reloadable::consume_ammo()
{
    if (ammo >= ammo_per_shot)
    {
        ammo -= ammo_per_shot;
        return true;
    }
    else
    {
        return false;
    }
}

bool Reloadable::reload()
{

    // TODO Also check the case of weapons requiring special ammo
    if (ammo < clip_size)
    {
        ammo = clip_size;
        return true;
    }
    else
    {
        return false;
    }
}

json Reloadable::to_json()
{
    json j;

    j["clip_size"] = clip_size;
    j["ammo"] = ammo;
    j["ammo_per_shot"] = ammo_per_shot;
    j["need_ammo"] = need_ammo;

    return j;
}

Reloadable * Reloadable::from_json(json j)
{
    Reloadable * c = new Reloadable(
        j["clip_size"],
        j["ammo"],
        j["ammo_per_shot"],
        j["need_ammo"]);

    return c;
}
