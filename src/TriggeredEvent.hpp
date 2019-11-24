#ifndef R20177_GAME_EVENT
#define R20177_GAME_EVENT

#include <vector>

#include "libtcod.hpp"

#include "EventTrigger.hpp"

#include "nlohmann/json.hpp"

using json = nlohmann::json;

// Forward declarations
class Entity;
class GameMap;
class GameState;

class Effect;

class TriggeredEvent
{
    public:

        EventTrigger * trigger;

        std::vector<Effect *> effects;

        // Constructor
        TriggeredEvent(EventTrigger *);

        // Checks whether conditions are such that a given event triggers
        // TODO should be able to check multiple conditions
        bool does_trigger(Entity *, GameMap *, GameState *);

        // Resolve game event
        void resolve(Entity *, GameMap *, GameState *);

        json to_json();

        static TriggeredEvent * from_json(json);
};

#endif
