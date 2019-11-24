#ifndef R20177_EVENT_TRIGGER
#define R20177_EVENT_TRIGGER

#include "nlohmann/json.hpp"

using json = nlohmann::json;

// Forward declarations
class Entity;
class GameMap;
class GameState;

/**
 * 
 */
class EventTrigger
{
    public:

        // Has the event already triggered? (prevents double activation)
        bool has_triggered;

        // TODO consider using a sort of cooldown system?
        
        virtual bool does_trigger(Entity * player, GameMap * game_map, GameState * game_state) = 0;

        virtual json to_json() = 0;

        static EventTrigger * from_json(json j);
};

class TurnEventTrigger : public EventTrigger
{

    public:

        // If it is an event which will trigger on a given game turn, save that
        // turn number in this variable.
        int turn;

        // A trigger which will fire at a given turn
        TurnEventTrigger(int turn);

        bool does_trigger(Entity * player, GameMap * game_map, GameState * game_state);

        json to_json();

        static TurnEventTrigger * from_json(json j);
};

#endif // ifndef R20177_EVENT_TRIGGER
