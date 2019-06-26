#include "libtcod.hpp"

#include <vector>

#include "Constants.h"
#include "menus.hpp"

#include "Consoles.hpp"

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

    //std::cout << "options.size(): " << options.size() << std::endl;

    // create an off-screen console that represents the menu's window

    //Consoles::singleton().menu->setDefaultForeground(TCODColor::white);
    con->setDefaultForeground(TCODColor::white);
    // print the header, with auto-wrap
    //Consoles::singleton().menu->printRectEx(
    con->printRectEx(
        0, 0, width, height, TCOD_BKGND_NONE, TCOD_LEFT, "%s", header);

    // print all the options
    int opt_y = header_height;

    // TODO implement this bit here!
    //for option_letter, option_text in options:
        //text = "({}) {}".format(option_letter, option_text)
        //libtcod.console_print_ex(
            //window, 0, opt_y, libtcod.BKGND_NONE, libtcod.LEFT, text)
        //opt_y += 1

    // Set default values for x and y if they are not set
    int menu_x, menu_y;
    if (x == -1)
    {
        menu_x = SCREEN_WIDTH/2 - width/2;
    } else
    {
        menu_x = x;
    }

    if (y == -1)
    {
        menu_y = SCREEN_HEIGHT/2 - height/2;
    } else
    {
        menu_y = y;
    }
        
    TCODConsole::blit(
        //Consoles::singleton().menu,
        con,
        0, 0, width, height,
        TCODConsole::root,
        menu_x, menu_y);


    // blit the contents of "window" to the root console
    //libtcod.console_blit(window, 0, 0, width, height, 0, x, y, 1.0, 0.4)
}

void main_menu(TCODImage * background_image)
{
    
    // Blit background image
    //background_image->blit2x(Consoles::singleton().terrain_layer, 0, 0);
    background_image->blit2x(TCODConsole::root, 0, 0);

    // Set default fg color
    //Consoles::singleton().terrain_layer->setDefaultForeground(TCODColor::lightYellow);
    TCODConsole::root->setDefaultForeground(TCODColor::lightYellow);

    //libtcod.console_print_ex(0, int(screen_width / 2), int(screen_height / 2)
                             //- 4, libtcod.BKGND_NONE, libtcod.CENTER,
                             //'Rogue 20177')
                             
    // Print game title
    //Consoles::singleton().terrain_layer->printf(
    TCODConsole::root->printf(
        SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - 4,
        "%s", GAME_NAME);

    //Consoles::singleton().terrain_layer->printEx(
        //SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - 4,
        //TCOD_BKGND_NONE, TCOD_CENTER, "%s", GAME_NAME);

    // Print author
    //libtcod.console_print_ex(0, int(screen_width / 2), int(screen_height - 2),
                             //libtcod.BKGND_NONE, libtcod.CENTER,
                             //'By heXe')

    //menu(con, "",
         //['Play a new game', 'Continue last game', 'Quit'], 24,
         //screen_width, screen_height)
         //
         //

    // Build the list of options
    std::vector<MenuOption> options;
    menu(Consoles::singleton().menu, options, " ",
         24);

}

//def main_menu(con, background_image, screen_width, screen_height):



/*

from equipment_slots import SLOT_NAMES

    ###############################
    ########### FIN QUI ###########
    ###############################



def item_submenu(con, header, player, item, screen_width, screen_height):

    if item.item.equipped:
        starting_menu_y = 30
    else:
        starting_menu_y = 5

    item_position = (
        player.inventory.get_item_position_in_list(item) +
        starting_menu_y)

    # TODO limit the height of the submenu
    item_position = min(item_position, 20000)

    # TODO
    # Get the right list of options for specific item
    # item_options = [
        # ('d', 'Drop'),
        # ('e', 'Equip'),
        # ('u', 'Use'),
    # ]

    item_options = item.item.get_inventory_options()

    width = 15

    # TODO
    # X position should depend on frame's width
    menu(con, header, item_options, width, screen_width, screen_height,
         x=31, y=item_position)

def inventory_menu(con, header, player, inventory_frame,
                   screen_width, screen_height):

    # Extract width and height
    w = inventory_frame.width
    h = inventory_frame.height

    # Draw frame
    inventory_frame.draw_frame(
        1, 1,
        w-2, h-2,
        'Inventory')

    # List the items in the inventory
    # Starting
    item_y = 5

    if player.inventory.items:
        for e in player.inventory.items:
            if not e.item.equipped:
                inventory_frame.print(
                    3, item_y, '({}) {}'.format(e.item_letter, e.name),
                    fg=e.color)

                item_y += 1

    # List equipped items
    if player.equipment.slots:

        inventory_frame.print(
                3, 28, 'Equipped items:',
            fg=libtcod.white)

        item_y = 30
        for slot, e in player.equipment.slots.items():
            inventory_frame.print(
                3, item_y, '({}) {} ({})'.format(
                    e.item_letter, e.name, SLOT_NAMES[slot]),
                fg=e.color)

            item_y += 1

    else:
        inventory_frame.print(
                3, 28, 'No items equipped!',
            fg=libtcod.white)

    # Blit panel console on root console
    libtcod.console_blit(
        inventory_frame,
        0, 0, w, h,
        con,
        0, 0)


def character_screen(player,
                     character_screen_width, character_screen_height,
                     screen_width, screen_height):

    # Create new console for showing character info
    window = libtcod.console_new(
        character_screen_width, character_screen_height)

    # Foreground color: white
    libtcod.console_set_default_foreground(
        window, libtcod.white)

    # Header of the character info section
    libtcod.console_print_rect_ex(
        window, 0, 1, character_screen_width, character_screen_height,
        libtcod.BKGND_NONE, libtcod.LEFT, 'Character Information')

    # Character level
    libtcod.console_print_rect_ex(
        window, 0, 2, character_screen_width, character_screen_height,
        libtcod.BKGND_NONE, libtcod.LEFT,
        'Level: {0}'.format(player.level.current_level))

    # Character experience
    libtcod.console_print_rect_ex(
        window, 0, 3, character_screen_width, character_screen_height,
        libtcod.BKGND_NONE, libtcod.LEFT,
        'Experience: {0}'.format(player.level.current_xp))

    # Experience to next level
    libtcod.console_print_rect_ex(
        window, 0, 4, character_screen_width, character_screen_height,
        libtcod.BKGND_NONE, libtcod.LEFT,
        'Experience to Level: {0}'.format(
            player.level.experience_to_next_level))

    # Maximum HP
    libtcod.console_print_rect_ex(
        window, 0, 6, character_screen_width, character_screen_height,
        libtcod.BKGND_NONE, libtcod.LEFT,
        'Maximum HP: {0}'.format(player.fighter.max_hp))

    # Attack value
    libtcod.console_print_rect_ex(
        window, 0, 7, character_screen_width, character_screen_height,
        libtcod.BKGND_NONE, libtcod.LEFT,
        'Attack: {0}'.format(player.fighter.power))

    # Defense value
    libtcod.console_print_rect_ex(
        window, 0, 8, character_screen_width, character_screen_height,
        libtcod.BKGND_NONE, libtcod.LEFT,
        'Defense: {0}'.format(player.fighter.defense))

    # Calculate character screen position w.r.t. the main screen
    x = screen_width // 2 - character_screen_width // 2
    y = screen_height // 2 - character_screen_height // 2

    # Blit character console on root console
    libtcod.console_blit(
        window, 0, 0, character_screen_width, character_screen_height,
        0, x, y, 1.0, 0.7)


def message_box(con, header, width, screen_width, screen_height):
    menu(con, header, [], width, screen_width, screen_height)
*/
