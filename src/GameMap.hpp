#ifndef R20177_GAME_MAP
#define R20177_GAME_MAP

#include "Constants.h"

#include "libtcod.hpp"

// Forward declaration
class Entity;

#include <vector>

class Rect
{
};

// TODO consider moving this to a separate file
class DijkstraMap
{
};

// TODO consider moving this to a separate file
class Direction
{
};

class MapPart
{

    public:

        // Attributes
        DijkstraMap d_map;

        
        // Constructor
        MapPart(Rect xy, std::vector<Direction> available_directions);

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

        Room(Rect xy, std::vector<Direction> available_directions);

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
        
        // Methods
        
        GameMap(int w, int h);
        ~GameMap();

        void get_player_starting_coords(int & x, int & y);

        Entity * get_item_at(int x, int y);

        void place_player(Entity * player);

        void dig(MapPart part, int pad=0);

        void create_dijkstra_map(MapPart * part);

        void add_part(Corridor *);
        void add_part(Junction *);
        void add_part(Room *);
        void add_part(Door *);

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
