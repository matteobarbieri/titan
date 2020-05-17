#ifndef R20177_BUFF
#define R20177_BUFF

#include "../nlohmann/json.hpp"

#include "../libtcod.hpp"

using json = nlohmann::json;

// Forward declarations
class Entity;
class GameMap;

/**
 * A class that represents a [de]buff which is applied to an entity/region of
 * space.
 */
class Buff
{
    public:

        //////////////////////////////
        ///////// PROPERTIES /////////
        //////////////////////////////

        /**
         * The duration of the buff.
         */
        int duration;

        /**
         * The entity to which the buff has been attached.
         */
        Entity * target;

        //////////////////////////////
        ////////// METHODS ///////////
        //////////////////////////////

        /**
         * Base constructor, sets duration
         */
        //Buff(Entity * target, int duration);
        Buff(int duration);

        /**
         * Returns true if the buff has expired
         */
        bool has_expired();

        /**
         *
         */
        virtual bool disables_entity();

        virtual Buff * clone() = 0;

        virtual void tick();

        virtual void expire(GameMap *);

        void apply(Entity *);

        virtual void _apply(Entity *);

        virtual json to_json() = 0;

        static Buff * from_json(json j);
};

class DelayedMessageBuff : public Buff
{

    public:

        std::string text;
        TCODColor color;

        DelayedMessageBuff(int duration, std::string text, TCODColor color);

        DelayedMessageBuff * clone();

        void expire(GameMap *);

        //void _apply(Entity *);

        json to_json();
        static DelayedMessageBuff * from_json(json j);
};

/**
 * Removes completely an entity when buff expires
 */
class DelayedRemoveBuff : public Buff
{

    public:

        GameMap * game_map;

        DelayedRemoveBuff(int duration);

        DelayedRemoveBuff * clone();

        void expire(GameMap *);

        //void _apply(Entity *);

        json to_json();
        static DelayedRemoveBuff * from_json(json j);
};

#endif
