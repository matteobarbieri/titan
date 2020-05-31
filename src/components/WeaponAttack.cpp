#include "WeaponAttack.hpp"
#include "../buffs/BuffStun.hpp"
#include "../Entity.hpp"

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
    
    // Save list of on hit effects
    json ohe;
    for (int i=0; i<(int)on_hit_effects.size(); i++)
    {
        ohe.push_back(on_hit_effects[i]->to_json());
    }
    j["on_hit_effects"] = ohe;

    // Save list of after attack effects
    json aae;
    for (int i=0; i<(int)after_attack_effects.size(); i++)
    {
        aae.push_back(after_attack_effects[i]->to_json());
    }
    j["after_attack_effects"] = aae;

    return j;
}

WeaponAttack * WeaponAttack::from_json(json j)
{
    WeaponAttack * c = new WeaponAttack(j["range"], j["dmg_min"], j["dmg_min"]);

    // On hit effects
    if (j["on_hit_effects"] != nullptr)
    {
        // Reapply on_hit_effects "manually"
        for (int i=0; i<(int)j["on_hit_effects"].size(); i++)
        {
            AttackEffect * ae = AttackEffect::from_json(j["on_hit_effects"][i]);
            c->on_hit_effects.push_back(ae);
        }
    }

    // After attack effects
    if (j["after_attack_effects"] != nullptr)
    {
        // Reapply after_attack_effects "manually"
        for (int i=0; i<(int)j["after_attack_effects"].size(); i++)
        {
            AttackEffect * ae = AttackEffect::from_json(j["after_attack_effects"][i]);
            c->after_attack_effects.push_back(ae);
        }
    }

    return c;
}

//////////////////////////////////////
/////////// ATTACK EFFECT ////////////
//////////////////////////////////////

void AttackEffect::apply(Entity * attacker, Entity * target, GameMap * level)
{

    // Call the actual implementation of a class
    _apply(attacker, target, level);
}

void AttackEffect::_apply(Entity * attacker, Entity * target, GameMap * level)
{
    // Do nothing by default
}

//json AttackEffect::to_json()
//{
    //json j;

    //return j;
//}

AttackEffect * AttackEffect::from_json(json j)
{
    if (j["subclass"] == "StunSelfAttackEffect")
    {
        return StunSelfAttackEffect::from_json(j);
    }


    return nullptr;
}

///////////////////////////////////////
/////// STUN SELF ATTACK EFFECT ///////
///////////////////////////////////////

StunSelfAttackEffect::StunSelfAttackEffect(int duration) : duration(duration)
{
}

void StunSelfAttackEffect::_apply(Entity * attacker, Entity * target, GameMap * level)
{

    BuffStun * bs = new BuffStun(duration, true);
    bs->apply(attacker);

    // TODO remove?
    DEBUG(attacker->name << " is tired!");
}

json StunSelfAttackEffect::to_json()
{
    json j;

    j["subclass"] = "StunSelfAttackEffect";
    j["duration"] = duration;

    return j;
}

StunSelfAttackEffect * StunSelfAttackEffect::from_json(json j)
{
    return new StunSelfAttackEffect(j["duration"]);
}

