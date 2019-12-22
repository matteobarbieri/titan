#include "../libtcod.hpp"

#include <vector>
#include <string>

#include "../Constants.h"
#include "../Consoles.hpp"

#include "../Entity.hpp"
#include "../EquipmentSlots.hpp"

#include "../components/Inventory.hpp"
#include "../components/Interactive.hpp"
#include "../components/Item.hpp"
#include "../components/Equipment.hpp"
#include "../components/Equippable.hpp"
#include "../components/Reloadable.hpp"
#include "../components/Container.hpp"


#include "menus.hpp"

void terminal_menu(Entity * terminal)
{
    // Extract width and height
    //int w = Consoles::singleton().menu->getWidth();
    //int h = Consoles::singleton().menu->getHeight();

    // Draw frame
    // Reset the color to white, just in case
    Consoles::singleton().terminal->setDefaultForeground(TCODColor::white);
    Consoles::singleton().terminal->printFrame(
        0, 0,
        TERMINAL_FRAME_WIDTH, TERMINAL_FRAME_HEIGHT,
        true, TCOD_BKGND_NONE, terminal->name.c_str());

    // Read terminal options from entity interactive component
    InteractiveTerminal * interactive_component = (InteractiveTerminal *)terminal->interactive;
    
    // TODO possibly move these in constants.h
    int menu_item_x = 4;
    int menu_item_y = 4;

    for (int i=0; i<(int)interactive_component->terminal_functions.size(); i++)
    {

        TerminalFunction * tf = interactive_component->terminal_functions[i];

        // Set the color first
        if (tf->enabled)
        {
            Consoles::singleton().terminal->setDefaultForeground(TCODColor::white);
        }
        else
        {
            Consoles::singleton().terminal->setDefaultForeground(TCODColor::lightGrey);
        }

        // Then print menu item
        Consoles::singleton().terminal->printf(
            menu_item_x, menu_item_y,
            "(%c) %s",
            tf->command_shortcut, tf->command.c_str());

        // Increment y coordinate of next menu item by one
        menu_item_y++;
    }

}


void inventory_menu(Entity * player, std::string header)
{
    // Extract width and height
    int w = Consoles::singleton().inventory_frame->getWidth();
    int h = Consoles::singleton().inventory_frame->getHeight();

    // Draw frame
    // Reset the color to white, just in case
    Consoles::singleton().inventory_frame->setDefaultForeground(TCODColor::white);
    Consoles::singleton().inventory_frame->printFrame(
        1, 1,
        w-2, h-2,
        true, TCOD_BKGND_NONE, header.c_str());

    // List the items in the inventory
    // Starting menu item height
    int item_y = INVENTORY_ITEMS_Y;

    Entity * e;

    for (int i=0; i<(int)player->inventory->items.size(); i++)
    {

        e = player->inventory->items[i];
        // If it isn't equipped
        if (!e->item->equipped)
        {

            // TODO Highlight item line if item is selected
            //Consoles::singleton().inventory_frame->setDefaultBackground(TCODColor::lightGrey);
            // Set the color first
            Consoles::singleton().inventory_frame->setDefaultForeground(e->color());

            // Then print menu item
            if (e->equippable != nullptr && e->equippable->reloadable != nullptr)
            {
                Consoles::singleton().inventory_frame->printf(
                    3, item_y,
                    "(%c)   %s [%d/%d]", // Leave enough space for the item's symbol
                    e->item->item_letter, e->name.c_str(),
                    e->equippable->reloadable->ammo, e->equippable->reloadable->clip_size);
            }
            else
            {
                Consoles::singleton().inventory_frame->printf(
                    3, item_y,
                    "(%c)   %s", // Leave enough space for the item's symbol
                    e->item->item_letter, e->name.c_str());
            }

            // Print the item's symbol separately
            Consoles::singleton().inventory_frame->putChar(7, item_y, e->symbol);

            // Increment y coordinate of next menu item by one
            item_y++;

        }
    } 

    // Print equipped items section title
    Consoles::singleton().inventory_frame->setDefaultForeground(TCODColor::white);
    Consoles::singleton().inventory_frame->printf(
        5, EQUIPPED_ITEMS_Y - 2,
        "== EQUIPPED ITEMS ==");

    item_y = EQUIPPED_ITEMS_Y;
    
    // Loop through equipment slots
    std::map<EquipmentSlot, Entity *>::iterator it;
    for (it=player->equipment->slots.begin(); it!=player->equipment->slots.end(); ++it)
    {
        if (it->second != nullptr) // if there is something equipped
        {
            // Set the color first
            Consoles::singleton().inventory_frame->setDefaultForeground(it->second->color());
            e = it->second;
            if (e->equippable != nullptr && e->equippable->reloadable != nullptr)
            {
                Consoles::singleton().inventory_frame->printf(
                    3, item_y,
                    "(%c)   %s [%d/%d] [%s]", // Leave enough space for the item's symbol
                    e->item->item_letter, e->name.c_str(),
                    e->equippable->reloadable->ammo, e->equippable->reloadable->clip_size,
                    SlotName::singleton().slot_names_short[it->first].c_str());
            }
            else
            {
                Consoles::singleton().inventory_frame->printf(
                    3, item_y,
                    "(%c)   %s [%s]", // Leave enough space for the item's symbol
                    e->item->item_letter, e->name.c_str(),
                    SlotName::singleton().slot_names_short[it->first].c_str());
            }

            // Print the item's symbol separately
            Consoles::singleton().inventory_frame->putChar(7, item_y, e->symbol);

            // Then print menu item
            //Consoles::singleton().inventory_frame->printf(
                //3, item_y,
                //"(%c) %s [%s]",
                //it->second->item->item_letter, it->second->name.c_str(),
                //SlotName::singleton().slot_names_short[it->first].c_str());
        }
        else
        {
            // Set the color first
            Consoles::singleton().inventory_frame->setDefaultForeground(TCODColor::white);

            // Then print menu item
            Consoles::singleton().inventory_frame->printf(
                3, item_y,
                "EMPTY [%s]",
                SlotName::singleton().slot_names[it->first].c_str());
           //it->first;
        }
        
        item_y++;
    }

}

