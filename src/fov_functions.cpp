#include "libtcod.hpp"

#include "map/GameMap.hpp"
#include "map/Tile.hpp"
#include "map/map_utils.hpp"

/*

def initialize_fov(game_map):
    fov_map = libtcod.map_new(game_map.width, game_map.height)

    for y in range(game_map.height):
        for x in range(game_map.width):
            libtcod.map_set_properties(
                fov_map,
                x,
                y,
                not game_map.tiles[x][y].block_sight,
                not game_map.tiles[x][y].blocked
            )

    return fov_map

*/

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
            fov_map->setProperties(
                x, y,
                (game_map->tiles[tile_index])->block_sight(),
                (game_map->tiles[tile_index])->blocked());
        }
    }

    return fov_map;
}

/*


def recompute_fov(fov_map, x, y, radius, light_walls=True, algorithm=0):

    libtcod.map_compute_fov(
        fov_map,
        x,
        y,
        radius,
        light_walls,
        algorithm
    )
*/
