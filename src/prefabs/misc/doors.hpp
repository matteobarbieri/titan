#ifndef ROGUE_20177_PREFABS_MISC_DOORS
#define ROGUE_20177_PREFABS_MISC_DOORS

#include <string>

#include "../../libtcod.hpp"

class Entity;
class Direction;

Entity * make_door(int x, int y, bool open=false, bool locked=false, unsigned int key_id=0);

Entity * make_text_panel(int x, int y, Direction * readable_from, std::string text,
                         TCODColor symbol_color=TCODColor::darkGreen, TCODColor=TCODColor::lightAzure, int symbol=240,
                         bool is_active=true);

#endif /* ifndef ROGUE_20177_PREFABS_MISC_DOORS */
