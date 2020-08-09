#include "BuffStun.hpp"
#include "../GameMessages.hpp"
#include "../Entity.hpp"
#include <sstream>

//BuffStun::BuffStun(Entity * target, int duration) : Buff(target, duration)
//{
//}

BuffStun::BuffStun(int duration, bool add_log_message) :
    Buff(duration), add_log_message(add_log_message)
{
}

//BuffStun::BuffStun(int duration) : Buff(duration)
BuffStun::BuffStun(int duration) : BuffStun(duration, true)
{
}

BuffStun * BuffStun::clone()
{
    return new BuffStun(duration, add_log_message);
}

void BuffStun::_apply()
{
    //DEBUG("ALM: " << add_log_message);
    if (add_log_message)
    {
        // Build message
        std::ostringstream stringStream;

        stringStream << target->name << " is stunned!";

        // Add message to message log
        MessageLog::singleton().add_message(
            {stringStream.str(), TCODColor::yellow});
    }
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
    j["add_log_message"] = add_log_message;

    return j;

}

BuffStun * BuffStun::from_json(json j)
{
    BuffStun * bs = new BuffStun(j["duration"], j["add_log_message"]);

    return bs;
}
