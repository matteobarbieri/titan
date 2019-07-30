#include <fstream>
#include <iomanip>

#include "SaveGame.hpp"

#include "Entity.hpp"
#include "map/GameMap.hpp"

#include "libtcod.hpp"

#include "nlohmann/json.hpp"

using json = nlohmann::json;

json tcodcolor_to_json(TCODColor c)
{
    json json_data;

    json_data.push_back(c.r);
    json_data.push_back(c.g);
    json_data.push_back(c.b);

    return json_data;
}

/*
void SaveVisitor::visit(GameMap & el)
{
    std::cout << "SaveVisitor::visit(GameMap & el) not yet implemented!" << std::endl;
}

void SaveVisitor::visit(Entity & el)
{
    std::cout << "SaveVisitor::visit(Entity & el) not yet implemented!" << std::endl;
}
*/

void SaveGame::save(const char * save_file,
     Entity * player, GameMap * game_map)
{

    std::cout << "Inside SaveGame::save(): Must implement game save!" << std::endl;
    json save_data;
    //SaveVisitor sv;

    // Save player data
    save_data["player"] = player->to_json();
    save_data["game_map"] = game_map->to_json();


    // write prettified JSON to another file
    std::ofstream o(save_file);
    //o << std::setw(4) << save_data << std::endl;
    o << save_data << std::endl;
    
}
