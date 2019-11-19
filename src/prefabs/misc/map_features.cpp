#include "../../libtcod.hpp"

#include "../../Entity.hpp"
#include "../../GameState.hpp"

#include "../../map/GameMap.hpp"

#include "../../components/Interactive.hpp"

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

    // TODO these are sample functions, which should not be added here
    TerminalFunction tf1("Test 1", 'a', test_f1);
    TerminalFunction tf2("Test 2", 'b', test_f2);

    interactive_component->terminal_functions.push_back(tf1);
    interactive_component->terminal_functions.push_back(tf2);

    terminal->interactive = interactive_component;

    terminal->interactive->owner = terminal;
    
    return terminal;
}
