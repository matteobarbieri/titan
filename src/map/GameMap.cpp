#include "../libtcod.hpp"

#include "../Constants.h"
#include "GameMap.hpp"
#include "Tile.hpp"
#include "../Entity.hpp"

#include "../utils.hpp"
#include "map_utils.hpp"

#include <vector>
#include <algorithm>

#include "../nlohmann/json.hpp"

using json = nlohmann::json;

// TODO
// Forward declaration
json tcodcolor_to_json(TCODColor c);

/////////////////////////////////
///////////// RECT //////////////
/////////////////////////////////

// TODO refactor with initialization list
Rect::Rect(int x1, int y1, int x2, int y2)
{

    this->x1 = x1;
    this->y1 = y1;
    this->x2 = x2;
    this->y2 = y2;

}

/////////////////////////////////
/////////// DIRECTION ///////////
/////////////////////////////////

Direction::Direction(int dx, int dy) : dx(dx), dy(dy)
{
}

// Initialize static members
Direction * Direction::NN = new Direction(0, -1);
Direction * Direction::SS = new Direction(0, 1);
Direction * Direction::EE = new Direction(1, 0);
Direction * Direction::WW = new Direction(-1, 0);

std::vector<Direction *> Direction::FourD()
{
    std::vector<Direction *> directions;
    directions.push_back(Direction::NN);
    directions.push_back(Direction::SS);
    directions.push_back(Direction::EE);
    directions.push_back(Direction::WW);
    
    return directions;
}

std::string Direction::to_json()
{
    if (dx == 0 && dy == -1)
    {
        return "NN";
    }

    if (dx == 0 && dy == 1)
    {
        return "SS";
    }

    if (dx == 1 && dy == 0)
    {
        return "EE";
    }

    if (dx == -1 && dy == 0)
    {
        return "WW";
    }

    return "";
}

Direction * Direction::from_json(std::string j)
{
    if (j == "NN")
        return Direction::NN;

    if (j == "SS")
        return Direction::SS;

    if (j == "EE")
        return Direction::EE;

    if (j == "WW")
        return Direction::WW;

    return nullptr;
}

/////////////////////////////////
/////////// MAP PART ////////////
/////////////////////////////////

// MapPart constructors
MapPart::MapPart(Rect xy) : xy(xy) {}
MapPart::MapPart(Rect xy, std::vector<Direction *> available_directions) : 
    xy(xy), available_directions(available_directions)
{
}

/////////////////////////////////
///////////// ROOM //////////////
/////////////////////////////////

// Room constructors
Room::Room(Rect xy) : MapPart(xy) {}
Room::Room(Rect xy, std::vector<Direction *> available_directions) : 
    MapPart(xy, available_directions) {}

/////////////////////////////////
/////////// CORRIDOR ////////////
/////////////////////////////////

Corridor::Corridor(Rect xy, std::vector<Direction *> available_directions, bool horizontal) : 
    MapPart(xy, available_directions), _horizontal(horizontal) {}

/////////////////////////////////
/////////// GAME MAP ////////////
/////////////////////////////////

GameMap::GameMap(int width, int height, bool _initialize_tiles) : 
    width(width), height(height)
{

    fov_map = nullptr;
    aux_fov_map_100 = nullptr;

    // Decide whether to initialize tiles or not
    if (_initialize_tiles)
        initialize_tiles();

    // TODO implement
}

Entity * GameMap::get_inspectable_entity_at(int x, int y)
{
    for (int i=0; i<(int)_entities.size(); i++)
    {
        if (_entities[i]->x == x && _entities[i]->y == y // check position
            && _entities[i]->render_order() != NONE // check that it is a physical entity
            && _entities[i]->render_order() != SFX) // check that it is a physical entity
            return _entities[i];
    }

    return nullptr;
}

Tile * GameMap::get_tile_at(int x, int y)
{

    int i = compute_tile_index(x, y, width);

    return tiles[i];
}


Entity * GameMap::get_item_at(int x, int y)
{
    for (int i=0; i<(int)_entities.size(); i++)
    {
        if (_entities[i]->x == x && _entities[i]->y == y && // check position
            _entities[i]->item != nullptr) // check that it is actually an item
            return _entities[i];
    }

    return nullptr;
}


void GameMap::get_player_starting_coords(int & x, int & y)
{

    // TODO change this
    for (int i=0; i<(int)entities().size(); i++)
    {
        if (entities()[i]->tag.find("entrypoint") != std::string::npos)
        {
            x = entities()[i]->x;
            y = entities()[i]->y;

            return;
        }
    }

    // TODO
    // Raise exception if no starting point is found
}

void GameMap::place_player(Entity * player, int x, int y)
{

    // Set player's coordinates
    player->x = x;
    player->y = y;

    // Add player to list of entities
    add_entity(player);
}

