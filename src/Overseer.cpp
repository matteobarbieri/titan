#include "Overseer.hpp"

#include "GameState.hpp"

#include "EventTrigger.hpp"
#include "TriggeredEvent.hpp"

Overseer::Overseer()
{
}

void Overseer::trigger_events()
{

    std::vector<TriggeredEvent *>::iterator tr_ev = scheduled_events.begin();
    while (tr_ev != scheduled_events.end())
    {
        if ((*tr_ev)->does_trigger(player, game_map, game_state))
        {
            (*tr_ev)->resolve(player, game_map, game_state);
            
        }
        ++tr_ev;
    }
}

json Overseer::to_json()
{
    json j;

    json j_scheduled_events;

    for (int i=0; i<(int)scheduled_events.size(); i++)
    {
        j_scheduled_events.push_back(scheduled_events[i]->to_json());
    }
    
    j["scheduled_events"] = j_scheduled_events;

    return j;
}

Overseer * Overseer::from_json(json j)
{
    Overseer * o = new Overseer();

    for (int i=0; i<(int)j["scheduled_events"].size(); i++)
    {
        o->scheduled_events.push_back(
            TriggeredEvent::from_json(j["scheduled_events"][i]));
    }

    return o;
}
