#include "libtcod.hpp"

// Actions
#include "actions/Action.hpp"
#include "actions/Move.hpp"
#include "actions/menus_actions.hpp"
#include "actions/ui.hpp"

#include "actions/combat.hpp"
#include "actions/skills.hpp"
#include "actions/items.hpp"
#include "actions/inspect.hpp"

#include "Player.hpp"

#include "GameState.hpp"
#include "GamePhase.hpp"

#include "input.hpp"

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

/**
 * Select skill if it corresponds to one of the available skills, else return
 * nullptr.
 */
Action * select_skill(char key_char)
{
    
    int skill_index = Player::singleton().char_to_skill_index(key_char);

    if (skill_index >=0)
    {
        DEBUG("Selecting skill");
        return new UseSkillAction(Player::singleton().skills[skill_index]);
    }
    else
    {
        return nullptr;
    }
}


Action * handle_player_dead_keys(TCOD_key_t key, TCOD_mouse_t mouse)
{
    /////////////////////////////////////////
    /////////// GO TO MAIN MENU /////////////
    /////////////////////////////////////////

    if (key.vk == TCODK_ESCAPE)
        return new ShowMenuAction();

    return nullptr;

}


/**
 * When the player has cleared a level
 */
Action * handle_level_summary_keys(TCOD_key_t key, TCOD_mouse_t mouse)
{
    /////////////////////////////////////////
    /////////// GO TO MAIN MENU /////////////
    /////////////////////////////////////////

    if (key.vk == TCODK_ESCAPE)
        return new ShowMenuAction();

    return nullptr;

}

Action * handle_player_turn_keys(TCOD_key_t key, TCOD_mouse_t mouse)
{
    char key_char = -1;

    // Code to prevent double input
    if (key.vk == TCODK_CHAR || // Regular characters a-z
        (key.vk >= TCODK_0 && key.vk <= TCODK_9)) // digits 0-9
    {
        key_char = key.c;
    }

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
    /////////////// COMBAT //////////////////
    /////////////////////////////////////////

    // Attack
    if (key_char == 'f')
    {
        return new AttackAction();
    }

    // Reload
    if (key_char == 'r')
    {
        return new ReloadAction();
    }

    // TODO ADD RELOAD IN OFF HAND!

    /////////////////////////////////////////
    //////////////// SKILLS /////////////////
    /////////////////////////////////////////

    if (key_char >= '1' && key_char <= '7')
    {

        DEBUG("Selecting skill");
        return select_skill(key_char);
    }

    /////////////////////////////////////////
    //////// CYCLE THROUGH TARGETS //////////
    /////////////////////////////////////////

    
    if (key.vk == TCODK_TAB)
    {
        if (key.shift)
        {
            //DEBUG("Pressed SHIFT TAB!");
            return new CycleTargetAction(-1);
        }
        else
        {
            //DEBUG("Pressed TAB!");
            return new CycleTargetAction(1);
        }
    }

    /////////////////////////////////////////
    /////////// GO TO MAIN MENU /////////////
    /////////////////////////////////////////

    if (key.vk == TCODK_ESCAPE)
        return new ShowMenuAction();

    /////////////////////////////////////////
    ///////////////// MISC //////////////////
    /////////////////////////////////////////

    if (key_char == 'z')
        return new WaitAction();
    
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

    if (key.vk == TCODK_F1)
    {
        return new ShowHelpAction();
    }

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

    if (key.vk == TCODK_ESCAPE)
    {
        return new BackToGameAction();
    }

    return nullptr;
}


Action * handle_popup_message_keys(TCOD_key_t key, TCOD_mouse_t mouse)
{

    char key_char = -1;

    // Code to prevent double input
    if (key.vk == TCODK_CHAR)
    {
        key_char = key.c;

        // First check the 'i', in case the player wants to close the inventory
        //if (key_char == 'i')
        //{
            //return new BackToGameAction();
        //}
    }

    if (key.vk == TCODK_ESCAPE || key.vk == TCODK_F1)
    {
        return new BackToGameAction();
    }

    return nullptr;
}


Action * handle_terminal_menu_keys(TCOD_key_t key, TCOD_mouse_t mouse)
{

    char key_char = -1;

    // Code to prevent double input
    if (key.vk == TCODK_CHAR)
    {

        key_char = key.c;

        // Then check if the letter is one of the valid inventory items
        if (key_char >= 'a' && key_char <= 'z')
        {
            return new SelectTerminalFunctionAction(key_char);
        }
        //DEBUG("Pressed key " << key_char);

    }

    if (key.vk == TCODK_ESCAPE)
    {
        return new GoToPhaseAction(ENEMY_TURN);
    }

    return nullptr;
}

