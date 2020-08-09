#include "Buff.hpp"
#include "BuffStun.hpp"
#include "../Entity.hpp"

#include "../GameMessages.hpp"
#include "../SaveGame.hpp"
#include "../map/GameMap.hpp"

//Buff::Buff(Entity * target, int duration) : duration(duration), target(target)
//{
//}


Buff::Buff(int duration) : duration(duration), stays_on_death(false)
{
}

Buff::~Buff()
{
}

bool Buff::has_expired()
{
    return duration <= 0;
}

bool Buff::disables_entity()
{
    return false;
}

void Buff::apply(Entity * e)
{
    e->apply_buff(this);

    // Link entity to buff
    target = e;

    // Do buff-specific stuff
    _apply();
}

// By deefault, do nothing else on apply
void Buff::_apply()
{
}

void Buff::tick()
{
    // Decrease number of turns remaining
    duration--;
}

/**
 * Function which is called once when the buff expires.
 */
void Buff::expire(GameMap * game_map)
{
    // Does nothing by default
}

Buff * Buff::from_json(json j)
{
    // TODO to change with the right version
    
    if (j["subclass"] == "BuffStun")
    {
        return BuffStun::from_json(j);
    }

    if (j["subclass"] == "DelayedMessageBuff")
    {
        return DelayedMessageBuff::from_json(j);
    }

    if (j["subclass"] == "DelayedRemoveBuff")
    {
        return DelayedRemoveBuff::from_json(j);
    }

    if (j["subclass"] == "TempChangeEntityBuff")
    {
        return TempChangeEntityBuff::from_json(j);
    }

    return nullptr;
}

/*
 ____       _                      _ 
|  _ \  ___| | __ _ _   _  ___  __| |
| | | |/ _ \ |/ _` | | | |/ _ \/ _` |
| |_| |  __/ | (_| | |_| |  __/ (_| |
|____/ \___|_|\__,_|\__, |\___|\__,_|
                    |___/            
                                            _            __  __ 
 _ __ ___   ___  ___ ___  __ _  __ _  ___  | |__  _   _ / _|/ _|
| '_ ` _ \ / _ \/ __/ __|/ _` |/ _` |/ _ \ | '_ \| | | | |_| |_ 
| | | | | |  __/\__ \__ \ (_| | (_| |  __/ | |_) | |_| |  _|  _|
|_| |_| |_|\___||___/___/\__,_|\__, |\___| |_.__/ \__,_|_| |_|  
                               |___/                            
*/

DelayedMessageBuff::DelayedMessageBuff(
        int duration, std::string text, TCODColor color) :
        Buff(duration), text(text), color(color)
{
}

DelayedMessageBuff * DelayedMessageBuff::clone()
{
    return new DelayedMessageBuff(duration, text, color);
}

void DelayedMessageBuff::expire(GameMap * game_map)
{
    MessageLog::singleton().add_message({text, color});
}

json DelayedMessageBuff::to_json()
{
    json j;

    j["subclass"] = "DelayedMessageBuff";

    j["duration"] = duration;
    j["text"] = text;
    j["color"] = tcodcolor_to_json(color);
    j["stays_on_death"] = stays_on_death;

    return j;
}

DelayedMessageBuff * DelayedMessageBuff::from_json(json j)
{
    DelayedMessageBuff * dmb = new DelayedMessageBuff(
        j["duration"], j["text"], json_to_tcodcolor(j["color"]));

    dmb->stays_on_death = j["stays_on_death"];

    return dmb;
}

/*
 ____       _                      _                                     
|  _ \  ___| | __ _ _   _  ___  __| |  _ __ ___ _ __ ___   _____   _____ 
| | | |/ _ \ |/ _` | | | |/ _ \/ _` | | '__/ _ \ '_ ` _ \ / _ \ \ / / _ \
| |_| |  __/ | (_| | |_| |  __/ (_| | | | |  __/ | | | | | (_) \ V /  __/
|____/ \___|_|\__,_|\__, |\___|\__,_| |_|  \___|_| |_| |_|\___/ \_/ \___|
                              |___/                            
*/

DelayedRemoveBuff::DelayedRemoveBuff(
        int duration) :
        Buff(duration)
{
}

DelayedRemoveBuff * DelayedRemoveBuff::clone()
{
    return new DelayedRemoveBuff(duration);
}

void DelayedRemoveBuff::expire(GameMap * game_map)
{
    //DEBUG("DRM Expired ");
    game_map->remove_entity(target);

    //delete target;
    //MessageLog::singleton().add_message({text, color});
}

json DelayedRemoveBuff::to_json()
{
    json j;

    j["subclass"] = "DelayedRemoveBuff";

    j["duration"] = duration;
    j["stays_on_death"] = stays_on_death;

    return j;
}

DelayedRemoveBuff * DelayedRemoveBuff::from_json(json j)
{
    DelayedRemoveBuff * drm = new DelayedRemoveBuff(
        j["duration"]);

    drm->stays_on_death = j["stays_on_death"];
    return drm;
}

/*
 _____                       ____ _                            
|_   _|__ _ __ ___  _ __    / ___| |__   __ _ _ __   __ _  ___ 
  | |/ _ \ '_ ` _ \| '_ \  | |   | '_ \ / _` | '_ \ / _` |/ _ \
  | |  __/ | | | | | |_) | | |___| | | | (_| | | | | (_| |  __/
  |_|\___|_| |_| |_| .__/   \____|_| |_|\__,_|_| |_|\__, |\___|
                   |_|                              |___/      
 _____       _   _ _         
| ____|_ __ | |_(_) |_ _   _ 
|  _| | '_ \| __| | __| | | |
| |___| | | | |_| | |_| |_| |
|_____|_| |_|\__|_|\__|\__, |
*/

TempChangeEntityBuff::TempChangeEntityBuff(int duration, std::string new_name, TCODColor new_color):
    Buff(duration), new_name(new_name), new_color(new_color)
{
}

TempChangeEntityBuff::TempChangeEntityBuff(
        int duration,
        std::string new_name, TCODColor new_color,
        std::string old_name, TCODColor old_color):
    Buff(duration), new_name(new_name), new_color(new_color),
    old_name(old_name), old_color(old_color)
{
}

TempChangeEntityBuff * TempChangeEntityBuff::clone()
{
    return new TempChangeEntityBuff(duration, new_name, new_color, old_name, old_color);
}

void TempChangeEntityBuff::_apply()
{
    DEBUG("Applied to " << target->name);
    // Save old values of the attributes, so that they can be restored
    old_color = target->color();
    old_name = target->name;

    // Change target entity's attributes
    target->color(new_color);
    target->name = new_name;
}

void TempChangeEntityBuff::expire(GameMap * game_map)
{
    // Restore previous attributes
    target->color(old_color);
    target->name = old_name;
}

json TempChangeEntityBuff::to_json()
{
    json j;

    j["subclass"] = "TempChangeEntityBuff";

    j["duration"] = duration;
    j["stays_on_death"] = stays_on_death;

    j["new_color"] = tcodcolor_to_json(new_color);
    j["new_name"] = new_name;

    j["old_color"] = tcodcolor_to_json(old_color);
    j["old_name"] = old_name;

    return j;
}

TempChangeEntityBuff * TempChangeEntityBuff::from_json(json j)
{
    TempChangeEntityBuff * tce = new TempChangeEntityBuff(
        j["duration"],
        j["new_name"], json_to_tcodcolor(j["new_color"]),
        j["old_name"], json_to_tcodcolor(j["old_color"]));

    tce->stays_on_death = j["stays_on_death"];
    return tce;
}
