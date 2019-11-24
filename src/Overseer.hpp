#ifndef R20177_OVERSEER
#define R20177_OVERSEER

#include "Constants.h"

#include "libtcod.hpp"

#include <utility>
#include <vector>

// Forward declarations
class Entity;
class GameMap;
class GameState;
class TriggeredEvent;

#include "nlohmann/json.hpp"

using json = nlohmann::json;

/**
 * A class that oversees the game and triggers event and stuff.
 */
class Overseer
{
    public:

        /**
         * A pointer to current game state
         */
        Entity * player;

        /**
         * A pointer to current game state
         */
        GameMap * game_map;

        /**
         * A pointer to current game state
         */
        GameState * game_state;

        // The list of scheduled events
        std::vector<TriggeredEvent *> scheduled_events;

        //Overseer(GameState *);
        Overseer();

        /**
         * Check for scheduled events and trigger them if the conditions are
         * satisfied.
         */
        void trigger_events();

        json to_json();

        /**
         * Do not restore pointers to player, game_map and game state (those
         * are restored externally).
         */
        static Overseer * from_json(json);
};

#endif // ifndef R20177_OVERSEER
