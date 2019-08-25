#include "Usable.hpp"

json Usable::to_json()
{
    json j;

    // TODO just so that there is something
    j["null"] = true;

    return j;
}

Usable * Usable::from_json(json j)
{
    // TODO There is nothing in this component so far
    Usable * c = new Usable();

    return c;
}
