#include <string>
#include <sstream>

#include "items.hpp"

#include "../GamePhase.hpp"
#include "../Entity.hpp"
#include "../components/Inventory.hpp"
//#include "../GameMessages.hpp"
#include "../GameState.hpp"

#include "../map/GameMap.hpp"

#include "Outcome.hpp"

Outcome * PickupAction::_execute()
{

    // Retrieve item on floor at player's coordinates
    Entity * item_on_floor = game_map->get_item_at(
        player->x, player->y);

    GamePhase next_phase;

    if (item_on_floor != nullptr)
    {
        
        // If there actually is an item on the floor, add it to the player's
        // inventory. Also now it's the enemies' turn (unless for some reason
        // the item can't be picked up).
        
        //# Return a feedback message
        //return Message("You pick up a {}".format(item),
                //libtcod.white)

        DEBUG("There's something here!");

        try
        {
            player->inventory->pickup(item_on_floor, game_map);
            
            //# Change game phase (enemies' turn)
            //next_phase = ENEMY_TURN; // TODO this is the right one!
            next_phase = PLAYERS_TURN;
            
            // Build message
            std::ostringstream stringStream;
            stringStream << "You pick up a " << item_on_floor->name << ".";

            // Add message to message log
            game_state->message_log->add_message(
                {stringStream.str(), TCODColor::white});

        }
        catch (const InventoryFullException& e) 
        {
            // Build message
            std::ostringstream stringStream;
            stringStream << "Unable to pick up " << item_on_floor->name << ": inventory full.";

            // Add message to message log
            game_state->message_log->add_message(
                {stringStream.str(), TCODColor::yellow});

            next_phase = PLAYERS_TURN;
        }
    }
    else
    {
        // No item on ground
        // Add message to message log
        game_state->message_log->add_message(
            {"There's nothing here to pick up!", TCODColor::yellow});

        next_phase = PLAYERS_TURN;
    }
     
    // Return outcome
    // TODO enable message log
    Outcome * outcome = new Outcome(
        next_phase, // TODO this is the WRONG one (for debug purposes)
        true,
        true);

    return outcome;

}

/*
from .action import Action

from game_state import GamePhase

from entity import get_blocking_entities_at_location

import libtcodpy as libtcod

from game_messages import Message

import random

from components.inventory import InventoryFullException
from components.equipment import UnableToEquipException


class UnequipItemAction(Action):

    def __init__(self, **kwargs):
        pass

    def _execute(self):

        # raise Exception("Not implemented")

        messages = list()

        try:

            item_to_unequip = self.game_state.selected_inventory_item

            if not item_to_unequip.item.can_perform_action('unequip'):
                return {'messages': messages}

            # (try to) equip the item
            messages.extend(self.player.inventory.unequip(
                item_to_unequip))

            # Reset selection
            self.game_state.selected_inventory_item = None

            # Change game phase (enemies' turn)
            # next_phase = GamePhase.ENEMY_TURN
            # TODO changing equipment should require some time, but keep the
            # menu open
            next_phase = GamePhase.INVENTORY_MENU
        except UnableToEquipException:
            next_phase = GamePhase.PLAYERS_TURN
        except Exception as e:
            raise e
            # next_phase = GamePhase.PLAYERS_TURN

        # Return outcome
        outcome = {
            "next_state": next_phase,
            'messages': messages,
        }

        return outcome


class EquipItemAction(Action):

    def __init__(self, **kwargs):
        pass

    def _execute(self):

        messages = list()

        try:

            item_to_equip = self.game_state.selected_inventory_item

            if not item_to_equip.item.can_perform_action('equip'):
                return {'messages': messages}

            # (try to) equip the item
            messages.extend(self.player.inventory.equip(
                item_to_equip))

            # Reset selection
            self.game_state.selected_inventory_item = None

            # Change game phase (enemies' turn)
            # next_phase = GamePhase.ENEMY_TURN
            # TODO changing equipment should require some time, but keep the
            # menu open
            next_phase = GamePhase.INVENTORY_MENU
        except UnableToEquipException:
            next_phase = GamePhase.PLAYERS_TURN
        except Exception as e:
            raise e
            # next_phase = GamePhase.PLAYERS_TURN


        # Return outcome
        outcome = {
            "next_state": next_phase,
            'messages': messages,
        }

        return outcome


class DropItemAction(Action):

    def __init__(self, **kwargs):
        pass

    def _execute(self):

        messages = list()

        try:

            item_to_equip = self.game_state.selected_inventory_item

            # (try to) add the item to the player's inventory
            messages.append(self.player.inventory.drop(
                item_to_equip, self.game_map))

            # Reset selection
            self.game_state.selected_inventory_item = None

            # Change game phase (enemies' turn)
            next_phase = GamePhase.ENEMY_TURN
        except Exception as e:
            raise e
            # next_phase = GamePhase.PLAYERS_TURN


        # Return outcome
        outcome = {
            "next_state": next_phase,
            'messages': messages,
        }

        # TODO check terrain/enemies!!!

        return outcome

class PickupAction(Action):

*/
