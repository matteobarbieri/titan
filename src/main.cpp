//#include <iostream>

#include <stdio.h>

#include "libtcod.hpp"
//#include "libtcod.h"

//using namespace std;

int main()
{
	int w=50, h=50;

	//TCOD_console_init_root(w, h, "Test 1", false, TCOD_RENDERER_SDL2);
	tcod::console::init_root(w, h, "Test 1", false, TCOD_RENDERER_SDL2);
	//TCODConsole::initRoot (w, h, "Test 1");
	
	printf("Hello world!\n");

	//cout << "Hello, world!" << endl;
}
