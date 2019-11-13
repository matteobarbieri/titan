#include <iostream>
#include <utility>

// TODO check if file stays in place
#include "../GameMap.hpp"

#include "../../RenderOrder.hpp"
#include "../../Overseer.hpp"
#include "../../EventTrigger.hpp"
#include "../../GameEvent.hpp"

#include "../../libtcod.hpp"
#include "../../Entity.hpp"

// Components
#include "../../components/Stairs.hpp"
#include "../../components/Interactive.hpp"
#include "../../components/Item.hpp"
#include "../../components/Usable.hpp"

// Prefabs
#include "../../prefabs/enemies.hpp"
#include "../../prefabs/misc/doors.hpp"
#include "../../prefabs/weapons/melee.hpp"
#include "../../prefabs/weapons/ranged.hpp"

namespace prologue {

/**
 * Add a single immobile orc in the room
 */
void add_monsters(GameMap * level);

/**
 * Add a dagger in the room
 */
void add_items(GameMap * level);

//GameMap generate_dungeon_level(width, height, min_room_length, max_room_length)
GameMap * generate_map(int width, int height, Overseer ** overseer)
{

    GameMap * level = new GameMap(width, height);
    level->dungeon_level = 1;

    //////////////////////////////
    /////////// CELLS ////////////
    //////////////////////////////
    
    // CELL 1
    Room * cell_1 = new Room(
        Rect(5, 8, 11, 12), Direction::FourD());

    // Add room to level
    level->add_part(cell_1);

    // Make a locked door
    unsigned int key_id_cell1 = 1;
    level->make_floor(12, 10);
    Entity * d2 = make_door(12, 10, false, true, key_id_cell1);

    level->add_entity(d2);
    
    // Add a window
    level->make_window(12, 9);

    // CELL 2
    Room * cell_2 = new Room(
        Rect(5, 14, 11, 18), Direction::FourD());

    // Add room to level
    level->add_part(cell_2);

    // Create and add entry stairs '<'
    //Stairs * up_stairs_component = new Stairs(level->dungeon_level - 1);

    Entity * entry_point = new Entity(
        6, 16, ' ',
        TCODColor::white, "ENTRY_POINT", NONE, false, false, true);

    level->add_entity(entry_point);
    
    // Make a locked door
    unsigned int key_id_cell2 = 2;
    level->make_floor(12, 16);
    Entity * door_cell2 = make_door(12, 16, false, true, key_id_cell2);

    level->add_entity(door_cell2);
    
    // Add a window
    level->make_window(12, 15);
    
    // CELL 3
    Room * cell_3 = new Room(
        Rect(5, 20, 11, 24), Direction::FourD());

    // Add room to level
    level->add_part(cell_3);

    // Make a locked door
    unsigned int key_id_cell3 = 2;
    level->make_floor(12, 22);
    Entity * door_cell3 = make_door(12, 22, false, true, key_id_cell3);

    level->add_entity(door_cell3);
    
    // Add a window
    level->make_window(12, 21);
    
    // CELL 4 
    Room * cell_4 = new Room(
        Rect(5, 26, 11, 30), Direction::FourD());

    // Add room to level
    level->add_part(cell_4);

    // Make a locked door
    unsigned int key_id_cell_4 = 2;
    level->make_floor(12, 28);
    Entity * door_cell4 = make_door(12, 28, false, true, key_id_cell_4);

    level->add_entity(door_cell4);
    
    // Add a window
    level->make_window(12, 27);
   
    //////////////////////////////
    ////////// CORRIDOR //////////
    //////////////////////////////
    
    Room * corridor = new Room(
        Rect(13, 8, 18, 30), Direction::FourD());

    // Add room to level
    level->add_part(corridor);

    /*
    // Add key for room 4
    Entity * door_key = new Entity(
        x1+2, y1+2, 208, 
        TCODColor::green, "Key card", ITEM, false, false, true);

    // Create item component
    Item * key_item_component = new Item(
        ItemType::MISC, ItemSubtype::NONE);

    // The key for the locked door
    unsigned int key_id = 1;
    key_item_component->key_id = key_id;

    // Attach item component to entity
    door_key->item = key_item_component;
    key_item_component->owner = door_key;

    level->add_entity(door_key);

    //////////////////////////////
    /////////// ROOM 3 ///////////
    //////////////////////////////
    
    y1 -= 8;
    w = 15; h = 7;
    
    Rect xy3(x1, y1, x1+w-1, y1+h-1);
    Room * r3 = new Room(xy3, Direction::FourD());

    // Add room to level
    level->add_part(r3);

    // Make a door
    level->make_floor(x1+w/2, y1+h);
    Entity * d3 = make_door(x1+w/2, y1+h);

    level->add_entity(d3);

    //////////////////////////////
    /////////// ROOM 4 ///////////
    //////////////////////////////
    
    y1 += 20;
    w = 15; h = 7;
    
    Rect xy4(x1, y1, x1+w-1, y1+h-1);
    Room * r4 = new Room(xy4, Direction::FourD());

    // Add room to level
    level->add_part(r4);

    // Make a door
    level->make_floor(x1+w/2, y1-1);
    Entity * d4 = make_door(x1+w/2, y1-1, true, key_id);

    level->add_entity(d4);

    //Entity * droid = make_security_droid(r4);
    Entity * droid = make_security_droid(entry_room);
    level->add_entity(droid);

    */

    // Monsters
    //add_monsters(level);

    // Add some items in the room
    //add_items(level);

    (*overseer) = new Overseer();

    ////////////////////////////////
    ///////// GAME EVENTS //////////
    ////////////////////////////////

    // At turn 10, the door unlocks
    GameEvent gev1 = GameEvent();
    gev1.log_message = "A loud noise wakes you up; looks like something hit the ship.";
    gev1.log_message_color = TCODColor::amber;
    gev1.unlock_doors_id = 2;

    TriggeredEvent ev1 = TriggeredEvent(EventTrigger(10), gev1);

    (*overseer)->scheduled_events.push_back(ev1);

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

    int x = level->width/2 - 2;
    int y = level->height/2 - 2;

    Entity * dagger = make_dagger();
    dagger->x = x;
    dagger->y = y;

    level->add_entity(dagger);
}

} // namespace