void GameMap::place_player(Entity * player)
{

    int x, y;
    
    // Get starting coordinates from the map object itself
    get_player_starting_coords(x, y);

    place_player(player, x, y);
}

void GameMap::initialize_fov_map()
{

    fov_map = new TCODMap(width, height);
    aux_fov_map_100 = new TCODMap(width, height);
    fov_map_noentities = new TCODMap(width, height);

    int tile_index;

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            // Compute tile index
            tile_index = compute_tile_index(x, y, width);

            // Set tile properties
            // blocks and blocks_sight must be negated, as the fov properties
            // correspond to their opposites (`walkable` and `transparent`
            // respectively)
            fov_map->setProperties(
                x, y,
                !(tiles[tile_index])->block_sight(),
                !(tiles[tile_index])->blocked());

        }
    }

    // Copy properties for fov map without entities
    fov_map_noentities->copy(fov_map);

    for (int i=0; i<(int)_entities.size(); i++)
    {

        int x =_entities[i]->x;
        int y =_entities[i]->y;

        bool blocks = !fov_map->isWalkable(x, y) || _entities[i]->blocks();
        bool blocks_sight = !fov_map->isTransparent(x, y) || _entities[i]->blocks_sight();

        fov_map->setProperties(x, y, !blocks_sight, !blocks);
    }

    // Copy properties for auxiliary fov map
    aux_fov_map_100->copy(fov_map);

    // Initialize path variable
    path_astar = new TCODPath(fov_map, 0.0f);
    //path_astar = new TCODPath(fov_map_noentities, 0.0f);
}

void GameMap::update_fov_map_properties(int x, int y, bool transparent, bool walkable)
{
    // Update main map (for player vision)
    fov_map->setProperties(
        x, y, transparent, walkable);

    // Update auxiliary map
    aux_fov_map_100->setProperties(
        x, y, transparent, walkable);

}

void GameMap::recompute_fov(Entity * player)
{
    
    // Recompute main FOV map
    fov_map->computeFov(
        player->x, player->y, FOV_RADIUS,
        FOV_LIGHT_WALLS, FOV_ALGORITHM);

    aux_fov_map_100->computeFov(
        player->x, player->y, 100,
        FOV_LIGHT_WALLS, FOV_ALGORITHM);
}


void GameMap::initialize_tiles()
{
    // Initialize array of Tile * of size width x height
    tiles = (Tile **)malloc(width * height * sizeof(Tile *));

    /*
    GRAY_PALETTE = [
    ]
    */

    std::vector<TCODColor> gray_palette = std::vector<TCODColor>();
    gray_palette.push_back(TCODColor(242, 242, 242));
    gray_palette.push_back(TCODColor(204, 204, 204));
    gray_palette.push_back(TCODColor(165, 165, 165));
    gray_palette.push_back(TCODColor(127, 127, 127));
    gray_palette.push_back(TCODColor(89, 89, 89));

    for (int x=0; x<height; x++)
    {
        for (int y=0; y<width; y++)
        {

            // Fill tiles with fake base tiles, allocating memory
            //tiles[x*width + y] = new Tile(true, true);

            // level.tiles[X][Y] = Wall.create_from_palette()
            tiles[x*width + y] = Wall::create_from_palette(gray_palette);
        }
    }
}

std::vector<Entity *> GameMap::entities()
{

    return _entities;
}

bool GameMap::is_blocked(int x, int y) const
{
    //int tile_index = compute_tile_index(x, y, width);
    //return tiles[tile_index]->blocked();

    return tiles[
        compute_tile_index(x, y, width)
    ]->blocked();
}


void GameMap::add_part(Corridor * corridor)
{
    corridors.push_back(corridor);
    dig(corridor);
}

void GameMap::add_part(Room * room)
{
    rooms.push_back(room);
    dig(room);
}

void GameMap::change_tile_symbol(int x, int y, int symbol)
{
    int i = compute_tile_index(x, y, width);

    // Set new symbol
    tiles[i]->_fg_symbol = symbol;
}

void GameMap::make_floor(int x, int y)
{

    int i = compute_tile_index(x, y, width);

    // Remove old tile
    delete tiles[i];

    tiles[i] = new Floor();
}

void GameMap::make_wall(int x, int y)
{

    int i = compute_tile_index(x, y, width);

    // Remove old tile
    delete tiles[i];

    tiles[i] = new Wall();
}

void GameMap::make_cover(int x, int y)
{

    int i = compute_tile_index(x, y, width);

    // Remove old tile
    delete tiles[i];

    tiles[i] = new Cover();
}

void GameMap::make_window(int x, int y)
{

    int i = compute_tile_index(x, y, width);

    // Remove old tile
    delete tiles[i];

    tiles[i] = new Window();
}

