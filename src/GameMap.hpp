#ifndef R20177_GAME_MAP
#define R20177_GAME_MAP

#include "libtcod.hpp"

// Forward declaration
//class Entity;
#include "Entity.hpp"

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

class Door : public MapPart
{
    public:

        Door(int x, int y, bool is_open);

        // TODO check if needed
        //void dig(GameMap game_map);
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

        Junction(Rect xy, std::vector<Direction> available_directions);

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
            std::vector<Direction> available_directions, bool horizontal);

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

        // Properties
        
        int width;
        int height;

        int top_x;
        int top_y;

        int dungeon_level;

        // List of map parts
        std::vector<Room *> rooms;
        
        // Methods
        
        GameMap(int w, int h);
        ~GameMap();

        void get_player_starting_coords(int & x, int & y);

        Entity * get_item_at(int x, int y);

        void place_player(Entity * player);

        void dig(MapPart * part, int pad=0);

        void create_dijkstra_map(MapPart * part);

        void add_part(Corridor *);
        void add_part(Junction *);
        void add_part(Room *);
        void add_part(Door *);

        void add_walls();

        bool can_place(MapPart part);

        void initialize_tiles();

        bool is_blocked(int x, int y);

        void export_txt(const char * txt_file);

        void export_shelf(const char * destination);

        // TODO check return type
        std::vector<MapPart *> all_parts();

        std::vector<Entity *> entities();
};

#endif
