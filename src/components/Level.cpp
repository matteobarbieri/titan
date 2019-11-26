#include "Level.hpp"

json Level::to_json()
{
    json j;

    // TODO just so that there is something
    j["null"] = true;

    return j;
}

Level * Level::from_json(json j)
{
    // TODO There is nothing in this component so far
    Level * c = new Level();

    return c;
}
