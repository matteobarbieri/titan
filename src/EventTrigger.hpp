#ifndef R20177_EVENT_TRIGGER
#define R20177_EVENT_TRIGGER

//#include "Constants.h"

//#include "libtcod.hpp"

//#include <utility>

// Forward declarations
class GameState;

/**
 * 
 */
class EventTrigger
{
    public:

        // Has the event already triggered? (prevents double activation)
        bool has_triggered;

        // If it is an event which will trigger on a given game turn, save that
        // turn number in this variable.
        int turn;

        // TODO consider using a sort of cooldown system?
        
        // A trigger which will fire at a given turn
        EventTrigger(int turn);

        bool does_trigger(GameState * game_state);
};

#endif // ifndef R20177_EVENT_TRIGGER
