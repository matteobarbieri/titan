#include "Buff.hpp"

Buff::Buff(Entity * target, int duration) : duration(duration), target(target)
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
