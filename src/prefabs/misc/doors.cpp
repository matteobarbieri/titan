#include "../../libtcod.hpp"

#include "../../Entity.hpp"

Entity * make_door(int x, int y)
{

    Entity * door = new Entity(
        x, y, '+',
        TCODColor::orange, "Door",
        STAIRS,
        true, true);

    return door;
}
