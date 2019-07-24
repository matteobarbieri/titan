#include "GameState.hpp"

#include "actions/Outcome.hpp"

GameState::GameState()
{
}

bool GameState::is_players_turn()
{

    // TODO implement
    return true;
}

bool GameState::is_enemies_turn()
{

    // TODO implement
    return false;
}

// TODO does it need also message_log ?
void GameState::update(Outcome * outcome, bool & fov_recompute,
        bool & redraw_terrain)
{

    // Update game phase
    game_phase = outcome->next_phase;

    // Update focused entity
    if (outcome->entity_focused != 0)
        entity_focused = outcome->entity_focused;

    // Update targeted entity
    if (outcome->entity_targeted != 0)
        entity_targeted = outcome->entity_targeted;

    // Update selected inventory item
    if (outcome->selected_inventory_item != 0)
        selected_inventory_item = outcome->selected_inventory_item;

    /*
    TODO this was the original python code
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
    */

    // Determine whether to recompute fov...
    fov_recompute = outcome->fov_recompute;
    // or redraw terrain
    redraw_terrain = outcome->redraw_terrain;

    // TODO enable message log
    /*
    # Add messages to the log
    if outcome.get('messages') is not None:
        for m in outcome.get('messages'):
            message_log.add_message(m)
    */

}

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
