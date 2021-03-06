#ifndef R20177_GAME_MAP
#define R20177_GAME_MAP

#include "../libtcod.hpp"

//#include "Entity.hpp"

#include "../nlohmann/json.hpp"

using json = nlohmann::json;

// Forward declaration
class Entity;
class Tile;

#include <vector>

class Rect
{
    public:
        int x1, y1, x2, y2;

        /**
         * Standard constructor which stores 4 integer values (two for each
         * coordinate pair).
         *
         */
        Rect(int x1, int y1, int x2, int y2);
};

// TODO consider moving this to a separate file
class DijkstraMap
{
};

// TODO consider moving this to a separate file
class Direction
{
    public:

        const int dx;
        const int dy;

        Direction(int dx, int dy);

        // Four cardinal directions
        static Direction * NN;
        static Direction * SS;
        static Direction * EE;
        static Direction * WW;

        /**
         * Return the list of four cardinal directions: North, South, West and
         * East
         */
        static std::vector<Direction *> FourD();

        std::string to_json();

        static Direction * from_json(std::string j);
};

class MapPart
{

    public:
        
        // Attributes
        Rect xy;
        std::vector<Direction *> available_directions;

        DijkstraMap d_map;
        
        // The list of other parts of the map this one is connected to
        // (possibly useful later for pathfinding etc.)
        // TODO to implement
        //self.connected_parts = list()

        // Constructor
        MapPart(Rect xy, std::vector<Direction *> available_directions);

        MapPart(Rect xy);

        bool has_tile(int x, int y);

        bool is_connected_to(MapPart other);

        void remove_connection(MapPart other);

        void connect_to(MapPart other);

        float distance_from(MapPart other);

        void reset_available_directions();

        bool intersects_with(MapPart other);


        bool has_available_directions();

        // TODO check parameters, probably requires reference passing
        void pick_starting_point();
        
        int height();

        int width();

        // TODO check parameters
        void center(int & xc, int & yc);


};

class Room : public MapPart
{

    public:

        // Properties

        // Methods
        Room(Rect xy, std::vector<Direction *> available_directions);
        Room(Rect xy);

        // TODO check if needed
        //void dig(GameMap game_map);
};

class Junction : public MapPart
{
    public:

        Junction(Rect xy, std::vector<Direction *> available_directions);

        // TODO check if needed
        //void dig(GameMap game_map);
};

class Corridor : public MapPart
{

    private:
        bool _horizontal;

    public:

        Corridor(
            Rect xy,
            std::vector<Direction *> available_directions, bool horizontal);

        // TODO check if needed
        //void dig(GameMap game_map);
        
        bool vertical();

        bool horizontal();

        void pick_room_starting_point(int & x, int & y, Direction & d);
};



/**
 * A map level
 */
class GameMap
{
    private:

        std::vector<Entity *> _entities;

    public:

        /////////////////////////////////
        ////////// ATTRIBUTES ///////////
        /////////////////////////////////
        
        int width;
        int height;

        int top_x;
        int top_y;

        int dungeon_level;

        // The main FOV map, used for vision
        TCODMap * fov_map;

        // A series of auxiliary FOV maps, used to determine other entities's
        // vision range and other things
        TCODMap * aux_fov_map_100;
        TCODMap * fov_map_noentities;

        TCODPath * path_astar;

        // Lists of map parts
        std::vector<Room *> rooms;
        std::vector<Corridor *> corridors;
        std::vector<Junction *> junctions;
        
        // The tiles
        Tile ** tiles;

        /////////////////////////////////
        //////////// METHODS ////////////
        /////////////////////////////////
        
        GameMap(int width, int height, bool _initialize_tiles=true);
        ~GameMap();

        void get_player_starting_coords(int & x, int & y);

        /**
         * Return an entity that can be inspected at given position.
         * Entity must be visible.
         */
        Entity * get_inspectable_entity_at(int x, int y);

        /**
         * If there is an item on the floor at coordinates x, y return it,
         * else return nullptr.
         */
        Entity * get_item_at(int x, int y);

        /**
         * Return a tile at a given position in the map
         */
        Tile * get_tile_at(int, int);

        /**
         * Places the player at the dungeon's entry point
         */
        void place_player(Entity * player);

        /**
         * Places player at specified coordinates
         */
        void place_player(Entity * player, int, int);

        void dig(MapPart * part, int pad=0);

        void recompute_fov(Entity * player);
        void update_fov_map_properties(int x, int y, bool transparent, bool walkable);

        /**
         * Turn a single tile to floor
         */
        void change_tile_symbol(int x, int y, int symbol);

        /**
         * Turn a single tile to floor
         */
        void make_floor(int x, int y);

        /**
         * Turn a single tile to wall
         */
        void make_wall(int x, int y);

        /**
         * Turn a single tile to partial cover
         */
        void make_cover(int x, int y);

        /**
         * Turn a single tile to a window, that is an impassable cell which
         * however allows to see through.
         */
        void make_window(int x, int y);

        void create_dijkstra_map(MapPart * part);
        void initialize_fov_map();

        void add_part(Corridor *);
        void add_part(Junction *);
        void add_part(Room *);

        //void add_walls();

        // Add or remove an entity from the level
        void add_entity(Entity *);
        void remove_entity(Entity *);

        bool can_place(MapPart part);

        void initialize_tiles();

        bool is_blocked(int x, int y) const;

        /**
         * Given a position and a range, return the number of entities in that 
         * range (normally monsters).
         */
        int search_target_in_range(int x, int y, int range, Entity ** target);

        void export_txt(const char * txt_file);

        void export_shelf(const char * destination);

        json to_json();
        static GameMap * from_json(json j);

        // TODO check return type
        std::vector<MapPart *> all_parts();

        std::vector<Entity *> entities();
};

#endif
