#include <iostream>
#include <utility>

// TODO check if file stays in place
#include "../GameMap.hpp"

#include "../../RenderOrder.hpp"
#include "../../Overseer.hpp"
#include "../../EventTrigger.hpp"
#include "../../TriggeredEvent.hpp"
#include "../../GameMessages.hpp"

#include "../../libtcod.hpp"
#include "../../Entity.hpp"

// Components
#include "../../components/Stairs.hpp"
#include "../../components/Interactive.hpp"
#include "../../components/Item.hpp"
#include "../../components/Ai.hpp"
#include "../../components/Usable.hpp"

// Prefabs
#include "../../prefabs/enemies.hpp"
#include "../../prefabs/misc/map_features.hpp"
#include "../../prefabs/funcs/misc.hpp"
#include "../../prefabs/weapons/melee.hpp"
#include "../../prefabs/weapons/ranged.hpp"
#include "../../prefabs/armor.hpp"

namespace prologue {

//GameMap generate_dungeon_level(width, height, min_room_length, max_room_length)
GameMap * generate_map(int width, int height, Overseer ** overseer)
{

    GameMap * level = new GameMap(width, height);
    level->dungeon_level = 1;

    //////////////////////////////
    /////////// CELLS ////////////
    //////////////////////////////
    
    // CELL 1
    // Add room to level
    level->add_part(new Room(
        Rect(5, 8, 11, 12), Direction::FourD()));

    // Make a locked door
    level->make_floor(12, 10);
    level->add_entity(make_door(12, 10, false, true, 2));
    
    // Add panel (Cell 01)
    level->make_floor(12, 11);
    level->add_entity(make_text_panel(12, 11, Direction::EE, "Cell 01"));
    
    // Add a window
    level->make_window(12, 9);

    // CELL 2
    // Add room to level
    level->add_part(new Room(
        Rect(5, 14, 11, 18), Direction::FourD()));

    // Player starting point
    Entity * entry_point = new Entity(
        6, 16, ' ',
        TCODColor::white, "", NONE, false, false, true);
    entry_point->tag = "entrypoint"; 

    level->add_entity(entry_point);
    
    // Make a locked door
    level->make_floor(12, 16);
    level->add_entity(make_door(12, 16, false, true, 2));

    // Add panel (Cell 02)
    level->make_floor(12, 17);
    level->add_entity(make_text_panel(12, 17, Direction::EE, "Cell 02"));

    // TODO move in the right room!
    // Add terminal
    Entity * terminal = make_terminal(6, 14);
    InteractiveTerminal * t1_interactive = (InteractiveTerminal *)terminal->interactive;
    
    // TODO remove
    level->add_entity(make_security_droid(7, 15));

    // TODO remove
    // Seeking droid in the first corridor
    
    Entity * seeking_droid = make_security_droid(14, 20, new SeekerAi());

    level->add_entity(seeking_droid);

    // Add a dagger in the initial room
    level->add_entity(make_baton(6, 15));
    level->add_entity(make_armor(7, 16));
    level->add_entity(make_pistol(8, 16));
    
    //auto test_f3 = [](Entity * player, GameMap * game_map, GameState * game_state)
    //{
        //unlock_doors(player, game_map, game_state, 3);
        //MessageLog::singleton().add_message({"Unlocking doors...", TCODColor::turquoise});
    //};

    //TerminalFunction * tf3 = new TerminalFunction("Unlock cell doors", 'a', test_f3);
    
    TerminalFunction * tf3 = new TerminalFunction("Unlock cell doors", 'a');
    tf3->effects.push_back(new UnlockDoorsEffect(3));
    tf3->effects.push_back(new AddLogMessageEffect(
        "Unlocking doors...", TCODColor::turquoise));
    
    t1_interactive->terminal_functions.push_back(tf3);
    level->add_entity(terminal);
    
    // Add a window
    level->make_window(12, 15);
    
    // CELL 3
    Room * cell_3 = new Room(
        Rect(5, 20, 11, 24), Direction::FourD());

    // Add room to level
    level->add_part(cell_3);

    // Make a locked door
    level->make_floor(12, 22);
    level->add_entity(make_door(12, 22, false, true, 2));
    
    // Add panel (Cell 03)
    level->make_floor(12, 23);
    level->add_entity(make_text_panel(12, 23, Direction::EE, "Cell 03"));
    
    // Add a window
    level->make_window(12, 21);
    
    // CELL 4 
    Room * cell_4 = new Room(
        Rect(5, 26, 11, 30), Direction::FourD());

    // Add room to level
    level->add_part(cell_4);

    // Make a locked door
    level->make_floor(12, 28);
    level->add_entity(make_door(12, 28, false, true, 2));
    
    // Add panel (Cell 04)
    level->make_floor(12, 29);
    level->add_entity(make_text_panel(12, 29, Direction::EE, "Cell 04"));
    
    // Add a window
    level->make_window(12, 27);
 
    // CELL 5
    Room * cell_5 = new Room(
        Rect(20, 8, 26, 12), Direction::FourD());

    // Add room to level
    level->add_part(cell_5);

    // Make a locked door
    level->make_floor(19, 10);
    level->add_entity(make_door(19, 10, false, true, 3));
    
    // Add panel (Cell 05)
    level->make_floor(19, 11);
    level->add_entity(make_text_panel(19, 11, Direction::WW, "Cell 05"));
    
    // Add a window
    level->make_window(19, 9);
 
    // CELL 6 
    Room * cell_6 = new Room(
        Rect(20, 14, 26, 18), Direction::FourD());

    // Add room to level
    level->add_part(cell_6);

    // Make a locked door
    level->make_floor(19, 16);
    level->add_entity(make_door(19, 16, false, true, 3));
    
    // Add panel (Cell 06)
    level->make_floor(19, 17);
    level->add_entity(make_text_panel(19, 17, Direction::WW, "Cell 06"));
    
    // Add a window
    level->make_window(19, 15);
 
    // CELL 7 
    Room * cell_7 = new Room(
        Rect(20, 20, 26, 24), Direction::FourD());

    // Add room to level
    level->add_part(cell_7);

    // Make a locked door
    level->make_floor(19, 22);
    level->add_entity(make_door(19, 22, false, true, 3));
    
    // Add panel (Cell 07)
    level->make_floor(19, 23);
    level->add_entity(make_text_panel(19, 23, Direction::WW, "Cell 07"));
    
    // Add a window
    level->make_window(19, 21);
   
    // CELL 8 
    Room * cell_8 = new Room(
        Rect(20, 26, 26, 30), Direction::FourD());

    // Add room to level
    level->add_part(cell_8);

    // Make a locked door
    level->make_floor(19, 28);
    level->add_entity(make_door(19, 28, false, true, 3));
    
    // Add panel (Cell 08)
    level->make_floor(19, 29);
    level->add_entity(make_text_panel(19, 29, Direction::WW, "Cell 08"));
    
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
    
    Entity * guard_corpse = new Entity(18, 43, '%', TCODColor::red, "Corpse of a guard");
    level->add_entity(guard_corpse);

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

    // Add panel (Cell 01)
    level->make_floor(42, 34);
    level->add_entity(make_text_panel(42, 34, Direction::SS, "Storage room 1"));
    
    level->add_part(new Room(Rect(39, 29, 43, 33), Direction::FourD()));

    // TODO add equipment?

    // Small room on the north side (right)
    level->make_floor(48, 34);
    level->add_entity(make_door(48, 34, false, false));

    // Add panel (Cell 01)
    level->make_floor(49, 34);
    level->add_entity(make_text_panel(49, 34, Direction::SS, "Storage room 2"));
    
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
    
    // Full cover
    level->make_wall(46, 53);
    level->make_wall(47, 53);
    

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

    (*overseer) = new Overseer();

    ////////////////////////////////
    ///////// GAME EVENTS //////////
    ////////////////////////////////
   
    // At turn 10, the door unlocks
    // TODO restore turn 10 (2 is for DEBUG)
    TriggeredEvent * ev1 = new TriggeredEvent(new TurnEventTrigger(2));

    ev1->effects.push_back(new UnlockDoorsEffect(2));
    ev1->effects.push_back(new AddLogMessageEffect(
        "You hear a loud noise, followed by explosions and alarms.",
        TCODColor::amber));
    ev1->effects.push_back(new AddLogMessageEffect(
        "Looks like something hit the ship.",
        TCODColor::amber));

    (*overseer)->scheduled_events.push_back(ev1);

    return level;
}

} // namespace
