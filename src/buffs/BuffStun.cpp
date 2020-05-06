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

json BuffStun::to_json()
{

    json j;

    j["subclass"] = "BuffStun";
    j["duration"] = duration;

    return j;

}

BuffStun * BuffStun::from_json(json j)
{
    BuffStun * bs = new BuffStun(j["duration"]);

    return bs;
}
