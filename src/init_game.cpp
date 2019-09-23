#include "libtcod.hpp"

#include "map/GameMap.hpp"

#include "Entity.hpp"
#include "Player.hpp"
#include "skills/Skill.hpp"

#include "RenderOrder.hpp"

#include "GameState.hpp"
#include "GamePhase.hpp"

// Components
#include "components/Fighter.hpp"
#include "components/Inventory.hpp"
#include "components/Level.hpp"
#include "components/Equipment.hpp"

/*
import libtcodpy as libtcod

from components.equippable import Equippable
from components.item import Item

from prefabs.weapons.dagger import make_dagger

from entity import Entity

from equipment_slots import EquipmentSlots

from game_messages import MessageLog

from game_state import GamePhase

# from map_objects.old import GameMap

from render_functions import RenderOrder

from map_objects.generators.dungeon import generate_dungeon_level as \
    generate_dungeon

from map_objects.generators.test_maps import generate_dungeon_level as \
    generate_room


def get_game_variables(constants):

*/

namespace tutorial {
    GameMap * generate_room(int, int);
}

namespace test_room {
    GameMap * generate_room(int, int);
}

void init_new_game(
    GameMap ** game_map, Entity ** player, GameState ** game_state)
{

    // Generate a small room
    //* game_map = test_room::generate_room(
        //MAP_WIDTH, MAP_HEIGHT);

    // Generate tutorial map
    * game_map = tutorial::generate_room(
        MAP_WIDTH, MAP_HEIGHT);

    /*

    */

    // Fighter component for player
    // fighter_component = Fighter(hp=100, defense=1, power=2)
    Fighter * fighter_component = new Fighter(30);

    // Iventory component for player
    // inventory_component = Inventory(26)
    Inventory * inventory_component = new Inventory(26);

    // The level component for leveling up
    // level_component = Level()
    Level * level_component = new Level();


    /*
    # The equipment component, for equippable items
    equipment_component = Equipment(
        available_slots=[
            EquipmentSlots.MAIN_HAND,
            EquipmentSlots.OFF_HAND,
            EquipmentSlots.HEAD,
            EquipmentSlots.CHEST,
            EquipmentSlots.HANDS,
            EquipmentSlots.LEGS,
            EquipmentSlots.FEET,
            ])
    */

    // TODO use a proper equipment component
    Equipment * equipment_component = new Equipment();

    /*

    player = Entity(
        0, 0, # Coordinates - useless here
        '@', libtcod.white, 'Player', # Appearance
        blocks=True,
        render_order=RenderOrder.ACTOR,
        components=dict(
            fighter=fighter_component,
            inventory=inventory_component,
            level=level_component,
            equipment=equipment_component
        )
    )
    */

    // Create the Player object
    // TODO complete this
    * player = new Entity(0, 0, '@',
               TCODColor::white, "Player",
               ACTOR,
               true, false);

    // Add components
    (* player)->fighter = fighter_component; 
    fighter_component->owner = (* player);

    (* player)->inventory = inventory_component; 
    (* player)->level = level_component; 
    (* player)->equipment = equipment_component; 

    // Place player in the map
    (*game_map)->place_player(* player);

    // Initialize player singleton
    Player::singleton().entity = (*player);

    // Add skills
    Skill * skill_stun = new Skill("Stun", "data/graphics/icons/skills/skill_stun.png");
    Player::singleton().skills.push_back(skill_stun);

    //Skill * skill_parry = new Skill("Parry", "data/graphics/icons/skills/skill_parry.png");
    //Player::singleton().skills.push_back(skill_parry);

    Player::singleton().preload_skill_textures(TCOD_sys_get_sdl_renderer());

    ////////////////////////////////////////////
    // Begin the game in player's turn
    * game_state = new GameState();
    (* game_state)->game_phase = PLAYERS_TURN;
}
