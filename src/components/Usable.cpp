#include "Usable.hpp"

#include "../utils.hpp"

#include "../prefabs/funcs/misc.hpp"

#include "../GameState.hpp"

#include "../actions/Outcome.hpp"

//#include <string>
#include <sstream>
/**
 _   _           _     _      
| | | |___  __ _| |__ | | ___ 
| | | / __|/ _` | '_ \| |/ _ \
| |_| \__ \ (_| | |_) | |  __/
 \___/|___/\__,_|_.__/|_|\___|
*/
Usable::Usable()
{
}

Usable::~Usable()
{
}

Usable * Usable::from_json(json j)
{
    // TODO There is nothing in this component so far
    //Usable * c = new Usable();

    if (j["subclass"] == "AOEUsable")
    {
        return AOEUsable::from_json(j);
    }

    return nullptr;
}

Outcome * Usable::use()
{
    return _use();
}

/**
    _    ___  _____ _   _           _     _
   / \  / _ \| ____| | | |___  __ _| |__ | | ___
  / _ \| | | |  _| | | | / __|/ _` | '_ \| |/ _ \
 / ___ \ |_| | |___| |_| \__ \ (_| | |_) | |  __/
/_/   \_\___/|_____|\___/|___/\__,_|_.__/|_|\___|
*/

AOEUsable::AOEUsable(bool is_consumable, int radius, int range) : Targetable(radius, range)
{
    this->is_consumable = is_consumable;
}

Outcome * AOEUsable::_use()
{
    GamePhase next_phase;

    // Build message
    std::ostringstream stringStream;

    next_phase = TARGETING_ITEM;

    DEBUG("Radius: " << radius << ", range: " << range);
    stringStream << "Select target ";

    // Add message to message log
    MessageLog::singleton().add_message(
        {stringStream.str(), TCODColor::yellow});

    // Return outcome
    Outcome * outcome = new Outcome(
        next_phase,
        false,
        false);

    return outcome;

}

json AOEUsable::to_json()
{
    json j;
    j["subclass"] = "AOEUsable";

    j["is_consumable"] = is_consumable;

    json j_effects;

    // Buffs currently applied to entity
    for (int i=0; i<(int)effects.size(); i++)
    {
        j_effects.push_back(effects[i]->to_json());
    }

    j["effects"] = j_effects;

    // Targetable subclass
    j["radius"] = radius;
    j["range"] = range;

    return j;
}

AOEUsable * AOEUsable::from_json(json j)
{
    
    AOEUsable * aoeu = new AOEUsable(j["is_consumable"], j["radius"], j["range"]);

    // Restore effects
    // TODO probably could move in parent class
    if (j["effects"] != nullptr)
    {
        for (int i=0; i<(int)j["effects"].size(); i++)
        {
            Effect * ef = Effect::from_json(j["effects"][i]);
            aoeu->effects.push_back(ef);
        }
    }

    return aoeu;
}

/*
 _____                    _        _     _      
|_   _|_ _ _ __ __ _  ___| |_ __ _| |__ | | ___ 
  | |/ _` | '__/ _` |/ _ \ __/ _` | '_ \| |/ _ \
  | | (_| | | | (_| |  __/ || (_| | |_) | |  __/
  |_|\__,_|_|  \__, |\___|\__\__,_|_.__/|_|\___|
               |___/
*/

Targetable::Targetable(int radius, int range) : radius(radius), range(range)
{
}

bool Targetable::is_in_range(int src_x, int src_y, int trg_x, int trg_y)
{

    // TODO must change

    if (l2(src_x, src_y, trg_x, trg_y ) <= range)
    {
        return true;
    }

    return false;

}

bool Targetable::is_in_radius(int src_x, int src_y, int trg_x, int trg_y)
{

    // TODO must change

    if (l2(src_x, src_y, trg_x, trg_y ) <= radius)
    {
        return true;
    }

    return false;

}
