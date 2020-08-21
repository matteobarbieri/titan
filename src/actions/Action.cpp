#include "../libtcod.hpp"

#include "Action.hpp"

#include "../map/GameMap.hpp"
#include "../GameState.hpp"
#include "../Entity.hpp"

#include "Outcome.hpp"

/** An action performed by a player 
 */

Action::Action()
{
}

/*
    def set_context(self, game_map, player, message_log, fov_map,
                    game_state):

        self.game_map = game_map
        self.player = player
        self.message_log = message_log
        self.fov_map = fov_map
        # self.entity_targeted = entity_targeted
        self.game_state = game_state
*/

// TODO consider whether to add also message log
void Action::set_context(
            GameMap * game_map, Entity * player, TCODMap * fov_map,
            GameState * game_state)
{
    
    // Set object attributes
    this->game_map = game_map;
    this->player = player;
    this->game_state = game_state;
    this->fov_map = fov_map;
    
}

/**
 * Virtual function, should never be called
 */
Outcome * Action::_execute()
{
    return 0;
}

Outcome * Action::execute()
{
    // Simply return the outcome of the proper subclass implementation of
    // _execute()
    return _execute();
}

Outcome * ToggleFullscreenAction::_execute()
{

    TCODConsole::setFullscreen(!TCODConsole::isFullscreen());

    return nullptr;
}

Outcome * CancelAction::_execute()
{
    // Throw exception to go to main menu;
    throw CancelActionException();
    //return nullptr;
}

Outcome * ShowHelpAction::_execute()
{
    
    Outcome * outcome = new Outcome(
        POPUP_MESSAGE,
        false,
        false);

    return outcome;
}
