#include "Stairs.hpp"

Stairs::Stairs(int floor) : floor(floor)
{
}

json Stairs::to_json()
{
    json j;

    j["floor"] = floor;

    return j;
}

Stairs * Stairs::from_json(json j)
{
    Stairs * c = new Stairs(j["floor"]);

    return c;
}
