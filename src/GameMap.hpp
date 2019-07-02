#ifndef R20177_GAME_MAP
#define R20177_GAME_MAP

#include "Constants.h"

#include "libtcod.hpp"

#include <vector>

// TODO consider moving this to a separate file
class DijkstraMap
{
};

// TODO consider moving this to a separate file
class Direction
{
};

/**
 * A map level
 */
class GameMap
{
    public:

        int width;
        int height;
        
        GameMap(int w, int h);
        ~GameMap();

};

class MapPart
{

    public:

        DijkstraMap d_map;

        // Constructor
        MapPart(int x, int y, std::vector<Direction> available_directions);

        bool has_tile(int x, int y);

        bool is_connected_to(MapPart other);

        void remove_connection(MapPart other);

        void connect_to(MapPart other);

        float distance_from(MapPart other);

        void reset_available_directions();

        bool intersects_with(MapPart other);

        void create_dijkstra_map(GameMap game_map);

        bool has_available_directions();

        // TODO check parameters, probably requires reference passing
        void pick_starting_point();
        
        int height();

        int width();

        // TODO check parameters
        void center(int & xc, int & yc);

        void dig(GameMap game_map, int pad=0);

};

#endif

/*
import logging

# from item_functions import cast_confuse, cast_fireball, cast_lightning, heal

from map_objects.tile import Tile, Wall, Floor
from map_objects.tile import Door as DoorTile

# TODO temporarily disabled
# from components.ai import BasicMonster
# from components.equipment import EquipmentSlots
# from components.equippable import Equippable
# from components.fighter import Fighter
# from components.item import Item
# from components.stairs import Stairs

import random

import shelve

# TODO temporarily disabled
# from random_utils import from_dungeon_level, random_choice_from_dict

from .map_utils import dig_rect, _intersection_area


class Door(MapPart):
    """
    Represents a door in the dungeon. Can be bigger than one tile (to represent
    large gates).
    Has a state, can be open or closed
    """

    def __init__(self, xy, is_open=False):
        super().__init__(xy)

        self.is_open = is_open

    def dig(self, game_map):
        """
        Create the door in the map
        """

        # Unpack coordinates
        x1, y1, x2, y2 = self.xy

        for x in range(x1, x2+1):
            for y in range(y1, y2+1):
                game_map.tiles[x][y] = DoorTile()


class Room(MapPart):

    def __init__(self, xy, available_directions):
        super().__init__(xy, available_directions)

    def dig(self, game_map):
        """
        Actually dig the map part in the game map.
        """

        super().dig(game_map, pad=1)

        # # Also dig the door, if there is one
        # if self.door_xy is not None:
            # x, y = self.door_xy

            # dig_rect(game_map, [x, y, x, y])


class Junction(MapPart):

    def __init__(self, xy, available_directions):
        super().__init__(xy, available_directions)


class Corridor(MapPart):

    def __init__(self, xy, available_directions, horizontal):
        super().__init__(xy, available_directions)

        # Save the information about the placement of the tunnel
        self.horizontal = horizontal

    @property
    def vertical(self):
        return not self.horizontal

    def pick_room_starting_point(self):
        """
        Pick a starting point for a viable room
        """

        # Unpack coordinates
        x1, y1, x2, y2 = self.xy

        if self.horizontal:
            if random.random() < 0.5:
                # Room on top
                d = Direction.NORTH
                y = y1 - 1
                x = random.randint(x1+1, x2-1)
            else:
                # Room on bottom
                d = Direction.SOUTH
                y = y2 + 1
                x = random.randint(x1+1, x2-1)
        else:
            if random.random() < 0.5:
                # Room on the left
                d = Direction.WEST
                x = x1 - 1
                y = random.randint(y1+1, y2-1)
            else:
                # Room on the right
                d = Direction.EAST
                x = x2 + 1
                y = random.randint(y1+1, y2-1)

        return x, y, d


class GameMap:



    @property
    def all_parts(self):
        """
        Returns the list of all major map parts
        (excludes doors)
        """
        return self.junctions + self.corridors + self.rooms

    def get_player_starting_coords(self):

        # TODO!!!
        for e in self.entities:
            if e.char == '<':
                return e.x, e.y

    def get_item_at(self, x, y):
        """
        If there is an item on the floor at coordinates x, y return it, else
        return None.
        """

        for e in self.entities:
            if e.x == x and e.y == y and e.item:
                return e

        else:
            return None

    def place_player(self, player):
        """
        Place player in the map
        """

        # TODO
        # Should place him/her in an entry/exit tile (depending on where they
        # came from).
        # starting_room = random.choice(self.rooms)

        x, y = self.get_player_starting_coords()
        player.x = x
        player.y = y

        # Add player to list of entities
        self.entities.append(player)

    def add_part(self, part):
        """
        Add a part to the map and also dig it
        """
        if ( type(part) == Corridor):
            self.add_corridor(part)
        elif type(part) == Junction:
            self.add_junction(part)
        elif type(part) == Room:
            self.add_room(part)
        elif type(part) == Door:
            self.add_door(part)

        # Create the tiles accordingly
        part.dig(self)

    def add_corridor(self, corridor):
        """
        """

        self.corridors.append(corridor)

    def add_junction(self, junction):
        """
        """

        self.junctions.append(junction)

    def add_door(self, door):
        """
        """

        self.doors.append(door)

    def add_room(self, room):
        """
        """

        self.rooms.append(room)

    def can_place(self, part):
        """
        Returns True if part can be placed in map (i.e. doesn't intersect with
        anything which is already present.

        Also needs to be in a valid tile

        Does not depend on tiles array
        """

        # First check that coordinates refer to a valid tile in the map
        # Unpack coordinates
        x1, y1, x2, y2 = part.xy

        # Check that it lies within the whole map
        geometry_check = (
            x1 > 0 and y1 > 0 and \
            x2 < (self.width - 1) and y2 < (self.height - 1)
        )

        if not geometry_check:
            return False

        # Then check for potential intersections with other elements of the map

        # Build the list of alla placed parts
        placed_parts = self.rooms + self.junctions + self.corridors

        for other in placed_parts:
            if part.intersects_with(other):
                return False

        return True

    def initialize_tiles(self):
        tiles = [
                [
                    Tile(True) for y in range(self.height)
                ] for x in range(self.width)
        ]

        return tiles

    def export_shelf(self, destination):
        with shelve.open(destination) as db:
            db['level'] = self

    def export_txt(self, txt_file):

        entities_dict = dict()

        for e in self.entities:
            entities_dict[(e.x, e.y)] = e

        with open(txt_file, 'w') as tf:
            for y in range(self.height):
                for x in range(self.width):

                    e = entities_dict.get((x, y))

                    # If there is an entity, show that
                    if e is not None:
                        tf.write(e.char)

                    # Else show the terrain element
                    else:
                        t = self.tiles[x][y]
                        if type(t) == Floor:
                            tf.write(".")
                        elif type(t) == DoorTile:
                            tf.write("+")
                        elif type(t) == Wall:
                            tf.write("#")
                        else:
                            tf.write(" ")

                tf.write("\n")


    def is_blocked(self, x, y):
        if self.tiles[x][y].blocked:
            return True

        return False
*/
