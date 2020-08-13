#ifndef R20177_ENTITY
#define R20177_ENTITY

#include <string>

#include "Constants.h"

#include "RenderOrder.hpp"

#include "libtcod.hpp"

#include "nlohmann/json.hpp"

using json = nlohmann::json;

// Forward declarations
class Fighter;
class MonsterAi;
class Stairs;
class Item;
class Level;
class Equipment;
class Equippable;
class Usable;
class Inventory;
class Interactive;
class Container;

class GameMap;
class GameState;

class Buff;

/** A container for all consoles that are used in the game, except for the root
 * one, which is accessed directly via TCODConsole::root.
 */
class Entity
{

    private:

        // The color of the entity's symbol
        TCODColor _color;
        TCODColor _bg_color;

        // Is it a blocking entity?
        bool _blocks;

        // Does it blocks sight?
        bool _blocks_sight;

        // Is it a fixed entity (e.g. doors, stairs...)
        bool _fixed;

        // In which order the entity is rendered
        RenderOrder _render_order;

    public:

        // Unique id
        unsigned long int _id;

        // Entity coordinates in the game map
        int x;
        int y;

        // The entity's name
        std::string name;

        // The character representing the entity on the map
        int symbol;

        // A tag used to identify entity type
        std::string tag;

        // Unique id
        unsigned long int group_id;

        // Components
        Fighter * fighter;
        MonsterAi * ai;
        Stairs * stairs;
        Item * item;
        Level * level;
        Equipment * equipment;
        Equippable * equippable;
        Usable * usable;
        Inventory * inventory;
        Interactive * interactive;
        Container * container;

        // Buffs currently applied to entity
        std::vector<Buff *> buffs;

        //////////////////////////////
        ////////// METHODS ///////////
        //////////////////////////////
        
        // Constructor
        Entity(int x, int y, int symbol,
               TCODColor color, std::string name,
               RenderOrder render_order=CORPSE,
               bool blocks=false, bool blocks_sight=false,
               bool _fixed=false,
               unsigned long int id=0,
               unsigned long int group_id=0);

        ~Entity();

        // Required for sorting
        //bool operator < (const Entity & other) const;

        // Getters
        RenderOrder render_order() const;
        TCODColor color() const;
        bool fixed() const;
        bool blocks() const;
        bool blocks_sight() const;

        // Setters
        void render_order(RenderOrder);
        void color(TCODColor);
        void fixed(bool);
        void blocks(bool);
        void blocks_sight(bool);

        /*
         * Default interaction with another entity
         *
         * player <-> monster: attack
         * player  -> door: interact
         */
        void interact_with(Entity * other, GameMap * game_map, GameState * GameState);

        void apply_buff(Buff *);

        /**
         * Returns true if for some reason (possibly depending on buffs) the
         * entity is disabled and cannot take an action.
         */
        bool is_disabled() const;

        /**
         * Return true if the entity is a valid target, that is has the
         * following properties:
         * - Is a fighter (i.e. has the fighter component)
         * - Is hostile to the player TODO
         */
        bool is_valid_target() const;

        /**
         * Used for entities which represent living things, cleans up components
         */
        void die();

        /**
         * Creates a json representation of the entity
         */
        json to_json();

        static Entity * from_json(json j);

};

#endif
