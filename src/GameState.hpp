#ifndef R20177_GAME_STATE
#define R20177_GAME_STATE

#include "libtcod.hpp"

#include "Constants.h"
#include "Entity.hpp"

#include "Outcome.hpp"

#include "GamePhase.hpp"

/** A container for all consoles that are used in the game, except for the root
 * one, which is accessed directly via TCODConsole::root.
 */
class GameState
{
    public:

        // Attributes
        GamePhase game_phase;
        
        // Entity being inspected
        Entity * entity_focused = NULL;

        // Entity being targeted
        Entity * entity_targeted = NULL;

        // Inventory item being selected
        Entity * selected_inventory_item = NULL;

        
        // Methods

        GameState();
        ~GameState();

        bool is_players_turn();

        bool is_enemies_turn();

        // TODO does it need also message_log ?
        void update(Outcome * outcome, bool & fov_recompute,
                bool & redraw_terrain);
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

