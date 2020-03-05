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
#include "../../components/Container.hpp"
#include "../../components/Fighter.hpp"
#include "../../components/Equipment.hpp"

#include "../../EquipmentSlots.hpp"

// Prefabs
#include "../../prefabs/enemies.hpp"
#include "../../prefabs/misc/map_features.hpp"
#include "../../prefabs/funcs/misc.hpp"
#include "../../prefabs/weapons/melee.hpp"
#include "../../prefabs/weapons/ranged.hpp"
#include "../../prefabs/armor.hpp"

namespace shooting_range {

//GameMap generate_dungeon_level(width, height, min_room_length, max_room_length)
GameMap * generate_map(int width, int height, Overseer ** overseer)
{

    GameMap * level = new GameMap(width, height);
    level->dungeon_level = 1;

    //////////////////////////////
    ///////// MAIN ROOM //////////
    //////////////////////////////
    
    // Add room to level
    level->add_part(new Room(
        Rect(2, 2, 20, 20), Direction::FourD()));

    // Player starting point
    Entity * entry_point = new Entity(
        4, 4, ' ',
        TCODColor::white, "", NONE, false, false, true);
    entry_point->tag = "entrypoint"; 

    level->add_entity(entry_point);

    level->add_entity(make_pistol(5, 3));
  
    // Add full cover
    level->make_wall(7, 15);

    level->add_entity(make_security_droid(8, 15));

    // Overseer
    (*overseer) = new Overseer();

    ////////////////////////////////
    ///////// GAME EVENTS //////////
    ////////////////////////////////
   
    return level;
}


} // namespace
