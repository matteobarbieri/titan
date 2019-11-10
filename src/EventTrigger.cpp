#include "EventTrigger.hpp"
#include "GameState.hpp"

EventTrigger::EventTrigger(int turn) : turn(turn)
{
}

bool EventTrigger::does_trigger(GameState * game_state)
{
    
    // The comparison must be done with turn-1, as the check for events is done
    // at the end of the enemie's turn, so it should fire at the end of the turn
    // BEFORE the one specified in the turn variable.
    if (turn != -1 && game_state->current_turn == (turn-1)) 
    {
        return true;
    }

    return false;

}
