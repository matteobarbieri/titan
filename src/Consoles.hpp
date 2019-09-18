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

        TCODConsole * terrain_layer;
        TCODConsole * main_window;
        TCODConsole * menu;
        TCODConsole * submenu;
        TCODConsole * panel;
        TCODConsole * entity_frame;
        TCODConsole * inventory_frame;
        TCODConsole * popup_message;

        /*! The static method to access the singleton */
        static Consoles & singleton();

    private:
        Consoles();
        ~Consoles();

};

#endif
