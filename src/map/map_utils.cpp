#include "GameMap.hpp"
#include "Tile.hpp"

/*
def dig_rect(game_map, xy):
    """
    Dig a rectangle of empty space in the map

    Edges are included, so tiles (x1, y1) and (x2, y2) are dug
    """

    # Extract coordinates
    x1, y1, x2, y2 = xy

    logging.basicConfig(level=logging.INFO)

    logger = logging.getLogger()
    # logger.debug("Digging rectangle from ({}, {}) to ({}, {})".format(*xy))

    for x in range(x1, x2+1):
        for y in range(y1, y2+1):

            game_map.tiles[x][y] = Floor()

    return game_map

*/

int compute_tile_index(int x, int y, int width)
{
    return y*width + x;
}

void dig_rect(GameMap * game_map, Rect xy)
{
    
    // Variable to keep array index
    int tile_index;

    for (int x = xy.x1; x <= xy.x2; x++)
    {
        for (int y = xy.y1; y <= xy.y2; y++)
        {
            // Compute tile index
            tile_index = compute_tile_index(x, y, game_map->width);

            // Free up object
            delete game_map->tiles[tile_index];

            // Create a Floor tile in that spot
            game_map->tiles[tile_index] = new Floor();

        }
    }
}

/*
import logging

from .tile import Tile, Floor


class NoMoreSpaceException(Exception):
    pass

def area_is_available(game_map, xy):
    """
    Check that an area is made of actually empty space
    """

    1/0

    # Extract coordinates
    x1, y1, x2, y2 = xy
    
    # Check that it lies within the whole map
    check_map_area = (
        x1 > 0 and y1 > 0 and \
        x2 < (game_map.width - 1) and y2 < (game_map.height - 1)
    )

    if not check_map_area:
        return False

    for x in range(x1, x2+1):
        for y in range(y1, y2+1):

            if type(game_map.tiles[x][y]) != Tile:
                return False

    return True

def _intersection_area(xy1, xy2):  

    # TODO debug test, seed = 6834278691794201604
    # [19, 29, 27, 37] does NOT intersect with <class 'map_objects.game_map.Junction'> at [27, 33, 38, 45]

    # Unpack coordinates
    x11, y11, x12, y12 = xy1
    x21, y21, x22, y22 = xy2

    dx = min(x12, x22) - max(x11, x21) + 1
    dy = min(y12, y22) - max(y11, y21) + 1

    if (dx>=0) and (dy>=0):
        return dx*dy
    else:
        return 0

*/