void menu(TCODConsole * con, std::vector<MenuOption> options, const char * header,
          int width,
          int x=-1, int y=-1, TCODColor header_fg=TCODColor::white)
{

    if (options.size() > 26)
    {
        // TODO consider changing to a more specific exception
        throw "Cannot have a menu with more than 26 options.";
    }

    // calculate total height for the header (after auto-wrap)
    // and one line per option
    int header_height = con->getHeightRect(0, 0, width, SCREEN_HEIGHT, "%s", header);

    // Compute the size of the "menu console"
    int height = options.size() + header_height;

    con->setDefaultForeground(header_fg);
    // print the header, with auto-wrap
    con->printRectEx(
        0, 0, width, height, TCOD_BKGND_NONE, TCOD_LEFT, "%s", header);

    // print all the options
    int opt_y = header_height;
    
    for (int i=0; i<(int)options.size(); i++)
    {
        con->printf(0, opt_y, "(%c) %s", options[i].letter, options[i].text);
        opt_y++;
    }

    // Set default values for x and y if they are not set
    if (x == -1) x = SCREEN_WIDTH/2 - width/2;
    if (y == -1) y = SCREEN_HEIGHT/2 - height/2;
        
    TCODConsole::blit(
        con,
        0, 0, width, height,
        TCODConsole::root,
        x, y,
        1.0, 0.4);

}

void main_menu(TCODImage * background_image)
{
    
    // Blit background image
    //background_image->blit2x(Consoles::singleton().terrain_layer, 0, 0);
    background_image->blit2x(TCODConsole::root, 0, 0);

    // Set default fg color
    //Consoles::singleton().terrain_layer->setDefaultForeground(TCODColor::lightYellow);
    TCODConsole::root->setDefaultForeground(TCODColor::lightYellow);

    // Print game title
    // TODO work on alignment
    // check http://roguecentral.org/doryen/data/libtcod/doc/1.5.2/html2/console_print.html
    TCODConsole::root->printf(
        SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - 4,
        "%s", GAME_NAME);

    // Print author
    //libtcod.console_print_ex(0, int(screen_width / 2), int(screen_height - 2),
                             //libtcod.BKGND_NONE, libtcod.CENTER,
                             //'By heXe')

    // Build the list of options

    // TODO refactor as singleton
    std::vector<MenuOption> options;
    options.push_back({'a', "Play a new game"});
    options.push_back({'b', "Continue last game"});
    options.push_back({'c', "Quit"});

    menu(Consoles::singleton().menu, options, " ",
         24);

}

