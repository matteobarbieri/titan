#include "../libtcod.hpp"

#include "Action.hpp"

#include "../map/GameMap.hpp"
#include "../GameState.hpp"
#include "../Entity.hpp"

#include "Outcome.hpp"

/** An action performed by a player 
 */

void Action::set_context(
            GameMap * game_map, Entity * player, TCODMap * fov_map,
            GameState * game_state)
{

    // TODO implement
    std::cout << "Implement Action::set_context!" << std::endl;
    float aa = 1/0;
}

Outcome * Action::execute()
{
    // TODO implement
    std::cout << "Implement Action::execute!" << std::endl;
    float aa = 1/0;
    return NULL;
}

/*
from game_state import GamePhase


class Action():

    def __init__(self):
        self.game_map = None
        self.player = None

    def set_context(self, game_map, player, message_log, fov_map,
                    game_state):

        self.game_map = game_map
        self.player = player
        self.message_log = message_log
        self.fov_map = fov_map
        # self.entity_targeted = entity_targeted
        self.game_state = game_state

    def _execute(self):
        """
        The actual code that must be reimplemented by children classes.

        Returns the outcome
        """
        return {}

    def execute(self):
        """
        Wrapper function. The actual outcome is computed by the _execute
        method, saved (in order to allow replay) and returned.
        """

        self.outcome = self._execute()
        return self.outcome


class NoopAction(Action):
    pass


class WaitAction(Action):
    """
    Wait one turn without doing anything
    """

    def _execute(self):
        # Return outcome
        outcome = {
            "next_state": GamePhase.ENEMY_TURN,
            "redraw_terrain": True,
        }

        return outcome
*/