Action * handle_container_menu_keys(TCOD_key_t key, TCOD_mouse_t mouse)
{

    char key_char = -1;

    // Code to prevent double input
    if (key.vk == TCODK_CHAR)
    {

        key_char = key.c;

        // Then check if the letter is one of the valid inventory items
        if (key_char >= 'a' && key_char <= 'z')
        {
            return new SelectContainerItemAction(key_char);
        }
        //DEBUG("Pressed key " << key_char);

    }

    if (key.vk == TCODK_ESCAPE)
    {
        return new GoToPhaseAction(ENEMY_TURN);
    }

    return nullptr;
}


Action * handle_container_item_menu_keys(TCOD_key_t key, TCOD_mouse_t mouse)
{

    char key_char = -1;

    // Code to prevent double input
    if (key.vk == TCODK_CHAR)
    {
        key_char = key.c;

        if (key_char == 'g')
        {
            return new TransferItemAction();
        }

    }

    if (key.vk == TCODK_ESCAPE)
    {
        return new GoToPhaseAction(CONTAINER_MENU);
    }

    return nullptr;
}

Action * handle_targeting_keys(TCOD_key_t key, TCOD_mouse_t mouse, GamePhase game_phase)
{
    char key_char = -1;

    // Code to prevent double input
    if (key.vk == TCODK_CHAR)
    {
        key_char = key.c;

    }

    // TODO should probably return to game instead
    if (key.vk == TCODK_ESCAPE)
    {
        //throw CancelActionException();
        return new CancelAction();
    }

    if (mouse.lbutton_pressed)
    {
        switch (game_phase)
        {
            case TARGETING_ITEM:
                return new ItemResolveTargetingAction(mouse.cx, mouse.cy);
                break;
            case TARGETING_SKILL:
                return new ResolveSkillAction(mouse.cx, mouse.cy);
                break;
            default:
                // TODO throw exception maybe?
                DEBUG("Should not arrive here....");
                break;
        }
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

        if (key_char == 'u')
            return new ItemUseAction();

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

    // Return to normal game
    if (key.vk == TCODK_ESCAPE)
    {
        return new BackToGameAction();
    }

    if (mouse.lbutton_pressed)
    {
        return new InspectAction(mouse.cx, mouse.cy);
    }

    // No key was pressed
    return nullptr;

}

Action * handle_input(
    TCOD_key_t key, TCOD_mouse_t mouse, GameState * game_state)
{

    /////////////////////////////////////////
    ////////// TOGGLE FULLSCREEN ////////////
    /////////////////////////////////////////
    if (key.vk == TCODK_ENTER && key.lalt)
    {
        return new ToggleFullscreenAction();
    }

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
        ///////////// TERMINAL MENU /////////////
        /////////////////////////////////////////
        case TERMINAL_MENU:
            return handle_terminal_menu_keys(key, mouse);
            break;

        /////////////////////////////////////////
        //////////// INVENTORY MENU /////////////
        /////////////////////////////////////////
        case INVENTORY_MENU:
            return handle_inventory_menu_keys(key, mouse);
            break;

        /////////////////////////////////////////
        //////////// CONTAINER MENU /////////////
        /////////////////////////////////////////
        case CONTAINER_MENU:
            return handle_container_menu_keys(key, mouse);
            break;

        /////////////////////////////////////////
        ///////// CONTAINER ITEM MENU ///////////
        /////////////////////////////////////////
        case CONTAINER_ITEM_MENU:
            return handle_container_item_menu_keys(key, mouse);
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

        /////////////////////////////////////////
        ////////// TARGETING ITEM MENU //////////
        /////////////////////////////////////////
        case TARGETING_ITEM:
        case TARGETING_SKILL:
            return handle_targeting_keys(key, mouse, game_state->game_phase);
            break;

        /////////////////////////////////////////
        ///////////// POPUP MESSAGE /////////////
        /////////////////////////////////////////
        case POPUP_MESSAGE:
            // TODO create the right function for this
            return handle_popup_message_keys(key, mouse);
            break;

        /////////////////////////////////////////
        ////////////// PLAYER DEAD //////////////
        /////////////////////////////////////////
        case PLAYER_DEAD:
            return handle_player_dead_keys(key, mouse);
            break;

        /////////////////////////////////////////
        ///////////// LEVEL SUMMARY /////////////
        /////////////////////////////////////////
        case LEVEL_SUMMARY:
            return handle_level_summary_keys(key, mouse);
            break;

        default:
            return nullptr;
            break;
    }
}
