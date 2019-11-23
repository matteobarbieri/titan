#include "Buff.hpp"

Buff::Buff(Entity * target, int duration) : duration(duration), target(target)
{
}

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
