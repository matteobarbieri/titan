#ifndef ROGUE_20177_PREFABS_MISC_MAP_FEATURES
#define ROGUE_20177_PREFABS_MISC_MAP_FEATURES

#include <string>

#include "../../libtcod.hpp"

class Entity;
class Direction;

/**
 * Creates a door, an entity which blocks sight and can be opened (if unlocked).
 */
Entity * make_door(int x, int y,
                   bool open=false, bool locked=false,
                   unsigned int key_id=0);

/**
 * Creates a text panel which will display information in the player's log when
 * bumped into. The panel will be readable from one direction only, this way
 * they can be placed directly into walls.
 */
Entity * make_text_panel(int x, int y, Direction * readable_from,
                         std::string text,
                         TCODColor symbol_color=TCODColor::darkGreen,
                         TCODColor text_color=TCODColor::lightAzure,
                         int symbol=240,
                         bool is_active=true);

/**
 * Creates a terminal, an entity with which the player can interact and operate.
 * By default it has no functionalities, they must be added individually
 * somewhere else.
 */
Entity * make_terminal(int x, int y,
                       TCODColor symbol_color=TCODColor::darkGreen,
                       std::string terminal_name="Terminal",
                       int symbol=167,
                       bool is_active=true);

/**
 * Creates a container.
 */
Entity * make_container(int x, int y,
                        TCODColor symbol_color=TCODColor::brass,
                        std::string name="Chest",
                        int symbol='C',
                        bool locked=false,
                        unsigned int key_id=0);


#endif /* ifndef ROGUE_20177_PREFABS_MISC_MAP_FEATURES */
