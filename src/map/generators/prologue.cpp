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

    // Center of the map
    int xc = width/2;
    int yc = height/2;

    //////////////////////////////
    ///////// FIRST ROOM /////////
    //////////////////////////////
    
    int x1 = xc-5, y1 = yc - 3;
    int w = 11, h = 7;

    // Collect coordinates in a variable
    Rect xy(x1, y1, x1+w-1, y1+h-1);
    Room * entry_room = new Room(xy, Direction::FourD());

    // Add room to level
    level->add_part(entry_room);

    // Create and add entry stairs '<'
    //Stairs * up_stairs_component = new Stairs(level->dungeon_level - 1);

    Entity * entry_point = new Entity(
        x1+5, y1+3, ' ',
        TCODColor::white, "ENTRY_POINT", NONE, false, false, true);

    level->add_entity(entry_point);

    //////////////////////////////
    /////////// ROOM 2 ///////////
    //////////////////////////////

    x1 += w + 1;
    y1 -= 2;

    w = 15; h = 11;
    
    Rect xy2(x1, y1, x1+w-1, y1+h-1);

    //Rect xy2(xc+5+2, yc-5, xc+5+2+10, yc+5);
    Room * r2 = new Room(xy2, Direction::FourD());

    // Add room to level
    level->add_part(r2);

    // Make a locked door
    unsigned int key_id = 1;
    level->make_floor(x1-1, yc);
    Entity * d2 = make_door(x1-1, yc, false, true, key_id);

    level->add_entity(d2);
    
    // Add a window
    level->make_window(x1-1, yc-2);

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
    gev1.unlock_doors_id = 1;

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
