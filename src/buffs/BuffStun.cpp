#include "BuffStun.hpp"

BuffStun::BuffStun(Entity * target, int duration) : Buff(target, duration)
{
}

bool BuffStun::disables_entity()
{
    return true;
}
