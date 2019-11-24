#include "GameEvent.hpp"

#include "libtcod.hpp"

#include "GameState.hpp"
#include "Entity.hpp"

#include "map/GameMap.hpp"
#include "components/Interactive.hpp"

#include "GameMessages.hpp"

#include "prefabs/funcs/misc.hpp"

TriggeredEvent::TriggeredEvent(EventTrigger * et): trigger(et)
{
}

bool TriggeredEvent::does_trigger(GameState * game_state)
{
    return trigger->does_trigger(game_state);
}

void TriggeredEvent::resolve(Entity * player, GameMap * game_map, GameState * game_state)
{
    for (int i=0; i<(int)effects.size(); i++)
    {
        effects[i]->apply(player, game_map, game_state);
    }
}
