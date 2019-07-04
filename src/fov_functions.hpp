#ifndef ROGUE_2077_FOV_FUNCTIONS
#define ROGUE_2077_FOV_FUNCTIONS

#include "libtcod.hpp"

#include "GameMap.hpp"

TCODMap * initialize_fov(GameMap * game_map);

void recompute_fov(TCODMap fov_map, int x, int y, int radius,
                   bool light_walls, TCOD_fov_algorithm_t algo=FOV_BASIC);

#endif /* ROGUE_2077_FOV_FUNCTIONS */
