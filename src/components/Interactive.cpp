#include "Interactive.hpp"

#include "../Entity.hpp"
#include "../map/GameMap.hpp"

void InteractiveDoor::interact(Entity * player, GameMap * game_map)
{

    // TODO works for door only!
    owner->blocks_sight(false);
    owner->blocks(false);

    owner->symbol = '-';
    game_map->fov_map->setProperties(
        owner->x, owner->y, true, true);

}
