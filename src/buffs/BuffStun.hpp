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

        // Whether it adds a message in the log to notify that the entity is
        // stunned.
        bool add_log_message;

        //////////////////////////////
        ////////// METHODS ///////////
        //////////////////////////////

        BuffStun(int duration, bool);
        BuffStun(int duration);

        BuffStun * clone();

        void _apply();
        virtual bool disables_entity();

        json to_json();
        static BuffStun * from_json(json j);
};


#endif
