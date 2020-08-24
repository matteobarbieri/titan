#include "libtcod.hpp"

#include "map/GameMap.hpp"

#include "Entity.hpp"
#include "Player.hpp"
//#include "skills/Skill.hpp"
#include "skills/SkillStun.hpp"

#include "RenderOrder.hpp"
#include "Overseer.hpp"

#include "GameState.hpp"
#include "GamePhase.hpp"

// Components
#include "components/Fighter.hpp"
#include "components/Inventory.hpp"
#include "components/Level.hpp"
#include "components/Equipment.hpp"

namespace tutorial {
    GameMap * generate_room(int, int);
}

namespace prologue {
    GameMap * generate_map(int, int, Overseer **);
}

namespace shooting_range {
    GameMap * generate_map(int, int, Overseer **);
}

namespace test_room {
    GameMap * generate_room(int, int);
}

void init_new_game(
    GameMap ** game_map, Entity ** player, GameState ** game_state, Overseer ** overseer)
{

    // Generate a small room
    //* game_map = test_room::generate_room(
        //MAP_WIDTH, MAP_HEIGHT);

    // Generate shooting range map
    //* game_map = shooting_range::generate_map(
        //MAP_WIDTH, MAP_HEIGHT, overseer);

    // Generate prologue map
    * game_map = prologue::generate_map(
        MAP_WIDTH, MAP_HEIGHT, overseer);

    // Generate tutorial map
    //* game_map = tutorial::generate_room(
        //MAP_WIDTH, MAP_HEIGHT);

    /*

    */

    // Fighter component for player
    Fighter * fighter_component = new Fighter(30, 50, 10);

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
    inventory_component->owner = (* player); 

    (* player)->level = level_component; 
    level_component->owner = (* player); 

    (* player)->equipment = equipment_component; 
    equipment_component->owner = (* player); 

    // Place player in the map
    (*game_map)->place_player(* player);

    // Initialize player singleton
    Player::singleton().entity = (*player);

    // Add skills
    // TODO should be probably done elsewhere!
    // First, empty the list of skills
    Player::singleton().skills.clear();

    // Temporarily disable stun skill
    //Skill * skill_stun = new SkillStun("Stun", "data/graphics/icons/skills/skill_stun.png");
    //Player::singleton().skills.push_back(skill_stun);

    //Skill * skill_parry = new Skill("Parry", "data/graphics/icons/skills/skill_parry.png");
    //Player::singleton().skills.push_back(skill_parry);
    
    SkillBlink * skill_parry = new SkillBlink("Parry", "data/graphics/icons/skills/skill_parry.png", 5);
    Player::singleton().skills.push_back(skill_parry);

    Player::singleton().preload_skill_textures(TCOD_sys_get_sdl_renderer());

    ////////////////////////////////////////////
    // Begin the game in player's turn
    (*game_state) = new GameState();
    //(*game_state)->game_phase = PLAYERS_TURN;
    (*game_state)->game_phase = POPUP_MESSAGE;

    // Set game state reference to Overseer
    (*overseer)->player = *player;
    (*overseer)->game_map = *game_map;
    (*overseer)->game_state = *game_state;
}
