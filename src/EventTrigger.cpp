#include "EventTrigger.hpp"
#include "GameState.hpp"

EventTrigger * EventTrigger::from_json(json j)
{

    if (j["subclass"] == "TurnEventTrigger")
    {
        return TurnEventTrigger::from_json(j);
    }

    DEBUG("In EventTrigger::from_json - This should not happen!");

    return nullptr;
}

TurnEventTrigger::TurnEventTrigger(int turn) : turn(turn)
{
    has_triggered = false;
}

bool TurnEventTrigger::does_trigger(Entity * player, GameMap * game_map, GameState * game_state)
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

json TurnEventTrigger::to_json()
{
    json j;

    // Set subclass
    j["subclass"] = "TurnEventTrigger";

    // Class properties
    j["turn"] = turn;

    return j;
}

TurnEventTrigger * TurnEventTrigger::from_json(json j)
{
    TurnEventTrigger * tet = new TurnEventTrigger(j["turn"]);

    return tet;
}
