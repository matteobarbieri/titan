#include "BuffStun.hpp"

//BuffStun::BuffStun(Entity * target, int duration) : Buff(target, duration)
//{
//}

BuffStun::BuffStun(int duration) : Buff(duration)
{
}

BuffStun * BuffStun::clone()
{
    return new BuffStun(duration);
}

bool BuffStun::disables_entity()
{
    return true;
}
