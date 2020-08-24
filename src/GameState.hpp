#ifndef R20177_GAME_STATE
#define R20177_GAME_STATE

//#include "libtcod.hpp"

#include "Constants.h"

#include "GamePhase.hpp"
#include "GameMessages.hpp"

#include "nlohmann/json.hpp"

using json = nlohmann::json;

// Forward declarations
class Outcome;
class Entity;
class Skill;

/** A container for all consoles that are used in the game, except for the root
 * one, which is accessed directly via TCODConsole::root.
 */
class GameState
{
    public:

        /////////////////////////////////
        ////////// ATTRIBUTES ///////////
        /////////////////////////////////
        
        GamePhase game_phase;

        int current_turn;
        
        // Entity being inspected
        Entity * entity_focused = nullptr;

        // Entity being inspected
        Entity * entity_interacted = nullptr;

        // Entity being targeted
        Entity * entity_targeted = nullptr;

        // Inventory item being selected
        Entity * selected_inventory_item = nullptr;
        
        // Inventory item being selected
        Skill * selected_skill = nullptr;
        
        /////////////////////////////////
        //////////// METHODS ////////////
        /////////////////////////////////
        
        GameState();
        //~GameState();

        /**
         * Used to determine whether to read player's input
         */
        bool is_players_turn();

        bool is_enemies_turn();

        // TODO does it need also message_log ?
        void update(Outcome * outcome, bool & fov_recompute,
                bool & redraw_terrain);

        /**
         * Creates a json representation of the component
         */
        json to_json();

        static GameState * from_json(json j);
};

#endif
