#include "libtcod.hpp"

// Actions
#include "actions/Action.hpp"
#include "actions/Move.hpp"
#include "actions/Menus.hpp"

#include "actions/items.hpp"
#include "actions/inspect.hpp"

#include "GameState.hpp"
#include "GamePhase.hpp"

#include "input.hpp"

/*
def handle_main_menu(key):
    key_char = chr(key.c)

    if key_char == 'a':
        return {'new_game': True}
    elif key_char == 'b':
        return {'load_game': True}
    elif key_char == 'c' or  key.vk == libtcod.KEY_ESCAPE:
        return {'exit': True}

    return {}
*/

char handle_main_menu(TCOD_key_t key)
{
    switch(key.c)
    {
        case 'a':
        case 'b':
        case 'c':
            return key.c;
            break;
        default:
            return '\0';
            break;
    }
}

Action * handle_player_turn_keys(TCOD_key_t key, TCOD_mouse_t mouse)
{
    char key_char = -1;

    // Code to prevent double input
    if (key.vk == TCODK_CHAR)
        key_char = key.c;

    /////////////////////////////////////////
    ////////////// MOVEMENT /////////////////
    /////////////////////////////////////////

    // Cross movement (arrow keys or WASD)
    if (key.vk == TCODK_UP || key_char == 'w')
        return new MoveAction(0, -1);

    if (key.vk == TCODK_DOWN || key_char == 's')
        return new MoveAction(0, 1);

    if (key.vk == TCODK_LEFT || key_char == 'a')
        return new MoveAction(-1, 0);

    if (key.vk == TCODK_RIGHT || key_char == 'd')
        return new MoveAction(1, 0);

    //if (key.vk == TCODK_UP || key_char == 'k')
        //return new MoveAction(0, -1);

    //if (key.vk == TCODK_DOWN || key_char == 'j')
        //return new MoveAction(0, 1);

    //if (key.vk == TCODK_LEFT || key_char == 'h')
        //return new MoveAction(-1, 0);

    //if (key.vk == TCODK_RIGHT || key_char == 'l')
        //return new MoveAction(1, 0);

    // Diagonal movements
    // XXX DISABLED
    //if (key_char == 'y')
        //return new MoveAction(-1, -1);

    //if (key_char == 'u')
        //return new MoveAction(1, -1);

    //if (key_char == 'b')
        //return new MoveAction(-1, 1);

    //if (key_char == 'n')
        //return new MoveAction(1, 1);

    /////////////////////////////////////////
    /////////// GO TO MAIN MENU /////////////
    /////////////////////////////////////////

    if (key.vk == TCODK_ESCAPE)
        return new ShowMenuAction();

    /*
    elif key_char == 'z':
        return WaitAction()
    */

    /////////////////////////////////////////
    ///////////////// MISC //////////////////
    /////////////////////////////////////////

    /*
    elif key_char == 'c':
        return ShowCharacterScreenAction()

    */

    if (key_char == 'g')
        return new PickupAction();

    if (key_char == 'i')
        return new ShowInventoryAction();

    /////////////////////////////////////////
    //////////// MOUSE ACTIONS //////////////
    /////////////////////////////////////////

    //(x, y) = (mouse.cx, mouse.cy)

    if (mouse.lbutton_pressed)
    {
        return new InspectAction(mouse.cx, mouse.cy);
    }

    /*
     *elif mouse.rbutton_pressed:
     *    return {'right_click': (x, y)}
     */

    // No key was pressed
    return nullptr;
}

Action * handle_inventory_menu_keys(TCOD_key_t key, TCOD_mouse_t mouse)
{

    char key_char = -1;

    // Code to prevent double input
    if (key.vk == TCODK_CHAR)
    {
        key_char = key.c;

        // First check the 'i', in case the player wants to close the inventory
        if (key_char == 'i')
        {
            return new BackToGameAction();
        }

        // Then check if the letter is one of the valid inventory items
        if (key_char >= 'a' && key_char <= 'z')
        {
            return new SelectInventoryItemAction(key_char);
        }
        //DEBUG("Pressed key " << key_char);

    }

    /*
    if key.vk == libtcod.KEY_ENTER and key.lalt:
        # Alt+Enter: toggle full screen
        return ToggleFullscreenAction()
    */

    if (key.vk == TCODK_ESCAPE)
    {
        return new BackToGameAction();
    }

    return nullptr;
}

Action * handle_inventory_item_keys(TCOD_key_t key, TCOD_mouse_t mouse)
{
    char key_char = -1;

    // Code to prevent double input
    if (key.vk == TCODK_CHAR)
    {
        key_char = key.c;


        if (key_char == 'd')
            return new DropItemAction();

        if (key_char == 'e')
            return new ItemEquipToggleAction();

        /*
         *if (key_char >= 'a' && key_char <= 'z')
         *{
         *    return new SelectInventoryItemAction(key_char);
         *}
         */

        //DEBUG("Pressed key " << key_char);

    }

    if (key.vk == TCODK_ESCAPE)
    {
        return new BackToInventoryMenuAction();
    }

    return nullptr;

    /*
    # Code to prevent double input
    key_char = chr(key.c) if key.vk == libtcod.KEY_CHAR else ""

    if key.vk == libtcod.KEY_ENTER and key.lalt:
        # Alt+Enter: toggle full screen
        return ToggleFullscreenAction()

    if key_char == "d":
        return DropItemAction()
    elif key_char == "e":
        return EquipItemAction()
    elif key_char == "t":
        return UnequipItemAction()
    */
}

