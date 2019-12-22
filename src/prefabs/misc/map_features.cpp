#include "../../libtcod.hpp"

#include "../../Entity.hpp"
#include "../../GameState.hpp"

#include "../../map/GameMap.hpp"

#include "../../components/Interactive.hpp"
#include "../../components/Container.hpp"

Entity * make_door(int x, int y, bool is_open, bool locked, unsigned int key_id)
{

    Entity * door = new Entity(
        x, y, '+',
        TCODColor::orange, "Door",
        STAIRS,
        true, true, true);

    door->tag = "door";

    door->interactive = new InteractiveDoor(is_open, locked, key_id);
    door->interactive->owner = door;
    return door;
}

Entity * make_text_panel(int x, int y, Direction * readable_from, std::string text,
                         TCODColor symbol_color, TCODColor text_color, int symbol, bool is_active)
{

    Entity * panel = new Entity(
        x, y, symbol,
        symbol_color, "Panel",
        STAIRS,
        true, true, true);

    panel->tag = "panel";

    // Add the interactive component to the panel
    panel->interactive = new InteractivePanel(
            text, text_color, readable_from, is_active);
    panel->interactive->owner = panel;
    return panel;
}

void test_f1(Entity * player, GameMap * game_map, GameState * game_state)
{
    std::cout << "This is test function f1 from a terminal" << std::endl;
}

void test_f2(Entity * player, GameMap * game_map, GameState * game_state)
{
    std::cout << "This is test function f2 from a terminal" << std::endl;
}

Entity * make_terminal(int x, int y, TCODColor symbol_color, std::string terminal_name, int symbol, bool is_active)
{

    Entity * terminal = new Entity(
        x, y, symbol,
        symbol_color, terminal_name,
        STAIRS,
        true, false, true);

    terminal->tag = "terminal";

     //Add the interactive component to the panel
    InteractiveTerminal * interactive_component = new InteractiveTerminal(
        is_active);

    terminal->interactive = interactive_component;

    terminal->interactive->owner = terminal;
    
    return terminal;
}


Entity * make_container(int x, int y,
                        TCODColor symbol_color,
                        std::string name,
                        int symbol,
                        bool locked,
                        unsigned int key_id)
{

    Entity * c = new Entity(
        x, y, symbol,
        symbol_color, name,
        ITEM,
        true, false, true);

    c->tag = "container";

    c->container = new Container();
    c->container->owner = c;

    // Add the interactive component to the c
    c->interactive = new InteractiveContainer(locked, key_id);
    c->interactive->owner = c;
    return c;
}

