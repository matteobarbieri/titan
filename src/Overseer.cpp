#include "Overseer.hpp"

#include "GameState.hpp"

#include "EventTrigger.hpp"
#include "GameEvent.hpp"

//Overseer::Overseer(GameState * game_state) : game_state(game_state)
//{

//}

Overseer::Overseer()
{

}

//void Overseer::set_game_state(GameState * game_state)
//{
    //this->game_state = game_state;
//}

void Overseer::trigger_events()
{

    std::vector<TriggeredEvent>::iterator tr_ev = scheduled_events.begin();
    while (tr_ev != scheduled_events.end())
    {
        if ((*tr_ev).does_trigger(game_state))
        {
            //DEBUG("Triggers!");
            // TODO do resolve event!
            (*tr_ev).resolve(game_map);
            
        }
        ++tr_ev;
    }
}
