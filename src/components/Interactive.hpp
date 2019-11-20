#ifndef ROGUE_20177_INTERACTIVE
#define ROGUE_20177_INTERACTIVE

#include <functional>

#include "../nlohmann/json.hpp"

#include "../libtcod.hpp"

using json = nlohmann::json;

// Forward declarations
//struct MenuOption;

class Entity;

class GameMap;
class GameState;

class Direction;


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
        
        virtual void interact(Entity *, GameMap *, GameState *) = 0;
};

class InteractiveDoor : public Interactive
{
    public:

        // Whether the door is open or closed
        bool is_open;

        // Whether the door is locked or unlocked
        bool locked;

        // The ID of the key which will open the door
        unsigned int key_id;

        InteractiveDoor(bool is_open=false, bool locked=false, unsigned int key_id=0);

        /**
         * Set the door as unlocked, changing its symbol and updating the FOV
         * map accordingly.
         */
        //void unlock(GameMap * game_map);
        void unlock();

        void open(GameMap * game_map);

        /**
         * Checks if the player has in its inventory an appropriate key to open
         * the door.
         */
        bool player_has_key(Entity * player);

        virtual void interact(Entity *, GameMap *, GameState *);
};

class InteractivePanel : public Interactive
{
    public:

        // The text shown in the log
        std::string text;

        // The color of the text shown in the log
        TCODColor text_color;

        // The direction from where the panel is readable
        Direction * readable_from;

        // Whether the panel is active or not
        bool is_active;

        InteractivePanel(std::string text, TCODColor text_color, Direction * readable_from, bool is_active);

        virtual void interact(Entity *, GameMap *, GameState *);
};

/**
 * A single function offered by a terminal
 */
class TerminalFunction
{
    public:

        // The name of the command, which will show up on the terminal.
        std::string command;

        // The shortcut used to launch that function. Will be a char most of the
        // times.
        int command_shortcut;

        // The pointer to the actual function which will be executed.
        void (*execute)(Entity *, GameMap *, GameState *);

        // Whether this specific option is currently enabled or not
        bool enabled;

        TerminalFunction(std::string, int, void (*)(Entity *, GameMap *, GameState *), bool enabled=true);
};

class InteractiveTerminal : public Interactive
{
    public:

        // The text shown in the log
        std::string text;

        // The color of the text shown in the log
        TCODColor text_color;

        std::vector<TerminalFunction> terminal_functions;

        // Whether the terminal is active or not
        bool is_active;

        InteractiveTerminal(bool is_active);

        // What happens when a player interacts with the terminal
        virtual void interact(Entity *, GameMap *, GameState *);
};


#endif /* ifndef ROGUE_20177_INTERACTIVE */
