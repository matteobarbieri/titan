#include <string>
#include <sstream>

#include "items.hpp"

#include "../GamePhase.hpp"
#include "../Entity.hpp"

#include "../components/Inventory.hpp"
#include "../components/Equipment.hpp"
#include "../components/Item.hpp"

//#include "../GameMessages.hpp"
#include "../GameState.hpp"
#include "../EquipmentSlots.hpp"

#include "../map/GameMap.hpp"

#include "Outcome.hpp"

Outcome * ItemEquipToggleAction::_execute()
{
    if (game_state->selected_inventory_item->item->equipped)
    {
        return unequip();
    }
    else
    {
        return equip();
    }
}

Outcome * DropItemAction::_execute()
{

    GamePhase next_phase;

    // Build message
    std::ostringstream stringStream;

    // Remove from equipment slots before unequipping
    if (game_state->selected_inventory_item->item->equipped)
    {
        player->equipment->unequip(
            game_state->selected_inventory_item);
    }

    player->inventory->drop(
        game_state->selected_inventory_item, game_map, player);

    next_phase = ENEMY_TURN;

    stringStream << "You drop a " << 
        game_state->selected_inventory_item->name << ".";

    // Add message to message log
    MessageLog::singleton().add_message(
        {stringStream.str(), TCODColor::white});

    // Return outcome
    // TODO enable message log
    Outcome * outcome = new Outcome(
        next_phase,
        false,
        false);

    return outcome;

}

Outcome * ItemEquipToggleAction::unequip()
{

    GamePhase next_phase;

    // Build message
    std::ostringstream stringStream;

    player->equipment->unequip(
        game_state->selected_inventory_item);

    next_phase = ENEMY_TURN;

    stringStream << "You unequip a " << 
        game_state->selected_inventory_item->name << ".";

    // Add message to message log
    MessageLog::singleton().add_message(
        {stringStream.str(), TCODColor::white});

    // Return outcome
    // TODO enable message log
    Outcome * outcome = new Outcome(
        next_phase,
        false,
        false);

    return outcome;

}

Outcome * ItemEquipToggleAction::equip()
{

    GamePhase next_phase;

    // Build message
    std::ostringstream stringStream;

    try
    {
        EquipmentSlot slot = player->equipment->equip(
            game_state->selected_inventory_item);

        if (slot != EquipmentSlot::NONE)
        {
            next_phase = ENEMY_TURN;

            stringStream << "You equip a " << 
                game_state->selected_inventory_item->name << ".";

            // Add message to message log
            MessageLog::singleton().add_message(
                {stringStream.str(), TCODColor::white});
        }
        else
        {
            next_phase = PLAYERS_TURN;
        }

    }
    catch (const UnableToEquipException& e) 
    {
        next_phase = PLAYERS_TURN;
    }

    /*
    */

    // Return outcome
    // TODO enable message log
    Outcome * outcome = new Outcome(
        next_phase, // TODO this is the WRONG one (for debug purposes)
        false,
        false);

    return outcome;

}

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
            MessageLog::singleton().add_message(
                {stringStream.str(), TCODColor::white});

        }
        catch (const InventoryFullException& e) 
        {
            // Build message
            std::ostringstream stringStream;
            stringStream << "Unable to pick up " << item_on_floor->name << ": inventory full.";

            // Add message to message log
            MessageLog::singleton().add_message(
                {stringStream.str(), TCODColor::yellow});

            next_phase = PLAYERS_TURN;
        }
    }
    else
    {
        // No item on ground
        // Add message to message log
        MessageLog::singleton().add_message(
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
