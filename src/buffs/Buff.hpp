#ifndef R20177_BUFF
#define R20177_BUFF

class Entity;

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
        Buff(Entity * target, int duration);

        /**
         * Returns true if the buff has expired
         */
        bool has_expired();

        /**
         *
         */
        virtual bool disables_entity() = 0;

        virtual void tick();

};

#endif
