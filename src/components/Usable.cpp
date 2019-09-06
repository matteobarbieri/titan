#include "Usable.hpp"

#include <math.h>

Usable::Usable()
{
}

Usable::~Usable()
{
}

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
    //Usable * c = new Usable();
    Usable * c;

    return c;
}

void AOEUsable::_use()
{
}

bool Targetable::is_in_radius(int src_x, int src_y, int trg_x, int trg_y)
{

    // TODO must change

    if (sqrt(pow(src_x-trg_x, 2) + pow(src_y-trg_y, 2)) < radius)
    {
        return true;
    }

    return false;

}
