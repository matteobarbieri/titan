#include "BuffStun.hpp"
#include "../GameMessages.hpp"
#include "../Entity.hpp"
#include <sstream>

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

void BuffStun::_apply(Entity * e)
{
    // Build message
    std::ostringstream stringStream;

    stringStream << e->name << " is stunned!";

    // Add message to message log
    MessageLog::singleton().add_message(
        {stringStream.str(), TCODColor::yellow});

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
