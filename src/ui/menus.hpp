#ifndef R20177_MENUS
#define R20177_MENUS

#include "../libtcod.hpp"

#include <vector>
#include <string>

// Forward declarations
class Entity;

typedef struct {
    char letter;
    const char * text;
} MenuOption;

void main_menu(TCODImage * background_image);

void menu(TCODConsole * con, std::vector<MenuOption> options, const char * header,
          int width,
          int x, int y, TCODColor header_fg);

/**
 * Draws stuff on a console to represent the interface to a Terminal in the game
 */
void terminal_menu(Entity *);

/**
 * Draws stuff on the console to display a container interface
 */
void container_menu(Entity *);

void inventory_menu(
    Entity * player, std::string header="Inventory");

void inventory_item_submenu(Entity * player, Entity * item);

void container_item_submenu(Entity * item, bool=true);

#endif /* ifndef R20177_MENUS */
