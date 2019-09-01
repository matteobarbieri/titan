#include "../libtcod.hpp"

#include "Interactive.hpp"

#include "../Entity.hpp"
#include "../map/GameMap.hpp"

#include "../GameMessages.hpp"

InteractiveDoor::InteractiveDoor(bool locked, unsigned int key_id) :
    locked(locked), key_id(key_id)
{
}

void InteractiveDoor::interact(Entity * player, GameMap * game_map)
{

    if (!locked)
    {
        owner->blocks_sight(false);
        owner->blocks(false);

        owner->symbol = '-';
        game_map->fov_map->setProperties(
            owner->x, owner->y, true, true);
    }
    else
    {
        // Door is locked, look for an appropriate key in player's inventory
        if (key_id > 0)
        {
        }
        else
        {
            MessageLog::singleton().add_message({"This door appears to be locked!", TCODColor::white});
        }
    }

}
