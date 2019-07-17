// TODO check if file stays in place
#include "../GameMap.hpp"

#include "../../libtcod.hpp"
#include "../../Entity.hpp"

// Components
#include "../../components/Stairs.hpp"

#include <iostream>

#include "../../RenderOrder.hpp"

/*
def generate_dungeon_level(width, height, min_room_length, max_room_length):
    # TODO add parameters (and use them!)

*/

// TODO implement
void add_monsters(GameMap * level);

// TODO implement
void add_items(GameMap * level);

//GameMap generate_dungeon_level(width, height, min_room_length, max_room_length)
GameMap * generate_room(int width, int height)
{

    std::cout << "Checkpoint 1 (generate_room)!" << std::endl;

    GameMap * level = new GameMap(width, height);

    int xc = width/2;
    int yc = height/2;

    // Collect coordinates in a variable
    Rect xy(xc-10, yc-6, xc+10, yc+6);

    // Add room to level
    Room * entry_room = new Room(xy, Direction::FourD());
    level->add_part(entry_room);

    // Add an external layer of walls to rooms
    //std::cout << "Adding walls" << std::endl;
    //level->add_walls();

    // Populate Dungeon with entities
    
    // Monsters
    // TODO enable
    //add_monsters(level);

    // Add some items in the room
    // TODO enable
    //add_items(level);

    // Create and add entry stairs '<'

    Stairs * up_stairs_component = new Stairs(level->dungeon_level - 1);

    // Create entity object
    Entity * up_stairs = new Entity(
        xc, yc, '<',
        TCODColor::white, "Stairs up", STAIRS);

    // Add stairs component to entity
    up_stairs->stairs = up_stairs_component;

    level->add_entity(up_stairs);

    /*
    #############################
    ######### FIN QUI ###########
    #############################

    */

    return level;
}

// TODO implement
void add_monsters(GameMap * level)
{
    std::cout << "Implement add_monsters!" << std::endl;
    float a = 1/0;
}

// TODO implement
void add_items(GameMap * level)
{
    std::cout << "Implement add_items!" << std::endl;
    float a = 1/0;
}

/*
"""
Create a dungeon-like map level, using tunnellers
"""

from ..game_map import GameMap, Room

from ..tile import Tile, Wall

from components.stairs import Stairs

from ..directions import Direction

import logging

import random

# import libtcodpy as libtcod
import tcod as libtcod

from render_functions import RenderOrder

from entity import Entity

from prefabs.orc import make_orc
from prefabs.weapons.pistol import make_pistol

from components.ai import MonsterAi


def add_monsters(level):
    """
    Add stairs for previous and next dungeon level
    """

    for room in level.rooms:
        # TODO for now, just generate an orc in every room
        if random.random() < 2:

            monster = make_orc(room, MonsterAi)

            level.entities.append(monster)


def add_items(level):
    """
    Add a ranged weapon on the floor
    """

    x = int(level.width/2) - 2
    y = int(level.height/2) - 2

    pistol = make_pistol()
    pistol.x = x
    pistol.y = y

    level.entities.append(pistol)



*/
