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

    // Do buff-specific stuff
    _apply(e);

    // Link entity to buff
    target = e;
}

// By deefault, do nothing else on apply
void Buff::_apply(Entity * e)
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
