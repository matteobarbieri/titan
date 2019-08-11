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
        
        MessageLog * message_log;

        // Entity being inspected
        Entity * entity_focused = nullptr;

        // Entity being targeted
        Entity * entity_targeted = nullptr;

        // Inventory item being selected
        Entity * selected_inventory_item = nullptr;
        
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

/*
def update_game_state(
    outcome,
    game_state: GameState, fov_recompute, redraw_terrain,
    message_log):  # noqa

    # Update game state
    if outcome.get('next_state') is not None:
        game_state.game_phase = outcome.get('next_state')

    # Update focused entity
    if outcome.get('entity_focused') is not None:
        game_state.entity_focused = outcome.get('entity_focused')

    # Update targeted entity
    if outcome.get('entity_targeted') is not None:
        game_state.entity_targeted = outcome.get('entity_targeted')

    # Update selected inventory item
    if outcome.get('selected_inventory_item') is not None:
        game_state.selected_inventory_item = outcome.get(
            'selected_inventory_item')

    # Determine whether to recompute fov...
    if outcome.get('fov_recompute') is not None:
        fov_recompute = outcome.get('fov_recompute')
    else:
        fov_recompute = fov_recompute

    # Or redraw terrain
    if outcome.get('redraw_terrain') is not None:
        redraw_terrain = outcome.get('redraw_terrain')
    else:
        redraw_terrain = redraw_terrain

    # Add messages to the log
    if outcome.get('messages') is not None:
        for m in outcome.get('messages'):
            message_log.add_message(m)

    return fov_recompute, redraw_terrain

*/

#endif

/*
from enum import Enum, auto


class GamePhase(Enum):
    PLAYERS_TURN = auto()
    ENEMY_TURN = auto()
    PLAYER_DEAD = auto()
    INVENTORY_MENU = auto()
    INVENTORY_ITEM_MENU = auto()
    TARGETING = auto()
    LEVEL_UP = auto()
    CHARACTER_SCREEN = auto()
    ENTITY_INFO = auto()


class GameState():

    def __init__(self):
        self.entity_targeted = None
        self.entity_focused = None
        self.game_phase = None

    def is_players_turn(self):
        """
        Returns true if waiting for some kind of input from the player.
        """
        return self.game_phase in [
            GamePhase.PLAYERS_TURN,
            GamePhase.INVENTORY_MENU, GamePhase.INVENTORY_ITEM_MENU,
            GamePhase.CHARACTER_SCREEN, GamePhase.ENTITY_INFO]

    def is_enemies_turn(self):
        """
        Returns true if it's the enemies' turn.
        """

        return self.game_phase == GamePhase.ENEMY_TURN

*/
