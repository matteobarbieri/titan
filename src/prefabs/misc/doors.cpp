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

Entity * make_terminal(int x, int y, TCODColor symbol_color, std::string terminal_name, int symbol, bool is_active)
{

    Entity * terminal = new Entity(
        x, y, symbol,
        symbol_color, terminal_name,
        STAIRS,
        true, false, true);

    terminal->tag = "terminal";

     //Add the interactive component to the panel
    terminal->interactive = new InteractiveTerminal(
            is_active);
    terminal->interactive->owner = terminal;
    
    return terminal;
}
