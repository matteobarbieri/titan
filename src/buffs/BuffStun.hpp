#ifndef R20177_BUFF_STUN
#define R20177_BUFF_STUN

#include "Buff.hpp"

/**
 * A class that represents a [de]buff which is applied to an entity/region of
 * space.
 */
class BuffStun : public Buff
{
    public:

        //////////////////////////////
        ///////// PROPERTIES /////////
        //////////////////////////////

        //////////////////////////////
        ////////// METHODS ///////////
        //////////////////////////////

        //BuffStun(Entity * target, int duration);
        BuffStun(int duration);

        BuffStun * clone();

        void _apply(Entity *);
        virtual bool disables_entity();

        json to_json();
        static BuffStun * from_json(json j);
};


#endif
