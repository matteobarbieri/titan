#include "Usable.hpp"


#include "../utils.hpp"

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

void Usable::use()
{
    return _use();
}

void AOEUsable::_use()
{
}

bool Targetable::is_in_range(int src_x, int src_y, int trg_x, int trg_y)
{

    // TODO must change

    if (l2(src_x, src_y, trg_x, trg_y ) <= range)
    {
        return true;
    }

    return false;

}

bool Targetable::is_in_radius(int src_x, int src_y, int trg_x, int trg_y)
{

    // TODO must change

    if (l2(src_x, src_y, trg_x, trg_y ) <= radius)
    {
        return true;
    }

    return false;

}
