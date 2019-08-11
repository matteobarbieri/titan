#include "Menus.hpp"
#include "Outcome.hpp"


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

Outcome * BackToGameAction::_execute()
{
    // Return outcome
    Outcome * outcome = new Outcome(
        PLAYERS_TURN, true, true);

    return outcome;
}

/*

from game_state import GamePhase

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
