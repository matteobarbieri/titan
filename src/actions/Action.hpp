#ifndef R20177_ACTION
#define R20177_ACTION

#include "../libtcod.hpp"

// Forward declaration
class Outcome;
class GameMap;
class GameState;
class Entity;

/** An action performed by a player 
 */

class Action
{

    public:

        Action();

        void set_context(
            GameMap * game_map, Entity * player, TCODMap * fov_map,
            GameState * game_state);

        /*
         * The actual code that must be reimplemented by children classes.
         * Returns the outcome
         */
        virtual Outcome * _execute();

        /*
         * Wrapper function. The actual outcome is computed by the _execute
         * method, saved (in order to allow replay) and returned.
         */
        virtual Outcome * execute();


        GameMap * game_map;
        Entity * player;
        TCODMap * fov_map;
        GameState * game_state;

};


class ExitGameException: public std::exception
{
};


#endif

/*

class NoopAction(Action):
    pass


class WaitAction(Action):
    """
    Wait one turn without doing anything
    """

    def _execute(self):
        # Return outcome
        outcome = {
            "next_state": GamePhase.ENEMY_TURN,
            "redraw_terrain": True,
        }

        return outcome
*/
