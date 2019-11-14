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
    unsigned int key_id_cell1 = 2;
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
 
    // CELL 5
    Room * cell_5 = new Room(
        Rect(20, 8, 26, 12), Direction::FourD());

    // Add room to level
    level->add_part(cell_5);

    // Make a locked door
    unsigned int key_id_cell_5 = 5;
    level->make_floor(19, 10);
    Entity * door_cell5 = make_door(19, 10, false, true, key_id_cell_5);

    level->add_entity(door_cell5);
    
    // Add a window
    level->make_window(19, 9);
 
    // CELL 6 
    Room * cell_6 = new Room(
        Rect(20, 14, 26, 18), Direction::FourD());

    // Add room to level
    level->add_part(cell_6);

    // Make a locked door
    unsigned int key_id_cell_6 = 6;
    level->make_floor(19, 16);
    Entity * door_cell6 = make_door(19, 16, false, true, key_id_cell_6);

    level->add_entity(door_cell6);
    
    // Add a window
    level->make_window(19, 15);
 
    // CELL 7 
    Room * cell_7 = new Room(
        Rect(20, 20, 26, 24), Direction::FourD());

    // Add room to level
    level->add_part(cell_7);

    // Make a locked door
    unsigned int key_id_cell_7 = 7;
    level->make_floor(19, 22);
    Entity * door_cell7 = make_door(19, 22, false, true, key_id_cell_7);

    level->add_entity(door_cell7);
    
    // Add a window
    level->make_window(19, 21);
   
    // CELL 8 
    Room * cell_8 = new Room(
        Rect(20, 26, 26, 30), Direction::FourD());

    // Add room to level
    level->add_part(cell_8);

    // Make a locked door
    unsigned int key_id_cell_8 = 8;
    level->make_floor(19, 28);
    Entity * door_cell8 = make_door(19, 28, false, true, key_id_cell_8);

    level->add_entity(door_cell8);
    
    // Add a window
    level->make_window(19, 27);
   
    //////////////////////////////
    ////////// CORRIDOR //////////
    //////////////////////////////
    
    Corridor * c_1 = new Corridor(
        Rect(13, 8, 18, 30), Direction::FourD(), false);

    // Add corridor to level
    level->add_part(c_1);

    // Doors leading to second corridor
    level->make_floor(15, 31);
    level->make_floor(16, 31);

    level->add_entity(make_door(15, 31, false, false));
    level->add_entity(make_door(16, 31, false, false));

    // Add corridors
    level->add_part(new Corridor(Rect(13, 32, 18, 43), Direction::FourD(), false));
    level->add_part(new Corridor(Rect(19, 38,  37, 43), Direction::FourD(), true));

    // TODO add guard's corpse and equipment
    
    // Doors leading to third room
    level->make_floor(38, 40);
    level->make_floor(38, 41);

    level->add_entity(make_door(38, 40, false, false));
    level->add_entity(make_door(38, 41, false, false));

    // Main room with first security droid
    level->add_part(new Room(Rect(39, 35, 50, 46), Direction::FourD()));

    // TODO add security droid

    // Small room on the north side (left)
    level->make_floor(41, 34);
    level->add_entity(make_door(41, 34, false, false));

    level->add_part(new Room(Rect(39, 29, 43, 33), Direction::FourD()));

    // TODO add equipment?

    // Small room on the north side (right)
    level->make_floor(48, 34);
    level->add_entity(make_door(48, 34, false, false));

    level->add_part(new Room(Rect(46, 29, 50, 33), Direction::FourD()));

    // TODO add equipment?

    // Warden's room, on the right
    level->make_floor(51, 38);
    level->add_entity(make_door(51, 38, false, false));

    level->add_part(new Room(Rect(52, 35, 59, 41), Direction::FourD()));

    // TODO Add inactive droids
    // TODO Add terminal
    // TODO Add cells key card
    
    // Utility room with cabinet with medpack
    level->make_floor(51, 45);
    level->add_entity(make_door(51, 45, false, false));

    level->add_part(new Room(Rect(52, 43, 56, 47), Direction::FourD()));

    
    // Large room with two armed grunts
    level->make_floor(44, 47);
    level->make_floor(45, 47);

    // TODO must be unlockable from warden's room
    //level->add_entity(make_door(44, 47, false, true));
    //level->add_entity(make_door(44, 47, false, true));

    level->add_entity(make_door(44, 47, false, false));
    level->add_entity(make_door(45, 47, false, false));

    level->add_part(new Room(Rect(39, 48, 50, 68), Direction::FourD()));
    
    // TODO Add full cover 
    // TODO Add partial cover

    // Room with special monster (which will require the use of skills)
    level->make_floor(44, 69);
    level->make_floor(45, 69);

    level->add_entity(make_door(44, 69, false, false));
    level->add_entity(make_door(45, 69, false, false));

    level->add_part(new Room(Rect(41, 70, 48, 77), Direction::FourD()));

    // Corridor to escape pod room
    level->make_floor(44, 78);
    level->make_floor(45, 78);

    level->add_entity(make_door(44, 78, false, false));
    level->add_entity(make_door(45, 78, false, false));

    level->add_part(new Corridor(Rect(43, 79, 46, 87), Direction::FourD(), false));

    // Escape pod room
    level->make_floor(44, 88);
    level->make_floor(45, 88);

    level->add_entity(make_door(44, 88, false, false));
    level->add_entity(make_door(45, 88, false, false));

    level->add_part(new Room(Rect(41, 89, 48, 96), Direction::FourD()));

    // Escape pod 1
    level->make_floor(49, 90);
    level->add_entity(make_door(49, 90, false, false));

    level->add_part(new Room(Rect(50, 89, 53, 91), Direction::FourD()));

    // TODO add terminal to launch escape pod

    // Escape pod 2
    level->make_floor(49, 95);
    level->add_entity(make_door(49, 95, false, false));

    level->add_part(new Room(Rect(50, 94, 53, 96), Direction::FourD()));

    // TODO add terminal to launch escape pod

    // Corridor to boss room
    level->make_floor(40, 73);
    level->make_floor(40, 74);

    level->add_entity(make_door(40, 73, false, false));
    level->add_entity(make_door(40, 74, false, false));
    
    level->add_part(new Room(Rect(50, 94, 53, 96), Direction::FourD()));

    // Huge Boss Room
    level->make_floor(30, 73);
    level->make_floor(30, 74);

    level->add_entity(make_door(30, 73, false, false));
    level->add_entity(make_door(30, 74, false, false));
    
    level->add_part(new Room(Rect(9, 64, 29, 83), Direction::FourD()));
    
    // TODO add boss and its mechanics
    
    // Corridor from boss room to escape pods room
    level->add_part(new Corridor(Rect(31, 72, 39, 75), Direction::FourD(), false));
    
    level->make_floor(19, 84);
    level->make_floor(20, 84);

    level->add_entity(make_door(19, 84, false, false));
    level->add_entity(make_door(20, 84, false, false));
    
    level->add_part(new Corridor(Rect(18, 85, 21, 94), Direction::FourD(), false));
    level->add_part(new Corridor(Rect(22, 91, 39, 94), Direction::FourD(), false));
    
    // Doors leading to escape pod room
    level->make_floor(40, 92);
    level->make_floor(40, 93);

    level->add_entity(make_door(40, 92, false, false));
    level->add_entity(make_door(40, 93, false, false));
    
    // Storage room with special weapon
    level->make_floor(26, 63);

    // TODO room must be locked until boss is alive
    level->add_entity(make_door(26, 63, false, false));
    
    level->add_part(new Room(Rect(24, 59, 28, 62), Direction::FourD()));

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
