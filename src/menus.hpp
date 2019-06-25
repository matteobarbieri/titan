#include "libtcod.hpp"

#include <vector>

#ifndef R20177_MENUS
#define R20177_MENUS

typedef struct {
    char letter;
    char * text;
} MenuOption;

void menu(TCODConsole * con, std::vector<MenuOption> options, char * header,
          int width, int screen_width, int screen_height,
          int x, int y, TCODColor header_fg);
         

//def menu(con, header, options, width, screen_width, screen_height,
         //x=None, y=None, header_fg=libtcod.white):

#endif /* ifndef R20177_MENUS */
