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

        /**
         * Determines whether the buff must be removed once the entity to which
         * it is attached dies/is destroyed. In some cases (explosives attached
         * to a monster) it might make sense to keep the buff there.
         */
        bool stays_on_death;

        //////////////////////////////
        ////////// METHODS ///////////
        //////////////////////////////

        /**
         * Base constructor, sets duration
         */
        //Buff(Entity * target, int duration);
        Buff(int duration);

        virtual ~Buff();

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

        virtual void _apply();

        virtual json to_json() = 0;

        static Buff * from_json(json j);
};

/**
 * Adds a message to the message log after a given number of turns
 */
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
 * Temporarily change attributes of an entity (color, name, etc.)
 */
class TempChangeEntityBuff : public Buff
{

    public:

        std::string new_name;
        TCODColor new_color;

        std::string old_name;
        TCODColor old_color;

        TempChangeEntityBuff(int duration, std::string new_name, TCODColor new_color);
        TempChangeEntityBuff(
                int duration,
                std::string new_name, TCODColor new_color,
                std::string old_name, TCODColor old_color);

        TempChangeEntityBuff * clone();

        void expire(GameMap *);

        void _apply();

        json to_json();
        static TempChangeEntityBuff * from_json(json j);
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