void inventory_item_submenu(Entity * player, Entity * item)
{

    int menu_y;
    
    // Start from a different height, based on whether the item is currently
    // equipped or not
    if (item->item->equipped)
    {
        menu_y = EQUIPPED_ITEMS_Y;
    }
    else
    {
        menu_y = INVENTORY_ITEMS_Y;
    }

    // TODO limit the height of the submenu
    //item_position = min(item_position, 20000)
    
    // TODO compute dinamically?
    int submenu_height = 5;

    Consoles::singleton().submenu->clear();

    menu(Consoles::singleton().submenu, item->item->item_inventory_options(), " ",
         ITEM_SUBMENU_WIDTH, 0, 0);

    // Blit here
    TCODConsole::blit(
        Consoles::singleton().submenu, 0, 0,
        ITEM_SUBMENU_WIDTH, submenu_height,
        Consoles::singleton().main_window,
        //TCODConsole::root,
        //FRAME_WIDTH, menu_y);
        FRAME_WIDTH, menu_y,
        1.0, 0.4);
    
}

void container_item_submenu(Entity * item, bool in_container)
{

    int menu_y;

    menu_y = CONTAINER_ITEMS_Y;
    
    // TODO limit the height of the submenu
    //item_position = min(item_position, 20000)
    
    // TODO compute dinamically?
    int submenu_width = 15;
    int submenu_height = 5;

    Consoles::singleton().submenu->clear();

    // TODO improve this?
    std::vector<MenuOption> container_item_submenu_options;

    if (in_container)
    {
        container_item_submenu_options.push_back({'g', "Retrieve"});
    }
    else
    {
        container_item_submenu_options.push_back({'g', "Store"});
    }

    menu(Consoles::singleton().submenu, container_item_submenu_options, " ",
         submenu_width, 0, 0);

    // Blit here
    TCODConsole::blit(
        Consoles::singleton().submenu, 0, 0,
        submenu_width, submenu_height,
        Consoles::singleton().main_window,
        //TCODConsole::root,
        //FRAME_WIDTH, menu_y);
        INVENTORY_FRAME_WIDTH, menu_y,
        1.0, 0.4);
    
}

void container_menu(Entity * container)
{
    // Extract width and height
    int w = Consoles::singleton().container_frame->getWidth();
    int h = Consoles::singleton().container_frame->getHeight();

    // Draw frame
    // Reset the color to white, just in case
    Consoles::singleton().container_frame->setDefaultForeground(TCODColor::white);
    Consoles::singleton().container_frame->printFrame(
        1, 1,
        w-2, h-2,
        true, TCOD_BKGND_NONE, container->name.c_str());

    // TODO possibly move these in constants.h
    int item_entry_x = 4;
    int item_entry_y = 4;

    if (container->container->items.size() == 0)
    {
        // Then print item entry
        Consoles::singleton().container_frame->printf(
            item_entry_x, item_entry_y,
            "    ==== EMPTY ====");
    }

    for (int i=0; i<(int)container->container->items.size(); i++)
    {


        Entity * e = container->container->items[i];

        //DEBUG("[CONTAINER MENU] Printing item entry" << e->name);

        // Set color
        Consoles::singleton().container_frame->setDefaultForeground(e->color());

        // Then print item entry
        Consoles::singleton().container_frame->printf(
                item_entry_x, item_entry_y,
                "(%c)   %s", // Leave enough space for the item's symbol
                e->item->item_letter, e->name.c_str());

        // Print the item's symbol separately
        Consoles::singleton().container_frame->putChar(item_entry_x+4, item_entry_y, e->symbol);

        // Increment y coordinate of next item entry by one
        item_entry_y++;
    }

}


