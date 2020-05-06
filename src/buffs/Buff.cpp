#include "Buff.hpp"
#include "BuffStun.hpp"

#include "../GameMessages.hpp"
#include "../SaveGame.hpp"

//Buff::Buff(Entity * target, int duration) : duration(duration), target(target)
//{
//}


Buff::Buff(int duration) : duration(duration)
{
}

bool Buff::has_expired()
{
    return duration <= 0;
}

void Buff::tick()
{
    // Decrease number of turns remaining
    duration--;
}

/**
 * Function which is called once when the buff expires.
 */
void Buff::expire()
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

void DelayedMessageBuff::expire()
{
    MessageLog::singleton().add_message({text, color});
}

bool DelayedMessageBuff::disables_entity()
{
    return false;
}

json DelayedMessageBuff::to_json()
{
    json j;

    j["subclass"] = "DelayedMessageBuff";

    j["duration"] = duration;
    j["text"] = text;
    j["color"] = tcodcolor_to_json(color);

    return j;
}

DelayedMessageBuff * DelayedMessageBuff::from_json(json j)
{
    DelayedMessageBuff * dmb = new DelayedMessageBuff(
        j["duration"], j["text"], json_to_tcodcolor(j["color"]));

    return dmb;
}
