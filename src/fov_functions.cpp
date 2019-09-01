#include "libtcod.hpp"

#include "map/GameMap.hpp"
#include "map/Tile.hpp"
#include "map/map_utils.hpp"

/*
TCODMap * initialize_fov(GameMap * game_map)
{

    TCODMap * fov_map = new TCODMap(game_map->width, game_map->height);
    int tile_index;

    for (int y = 0; y < game_map->height; y++)
    {
        for (int x = 0; x < game_map->width; x++)
        {
            // Compute tile index
            tile_index = compute_tile_index(x, y, game_map->width);

            // Set tile properties
            // blocks and blocks_sight must be negated, as the fov properties
            // correspond to their opposites (`walkable` and `transparent`
            // respectively)
            fov_map->setProperties(
                x, y,
                !(game_map->tiles[tile_index])->block_sight(),
                !(game_map->tiles[tile_index])->blocked());
        }
    }

    return fov_map;
}
*/
