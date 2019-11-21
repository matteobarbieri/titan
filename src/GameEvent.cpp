#include "GameEvent.hpp"

#include "libtcod.hpp"

#include "GameState.hpp"
#include "Entity.hpp"

#include "map/GameMap.hpp"
#include "components/Interactive.hpp"

#include "GameMessages.hpp"

GameEvent::GameEvent()
{
    // By default, do not unlock any door
    unlock_doors_id = 0;

    log_message = "";
}

void GameEvent::resolve(GameMap * game_map)
{
    if (unlock_doors_id != 0)
    {

        //DEBUG("Unlocking doors with id " << unlock_doors_id);
        //std::vector<Entity *>::iterator e = game_map->entities().begin();

        for (int i=0; i<(int)game_map->entities().size(); i++)
        {
            Entity * e = game_map->entities()[i];

            //DEBUG("Entity name: " << e->name);
            std::size_t found = e->tag.find("door");

            if (
                    found != std::string::npos &&
                    e->interactive != nullptr &&
                    ((InteractiveDoor *)e->interactive)->key_id == unlock_doors_id)
            {
                ((InteractiveDoor *)e->interactive)->unlock();
            }
        }

    }
    
    //DEBUG("Resolving event");
    //DEBUG(log_message);

    if (log_message != "")
    {
        MessageLog::singleton().add_message({log_message, log_message_color});
    }
}

TriggeredEvent::TriggeredEvent(EventTrigger et, GameEvent ev): trigger(et), event(ev)
{
}

bool TriggeredEvent::does_trigger(GameState * game_state)
{
    return trigger.does_trigger(game_state);
}

void TriggeredEvent::resolve(GameMap * game_map)
{
    event.resolve(game_map);
}
