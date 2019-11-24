#include "TriggeredEvent.hpp"

#include "GameState.hpp"
#include "Entity.hpp"

#include "map/GameMap.hpp"
#include "components/Interactive.hpp"

#include "GameMessages.hpp"

#include "prefabs/funcs/misc.hpp"

TriggeredEvent::TriggeredEvent(EventTrigger * et): trigger(et)
{
}

bool TriggeredEvent::does_trigger(Entity * player, GameMap * game_map, GameState * game_state)
{
    return trigger->does_trigger(player, game_map, game_state);
}

void TriggeredEvent::resolve(Entity * player, GameMap * game_map, GameState * game_state)
{
    for (int i=0; i<(int)effects.size(); i++)
    {
        effects[i]->apply(player, game_map, game_state);
    }
}

json TriggeredEvent::to_json()
{
    json j;

    // The trigger
    j["trigger"] = trigger->to_json();

    // All effects
    json j_effects;

    for (int i=0; i<(int)effects.size(); i++)
    {
        j_effects.push_back(effects[i]->to_json());
    }

    j["effects"] = j_effects;

    return j;
}

TriggeredEvent * TriggeredEvent::from_json(json j)
{
    TriggeredEvent * te = new TriggeredEvent(
        EventTrigger::from_json(j["trigger"]));

    for (int i=0; i<(int)j["effects"].size(); i++)
    {
        te->effects.push_back(Effect::from_json(j["effects"][i]));
    }

    return te;
}
