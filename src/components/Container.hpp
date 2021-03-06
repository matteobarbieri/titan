#ifndef ROGUE_20177_CONTAINER
#define ROGUE_20177_CONTAINER

#include <vector>

#include "../nlohmann/json.hpp"

using json = nlohmann::json;

// Forward declarations
class Entity;
class Inventory;
//class GameMap;

class Container
{

    private:

        /**
         * Cleanup function
         */
        void remove_item(Entity * item);

    public:

        Entity * owner;

        std::vector<Entity *> items;

        /////////////////////////////
        ////////// METHODS //////////
        /////////////////////////////
        
        Container();

        /**
         * Retrieve an item from the container
         */
        void get(Entity * item);

        /**
         * Put an item in the container
         */
        void put(Entity * item);

        /**
         * Return the position of the item in the inventory's items list
         */
        //int get_item_position_in_list(Entity * item);
        //
        
        void refresh_items_letters(Inventory *);

        /**
         * Creates a json representation of the component
         */
        json to_json();

        static Container * from_json(json j);
};


#endif /* ifndef ROGUE_20177_CONTAINER */
