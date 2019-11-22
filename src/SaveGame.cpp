#include <fstream>
#include <iomanip>

#include "SaveGame.hpp"

#include "Entity.hpp"
#include "GameState.hpp"
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

TCODColor json_to_tcodcolor(json j)
{

    return TCODColor(j[0].get<int>(), j[1].get<int>(), j[2].get<int>());
}

void SaveGame::load(const char * save_file,
     Entity ** player, GameMap ** game_map, GameState ** game_state)
{

    json save_data;

    // Read JSON from save file
    std::ifstream i(save_file);

    i >> save_data;

    // Load player data
    * player = Entity::from_json(save_data["player"]);

    // Load map data
    * game_map = GameMap::from_json(save_data["game_map"]);
   
    // Manually add back player to list of entities
    (*game_map)->add_entity(*player);

    // Load game state
    * game_state = GameState::from_json(save_data["game_state"]);

    
}

void SaveGame::save(const char * save_file,
     Entity * player, GameMap * game_map, GameState * game_state)
{

    json save_data;

    // Save player data
    save_data["player"] = player->to_json();

    //DEBUG("Removing player from entities");
    // Remove player from entities in order to avoid duplicates
    game_map->remove_entity(player);

    //DEBUG("Saving game map data");
    // Save map data
    save_data["game_map"] = game_map->to_json();

    //DEBUG("Saving player map data");
    // Save player data
    save_data["game_state"] = game_state->to_json();

    // write prettified JSON to another file
    std::ofstream o(save_file);
    //o << std::setw(4) << save_data << std::endl;
    o << save_data << std::endl;
    
}
