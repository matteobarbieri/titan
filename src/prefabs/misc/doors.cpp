#include "../../libtcod.hpp"

#include "../../Entity.hpp"

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

void test_f()
{
    std::cout << "This is a test function from a terminal" << std::endl;
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

    //interactive_component->terminal_options.push_back(test_f);
    interactive_component->terminal_options.push_back((void *)test_f);

    terminal->interactive = interactive_component;

    terminal->interactive->owner = terminal;
    
    return terminal;
}
