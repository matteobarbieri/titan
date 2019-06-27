#include "libtcod.hpp"

#include <vector>

#ifndef R20177_MENUS
#define R20177_MENUS

typedef struct {
    char letter;
    const char * text;
} MenuOption;

void main_menu(TCODImage * background_image);

void menu(TCODConsole * con, std::vector<MenuOption> options, const char * header,
          int width,
          int x, int y, TCODColor header_fg);

#endif /* ifndef R20177_MENUS */
