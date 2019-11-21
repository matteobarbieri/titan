#ifndef R20177_OVERSEER
#define R20177_OVERSEER

#include "Constants.h"

#include "libtcod.hpp"

#include <utility>
#include <vector>

// Forward declarations
class GameState;
class GameMap;
//class EventTrigger;
//class GameEvent;
class TriggeredEvent;

/**
 * A class that oversees the game and triggers event and stuff.
 */
class Overseer
{
    public:

        /**
         * A pointer to current game state
         */
        GameState * game_state;

        /**
         * A pointer to current game state
         */
        GameMap * game_map;

        //pair
        //std::vector<std::pair<EventTrigger, GameEvent>> scheduled_events;
        std::vector<TriggeredEvent> scheduled_events;

        //Overseer(GameState *);
        Overseer();

        // Sets the game state
        //void set_game_state(GameState *);

        /**
         * Check for scheduled events and trigger them if the conditions are
         * satisfied.
         */
        void trigger_events();

};

#endif // ifndef R20177_OVERSEER
