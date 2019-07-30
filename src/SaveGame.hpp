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

// Abstract class for visitor

/*
class Visitor
{
    public:
        virtual void visit(Entity & el) = 0;
        virtual void visit(GameMap & el) = 0;

};

class SaveVisitor : public Visitor
{
    public:

        json save_data;

        void visit(Entity & el) override;
        void visit(GameMap & el) override;
};
*/

class SaveGame
{
    private:

    public:
        //SaveGame();

        void save(const char * save_file,
             Entity * player, GameMap * game_map);

        void load(const char * save_file,
             Entity ** player, GameMap ** game_map);

};


#endif /* ifndef ROGUE_20177_SAVE_GAME*/
