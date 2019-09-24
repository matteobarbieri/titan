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

        BuffStun(Entity * target, int duration);

        virtual bool disables_entity();

};


#endif