void GameMap::dig(MapPart * map_part, int padding)
{

    Rect padded_rect(
        map_part->xy.x1 + padding,
        map_part->xy.y1 + padding,
        map_part->xy.x2 - padding,
        map_part->xy.y2 - padding);

    dig_rect(this, padded_rect);

    // TODO Do more than this
}

json GameMap::to_json()
{
    json j_data;
    json j_tiles;
    json j_entities;

    for (int i=0; i<width*height; i++)
    {
        j_tiles.push_back(tiles[i]->to_json());
    }

    //DEBUG("Saving entities");
    //DEBUG("N of entities: " << entities().size());

    // Save entities (assumes player has been removed)
    //for (auto it = entities().begin(); it != entities().end(); ++it)
    //{
        //DEBUG("Saving entity: " << (*it)->name);

        //j_entities.push_back((*it)->to_json());
    //}

    for (int i=0; i< (int)_entities.size(); i++)
    {
        //DEBUG("Saving entity: " << _entities[i]->name);
        j_entities.push_back(_entities[i]->to_json());
    }

    //DEBUG("After");

    j_data["dungeon_level"] = dungeon_level;
    j_data["width"] = width;
    j_data["height"] = height;
    j_data["tiles"] = j_tiles;
    j_data["entities"] = j_entities;

    return j_data;
}

GameMap * GameMap::from_json(json j)
{

    //DEBUG("Reconstructing game_map");

    //std::cout << "Checkpoint 1" << std::endl;

    // Create object but do not initialize tiles
    GameMap * game_map = new GameMap(j["width"], j["height"], false);

    //std::cout << "Checkpoint 2" << std::endl;

    game_map->dungeon_level = j["dungeon_level"];

    //std::cout << "Checkpoint 3" << std::endl;

    // Initialize array of Tile * of size width x height
    game_map->tiles = (Tile **)malloc(
        game_map->width * game_map->height * sizeof(Tile *));

    //std::cout << "Checkpoint 4" << std::endl;

    // Restore tiles from json data
    for (int i=0; i<(game_map->width * game_map->height); i++)
    {
        game_map->tiles[i] = Tile::from_json(j["tiles"][i]);
    }
    
    //std::cout << "Checkpoint 5" << std::endl;
   
    // Restore list of entities
    for (json::iterator it = j["entities"].begin(); it != j["entities"].end(); ++it)
    {
        game_map->add_entity(Entity::from_json(*it));
    }

    return game_map;
}

/*


def add_walls(level):
    """
    Creates walls in all Tile-type tiles adjacent to something non-Tile
    """

    for X in range(level.width):
        for Y in range(level.height):
            for x in range(max(X-1, 0), min(X+2, level.width)):
                for y in range(max(Y-1, 0), min(Y+2, level.height)):
                    if (type(level.tiles[X][Y]) == Tile and \
                       (type(level.tiles[x][y]) not in [Tile, Wall])):
                       ## Create wall
                       level.tiles[X][Y] = Wall.create_from_palette()


*/


int GameMap::search_target_in_range(int base_x, int base_y, int range, Entity ** target)
{

    std::vector<Entity *> enemies_in_range;

    // TODO expand for multiple options depending on player equipment
    // Look for targets in melee range
    for (int x = base_x - range; x <=base_x + range; x++)
    {
        for (int y = base_y - range; y <=base_y + range; y++)
        {
            // First check if it is a visible tile, to avoid to target stuff
            // over walls with greater melee range
            if (fov_map->isInFov(x, y))
            {
                Entity * target = get_blocking_entities_at_location(
                    entities(), x, y);

                // Check if it is actually a monster
                if (target != nullptr && target->ai != nullptr && target->fighter != nullptr)
                {
                    enemies_in_range.push_back(target);
                }
            }
        }
    }

    int n_enemies_in_range = enemies_in_range.size();

    // If there is only one enemy in range return it
    if (n_enemies_in_range == 1)
    {
        (*target) = enemies_in_range[0];
    }

    return n_enemies_in_range;


}

// Compares two entities according to render_order
bool compare_render_order(Entity * e1, Entity * e2)
{
    //std::cout << "here!" << std::endl;
    return (e1->render_order() < e2->render_order());
}

void GameMap::add_entity(Entity * entity)
{
    _entities.push_back(entity);

    // Sort entities based on render order when a new one is added
    std::sort(_entities.begin(), _entities.end(), compare_render_order);
    
    //std::cout << "sorting" << std::endl;
    //for (int i=0;i<(int)_entities.size();i++)
    //{
        //std::cout << i << " " << _entities[i]->name << std::endl;
    //}
}

void GameMap::remove_entity(Entity * entity)
{

    // Solution taken from
    // https://stackoverflow.com/questions/3385229/c-erase-vector-element-by-value-rather-than-by-position

    _entities.erase(
        std::remove(_entities.begin(), _entities.end(), entity),
        _entities.end());
}
