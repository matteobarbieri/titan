#ifndef ROGUE_20177_INTERACTIVE
#define ROGUE_20177_INTERACTIVE

#include "../nlohmann/json.hpp"

using json = nlohmann::json;

// Forward declarations
//struct MenuOption;

class Entity;

class GameMap;


/**
 * The item class represents items and their properties "while they are in the
 * backpack". Meaning that for instance the info about their properties as items
 * that can be equipped must be stored in an instance of the  `Equippable` class.
 */
class Interactive
{

    private:

    public:

        Entity * owner;

        // TODO to enable again
        //std::vector<MenuOption> item_inventory_options();

        /**
         * Creates a json representation of the component
         */
        //json to_json();
        //static Interactive * from_json(json j);
        
        virtual void interact(Entity *, GameMap *) = 0;
};

class InteractiveDoor : public Interactive
{
    public:

        virtual void interact(Entity *, GameMap *);
};

#endif /* ifndef ROGUE_20177_INTERACTIVE */
