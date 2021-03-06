#ifndef ROGUE_20177_SAVE_GAME
#define ROGUE_20177_SAVE_GAME

// Json library
#include "nlohmann/json.hpp"

#include "libtcod.hpp"

using json = nlohmann::json;

/**
 * Auxiliary function to encode a TCODColor instance to a json object
 */
json tcodcolor_to_json(TCODColor c);

/**
 * Restore a TCODColor instance from a json object
 */
TCODColor json_to_tcodcolor(json j);

// Forward declarations
class Entity;
class GameMap;
class GameState;
class Overseer;

class SaveGame
{
    private:

    public:
        //SaveGame();

        void save(const char * save_file,
             Entity * player, GameMap * game_map, GameState * game_state,
             Overseer * overseer);

        void load(const char * save_file,
             Entity ** player, GameMap ** game_map, GameState ** game_state,
             Overseer ** overseer);

};


#endif /* ifndef ROGUE_20177_SAVE_GAME*/
