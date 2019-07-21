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

void inventory_menu(
    Entity * player, std::string header="Inventory");

#endif /* ifndef R20177_MENUS */
