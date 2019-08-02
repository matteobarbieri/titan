#ifndef R20177_UID
#define R20177_UID

#include "Constants.h"

/** A class used to generate unique ids
 * one, which is accessed directly via TCODConsole::root.
 */
class Uid
{
    public:

        unsigned long int last_id;

        /**
         * Increment the last available ID by one, then return it.
         */
        unsigned long int next_id();

        /*! The static method to access the singleton */
        static Uid & singleton();

    private:
        Uid();
        ~Uid();

};

#endif
