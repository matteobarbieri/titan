#include "Constants.h"
#include "Consoles.hpp"

#include "libtcod.hpp"

/*! The static method to access the singleton */
Consoles & Consoles::singleton()
{
    static Consoles instance;

    return instance;
}

Consoles::~Consoles()
{
    std::cout << "Destructing consoles" << std::endl;
}
/*! Private constructor */
Consoles::Consoles()
{
    /*! Initialize pointers for consoles */
    
    /*! The main console, where terrain is displayed */
    terrain_layer = new TCODConsole(
        TERRAIN_LAYER_WIDTH, TERRAIN_LAYER_HEIGHT);

    /*! The main window */
    main_window = new TCODConsole(
        TERRAIN_LAYER_WIDTH, TERRAIN_LAYER_HEIGHT);

    /*! The auxiliary console for the list menu */
    menu = new TCODConsole(
        TERRAIN_LAYER_WIDTH, TERRAIN_LAYER_HEIGHT);

    /*! The auxiliary console for the submenu */
    submenu = new TCODConsole(
        TERRAIN_LAYER_WIDTH, TERRAIN_LAYER_HEIGHT);

    /*! The panel below the main console, containig HP and other info */
    panel = new TCODConsole(
        PANEL_WIDTH, PANEL_HEIGHT);

    /*! The console for entity details */
    entity_frame = new TCODConsole(
        FRAME_WIDTH, FRAME_HEIGHT);

    /*! The console for inventory and equipment */
    inventory_frame = new TCODConsole(
        FRAME_WIDTH, FRAME_HEIGHT);

    /*! The console for popup messages */
    popup_message = new TCODConsole(
        SCREEN_WIDTH, SCREEN_HEIGHT);
}