Action * handle_entity_info_keys(TCOD_key_t key, TCOD_mouse_t mouse)
{

    //if key.vk == libtcod.KEY_ENTER and key.lalt:
        //# Alt+Enter: toggle full screen
        //return ToggleFullscreenAction()

    // Return to normal game
    if (key.vk == TCODK_ESCAPE)
    {
        return new BackToGameAction();
    }

    // No key was pressed
    return nullptr;

}

/*
def handle_player_turn_keys(key, mouse):


    #########################################
    ################ COMBAT #################
    #########################################

    # Targeting
    if key.vk == libtcod.KEY_TAB:

        if key.shift:
            # Select the previous one
            return CycleTargetAction(-1)
        else:
            # Select the next one
            return CycleTargetAction(1)

    elif key_char == 'f':
        return ShootAction()

    #########################################
    ########## TOGGLE FULLSCREEN ############
    #########################################

    elif key.vk == libtcod.KEY_ENTER and key.lalt:
        # Alt+Enter: toggle full screen
        return ToggleFullscreenAction()


    #########################################
    ############ SELECT ENTITY ##############
    #########################################


    """
    elif key_char == 'd':
        return {'drop_inventory': True}
    elif key.vk == libtcod.KEY_ENTER:
        return {'take_stairs': True}
    """

*/

Action * handle_input(
    TCOD_key_t key, TCOD_mouse_t mouse, GameState * game_state)
{

    switch (game_state->game_phase)
    {

        /////////////////////////////////////////
        //////////// PLAYER'S TURN //////////////
        /////////////////////////////////////////
        case PLAYERS_TURN:
            //DEBUG("PLAYERS_MENU here");
            return handle_player_turn_keys(key, mouse);
            break;

        /////////////////////////////////////////
        //////////// INVENTORY MENU /////////////
        /////////////////////////////////////////
        case INVENTORY_MENU:
            return handle_inventory_menu_keys(key, mouse);
            break;

        /////////////////////////////////////////
        ////////////// ENTITY INFO //////////////
        /////////////////////////////////////////
        case ENTITY_INFO:
            return handle_entity_info_keys(key, mouse);
                break;

        /////////////////////////////////////////
        ////////// INVENTORY ITEM MENU //////////
        /////////////////////////////////////////
        case INVENTORY_ITEM_MENU:
            return handle_inventory_item_keys(key, mouse);
            break;

        default:
            return nullptr;
            break;
    }
}

/*

def handle_input(key, mouse, game_state):
    """
    Handle inputs differently depending on game state
    """


    #########################################
    ########### CHARACTER SCREEN ############
    #########################################
    elif game_state == GamePhase.CHARACTER_SCREEN:
        return handle_character_screen(key, mouse)
    # Return empty outcome dict
    return {}

*/

/*




def handle_level_up_menu(key):
    pass
    """
    if key:
        key_char = chr(key.c)

        if key_char == 'a':
            return {'level_up': 'hp'}
        elif key_char == 'b':
            return {'level_up': 'str'}
        elif key_char == 'c':
            return {'level_up': 'def'}

    return {}
    """


def handle_character_screen(key, mouse):

    if key.vk == libtcod.KEY_ENTER and key.lalt:
        # Alt+Enter: toggle full screen
        return ToggleFullscreenAction()
    elif key.vk == libtcod.KEY_ESCAPE:
        # Exit the menu, go back to main game
        return BackToGameAction()

    # No key was pressed
    return NoopAction()


def handle_targeting_keys(key):
    if key.vk == libtcod.KEY_ESCAPE:
        return {'exit': True}

    return {}

def handle_player_dead_keys(key):
    """
    The set of keys for a dead player.

    Can only see the inventory and toggle fullscreen.
    """

    key_char = chr(key.c) if key.vk == libtcod.KEY_CHAR else ""

    if key_char == 'i':
        return {'show_inventory': True}

    if key.vk == libtcod.KEY_ENTER and key.lalt:
        # Alt+Enter: toggle full screen
        return {'fullscreen': True}
    elif key.vk == libtcod.KEY_ESCAPE:
        # Exit the menu
        return {'exit': True}

    return {}

def handle_mouse(mouse):
    (x, y) = (mouse.cx, mouse.cy)

    if mouse.lbutton_pressed:
        return {'left_click': (x, y)}
    elif mouse.rbutton_pressed:
        return {'right_click': (x, y)}

    return {}
*/
