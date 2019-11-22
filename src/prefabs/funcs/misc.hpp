#ifndef ROGUE_20177_PREFABS_FUNCS_MISC
#define ROGUE_20177_PREFABS_FUNCS_MISC

#include <functional>

// Forward declaration
class Entity;
class GameMap;
class GameState;

#include "../../nlohmann/json.hpp"

#include "../../libtcod.hpp"

using json = nlohmann::json;

void unlock_doors(Entity *, GameMap *, GameState *, unsigned int);

/**
 * Represents some kind of effect on the map/player/game state.
 * Should be usable 
 */
class Effect
{
    public:

        /**
         * Apply the effect specified
         */
        virtual void apply(Entity *, GameMap *, GameState *) = 0;

        virtual json to_json() = 0;

        static Effect * from_json(json);
};

/**
 * Open all doors having a key_id equal to the one specified.
 */
class UnlockDoorsEffect : public Effect
{

    public:

        unsigned int key_id;
        
        UnlockDoorsEffect(unsigned int);

        void apply(Entity *, GameMap *, GameState *);

        json to_json();

        static UnlockDoorsEffect * from_json(json);
};


/**
 * Open all doors having a key_id equal to the one specified.
 */
class AddLogMessageEffect : public Effect
{

    public:

        // The text to be displayed
        std::string text;

        // The color of the text
        TCODColor text_color;

        AddLogMessageEffect(std::string, TCODColor = TCODColor::white);

        void apply(Entity *, GameMap *, GameState *);

        json to_json();

        static AddLogMessageEffect * from_json(json);
};

#endif /* ifndef ROGUE_20177_PREFABS_FUNCS_MISC */
