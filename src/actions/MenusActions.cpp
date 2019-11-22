#include "Menus.hpp"
#include "Outcome.hpp"

#include "../Entity.hpp"
#include "../GameState.hpp"

#include "../components/Inventory.hpp"
#include "../components/Interactive.hpp"
#include "../components/Item.hpp"

Outcome * ShowMenuAction::_execute()
{
    // Throw exception to go to main menu;
    throw ShowMenuException();
    return nullptr;
}

Outcome * ShowInventoryAction::_execute()
{

    // Return outcome
    Outcome * outcome = new Outcome(
        //INVENTORY_MENU, false, false);
        INVENTORY_MENU, true, true);

    return outcome;
}

Outcome * BackToInventoryMenuAction::_execute()
{
    // Clear selected inventory item
    game_state->selected_inventory_item = nullptr;

    // Return outcome
    Outcome * outcome = new Outcome(
        INVENTORY_MENU, true, true);

    return outcome;
}


/////////////////////////////
//////// GO TO PHASE ////////
/////////////////////////////

GoToPhaseAction::GoToPhaseAction(GamePhase next_phase) : next_phase(next_phase)
{
}

Outcome * GoToPhaseAction::_execute()
{
    // Return outcome
    Outcome * outcome = new Outcome(
        next_phase, true, true);

    return outcome;
}

Outcome * BackToGameAction::_execute()
{
    // Return outcome
    Outcome * outcome = new Outcome(
        PLAYERS_TURN, true, true);

    return outcome;
}

SelectInventoryItemAction::SelectInventoryItemAction(char item_letter) :
    item_letter(item_letter)
{
}

Outcome * SelectInventoryItemAction::_execute()
{

    Entity * aux;
    bool found = false;

    //DEBUG((int)player->inventory->items().size());

    for (int i=0; i<(int)player->inventory->items.size(); i++)
    {
        // shortcut to entity
        aux = player->inventory->items[i];

        // Check if the letter coincides and the item is not currently equipped
        //if (aux->item->item_letter == item_letter && !aux->item->equipped)
        
        //DEBUG(aux->item->item_letter);

        if (aux->item->item_letter == item_letter)
        {
            game_state->selected_inventory_item = aux;
            found = true;
            break;
        }
    }
    
    if (found)
    {
        Outcome * outcome = new Outcome(
            INVENTORY_ITEM_MENU, false, false);

        return outcome;
    }
    else
    {
        return nullptr;
    }
}


SelectTerminalFunctionAction::SelectTerminalFunctionAction(char command_shortcut) :
    command_shortcut(command_shortcut)
{
}


Outcome * SelectTerminalFunctionAction::_execute()
{

    bool found = false;

    // Read terminal options from entity interactive component
    InteractiveTerminal * interactive_component = 
        (InteractiveTerminal *)game_state->entity_interacted->interactive;
    
    for (int i=0; i<(int)interactive_component->terminal_functions.size(); i++)
    {
        // Shortcut to Terminal function
        TerminalFunction * tf = interactive_component->terminal_functions[i];

        // Check if the letter pressed coincides with the shortcut
        if (tf->command_shortcut == command_shortcut)
        {
            // Execute the actual function here
            tf->execute(player, game_map, game_state);
            found = true;
            break;
        }
    }
    
    if (found)
    {

        // TODO check next game phase
        Outcome * outcome = new Outcome(
            TERMINAL_MENU, true, true);

        return outcome;
    }
    else
    {
        return nullptr;
    }
}

/*

from loader_functions.data_loaders import save_game


class ShowCharacterScreenAction(Action):

    def _execute(self):

        # Return outcome
        outcome = {
            'next_state': GamePhase.CHARACTER_SCREEN,
        }

        return outcome


class SelectInventoryItemAction(Action):

    def __init__(self, item_letter):
        self.item_letter = item_letter

    def _execute(self):

        try:
            item_index = self.player.inventory.item_letters.index(
                self.item_letter)
            item = self.player.inventory.items[item_index]
            # print("Selected {}!".format(item))
            next_phase = GamePhase.INVENTORY_ITEM_MENU
        except ValueError as e:
            # print("Value error!")
            item = None
            next_phase = GamePhase.INVENTORY_MENU

        except IndexError as e:
            # print("Index error!")
            item = None
            next_phase = GamePhase.INVENTORY_MENU

        except Exception as e:
            print("Uncaught Exception!")
            raise e

        # Return outcome
        outcome = {
            'selected_inventory_item': item,
            'next_state': next_phase
        }

        return outcome



class BackToInventoryMenuAction(Action):

    def _execute(self):
        """
        Simply reset the state to show inventory menu
        """

        # Return outcome
        outcome = {
            'next_state': GamePhase.INVENTORY_MENU,
            'redraw_terrain': True,
        }

        return outcome


*/
