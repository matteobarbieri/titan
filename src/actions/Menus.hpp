#ifndef ROGUE_20177_ACTIONS_MENU
#define ROGUE_20177_ACTIONS_MENU

#include "Action.hpp"


// Forward declarations
class Outcome;

class ShowMenuAction : public Action
{

    public:

        // Constructors
        //ShowMenuAction();
        //~ShowMenuAction();

        Outcome * _execute();

};

class ShowCharacterScreenAction : public Action
{

    public:

        Outcome * _execute();

};

class ShowInventoryAction : public Action
{

    public:

        Outcome * _execute();

};

class BackToInventoryMenuAction : public Action
{

    public:

        Outcome * _execute();

};

class BackToGameAction : public Action
{

    public:

        Outcome * _execute();

};


/*
from game_state import GamePhase

from loader_functions.data_loaders import save_game

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


*/

#endif /* ifndef ROGUE_20177_ACTIONS_MENU */
