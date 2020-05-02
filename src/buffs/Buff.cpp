#include "Buff.hpp"

#include "../GameMessages.hpp"

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

json Buff::to_json()
{
    // TODO to remove, will be virtual
    json j;

    j["duration"] = duration;

    return j;
}

Buff * Buff::from_json(json j)
{
    // TODO to change with the right version
    
    //Buff * b = new Buff(j["duration"]);

    //return b;

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
