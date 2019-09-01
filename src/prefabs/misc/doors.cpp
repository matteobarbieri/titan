#include "../../libtcod.hpp"

#include "../../Entity.hpp"

#include "../../components/Interactive.hpp"

Entity * make_door(int x, int y, bool locked, unsigned int key_id)
{

    Entity * door = new Entity(
        x, y, '+',
        TCODColor::orange, "Door",
        STAIRS,
        true, true, true);

    door->interactive = new InteractiveDoor(locked, key_id);
    door->interactive->owner = door;
    return door;
}
