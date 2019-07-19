#ifndef R20177_CONSOLES
#define R20177_CONSOLES

#include "Constants.h"

#include "libtcod.hpp"

/** A container for all consoles that are used in the game, except for the root
 * one, which is accessed directly via TCODConsole::root.
 */
class Consoles
{
    public:

        TCODConsole * terrain_layer = NULL;
        TCODConsole * main_window = NULL ;
        TCODConsole * menu = NULL ;
        TCODConsole * submenu = NULL ;
        TCODConsole * panel = NULL ;
        TCODConsole * entity_frame = NULL ;
        TCODConsole * inventory_frame = NULL ;

        /*! The static method to access the singleton */
        static Consoles & singleton();

    private:
        Consoles();
        ~Consoles();

};

#endif
