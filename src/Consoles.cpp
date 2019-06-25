#include "Constants.h"
#include "Consoles.hpp"

#include "libtcod.hpp"

/*! The static method to access the singleton */
Consoles & Consoles::singleton()
{
    static Consoles instance;

    return instance;
}

/*! Private constructor */
Consoles::Consoles()
{
    /*! Initialize pointers for consoles */
    terrain_layer = new TCODConsole(
        TERRAIN_LAYER_WIDTH, TERRAIN_LAYER_HEIGHT);
    main_window = new TCODConsole(
        TERRAIN_LAYER_WIDTH, TERRAIN_LAYER_HEIGHT);
    menu = new TCODConsole(
        TERRAIN_LAYER_WIDTH, TERRAIN_LAYER_HEIGHT);
    submenu = new TCODConsole(
        TERRAIN_LAYER_WIDTH, TERRAIN_LAYER_HEIGHT);

}

