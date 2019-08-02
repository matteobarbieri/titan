#ifndef ROGUE_20177_SAVE_GAME
#define ROGUE_20177_SAVE_GAME

// Json library
#include "nlohmann/json.hpp"

#include "libtcod.hpp"

using json = nlohmann::json;

json tcodcolor_to_json(TCODColor c);
TCODColor json_to_tcodcolor(json j);

// Forward declarations
class Entity;
class GameMap;
class GameState;

class SaveGame
{
    private:

    public:
        //SaveGame();

        void save(const char * save_file,
             Entity * player, GameMap * game_map, GameState * game_state);

        void load(const char * save_file,
             Entity ** player, GameMap ** game_map, GameState ** game_state);

};


#endif /* ifndef ROGUE_20177_SAVE_GAME*/
