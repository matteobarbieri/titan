#include "Overseer.hpp"

#include "GameState.hpp"

#include "EventTrigger.hpp"
#include "GameEvent.hpp"

Overseer::Overseer()
{
}

void Overseer::trigger_events()
{

    std::vector<TriggeredEvent *>::iterator tr_ev = scheduled_events.begin();
    while (tr_ev != scheduled_events.end())
    {
        if ((*tr_ev)->does_trigger(game_state))
        {
            (*tr_ev)->resolve(player, game_map, game_state);
            
        }
        ++tr_ev;
    }
}
