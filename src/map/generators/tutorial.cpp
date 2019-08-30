#include <iostream>

// TODO check if file stays in place
#include "../GameMap.hpp"

#include "../../RenderOrder.hpp"

#include "../../libtcod.hpp"
#include "../../Entity.hpp"

// Components
#include "../../components/Stairs.hpp"

// Prefabs
#include "../../prefabs/enemies.hpp"
#include "../../prefabs/misc/doors.hpp"
#include "../../prefabs/weapons/melee.hpp"
#include "../../prefabs/weapons/ranged.hpp"

namespace tutorial {

/**
 * Add a single immobile orc in the room
 */
void add_monsters(GameMap * level);

/**
 * Add a dagger in the room
 */
void add_items(GameMap * level);

//GameMap generate_dungeon_level(width, height, min_room_length, max_room_length)
GameMap * generate_room(int width, int height)
{

    GameMap * level = new GameMap(width, height);
    level->dungeon_level = 1;

    int xc = width/2;
    int yc = height/2;

    // Collect coordinates in a variable
    Rect xy(xc-5, yc-3, xc+5, yc+3);
    Room * entry_room = new Room(xy, Direction::FourD());

    // Add room to level
    level->add_part(entry_room);

    Rect xy2(xc+5+2, yc-5, xc+5+2+10, yc+5);
    Room * r2 = new Room(xy2, Direction::FourD());

    // Add room to level
    level->add_part(r2);

    // Make a door
    level->make_floor(xc+5+1, yc);
    Entity * d1 = make_door(xc+5+1, yc);
    level->add_entity(d1);


    // Populate Dungeon with entities
    // Create and add entry stairs '<'
    Stairs * up_stairs_component = new Stairs(level->dungeon_level - 1);

    // Create entity object
    Entity * up_stairs = new Entity(
        xc, yc, '<',
        TCODColor::white, "Stairs up", STAIRS);

    // Add stairs component to entity
    up_stairs->stairs = up_stairs_component;

    level->add_entity(up_stairs);

    // Monsters
    //add_monsters(level);

    // Add some items in the room
    //add_items(level);

    return level;
}

////////////////////////////////
/////// IMPLEMENTATIONS ////////
////////////////////////////////

void add_monsters(GameMap * level)
{
    Entity * orc = make_orc(level->rooms[0]);
    level->add_entity(orc);
}

// TODO implement
void add_items(GameMap * level)
{
    //std::cout << "Implement add_items!" << std::endl;
    //float a = 1/0;

    int x = level->width/2 - 2;
    int y = level->height/2 - 2;

    Entity * dagger = make_dagger();
    dagger->x = x;
    dagger->y = y;

    level->add_entity(dagger);

}

}
