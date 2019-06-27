#ifndef R20177_CONSOLES
#define R20177_CONSOLES

#include "Constants.h"

#include "libtcod.hpp"

/** A container for all consoles that are used in the game, except for the root
 * one, which is accessed directly via TCODConsole::root.
 */
class Entity
{
    public:

        Entity();
        ~Entity();

};

#endif
