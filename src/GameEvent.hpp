#ifndef R20177_GAME_EVENT
#define R20177_GAME_EVENT

#include <string>
#include "libtcod.hpp"

#include "EventTrigger.hpp"

// Forward declarations
//class EventTrigger;
class GameState;
class GameMap;

class GameEvent
{
    public:

        unsigned int unlock_doors_id;
        std::string log_message;
        TCODColor log_message_color;

        // Methods

        GameEvent();

        void resolve(GameMap *);
};

class TriggeredEvent
{
    public:

        EventTrigger trigger;
        GameEvent event;

        TriggeredEvent(EventTrigger, GameEvent);

        bool does_trigger(GameState *);
        void resolve(GameMap *);
};

#endif
