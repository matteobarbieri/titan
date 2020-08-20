#include <string>
#include <sstream>

#include "items.hpp"

#include "../GamePhase.hpp"
#include "../Entity.hpp"

#include "../components/Inventory.hpp"
#include "../components/Equipment.hpp"
#include "../components/Item.hpp"
#include "../components/Usable.hpp"
#include "../components/Container.hpp"

//#include "../GameMessages.hpp"
#include "../GameState.hpp"
#include "../EquipmentSlots.hpp"

#include "../map/GameMap.hpp"

#include "Outcome.hpp"

Outcome * ItemEquipToggleAction::_execute()
{
    // First check if it can be equipped at all
    if (game_state->selected_inventory_item->equippable == nullptr)
    {
        return nullptr;
    }

    if (game_state->selected_inventory_item->item->equipped)
    {
        return unequip();
    }
    else
    {
        return equip();
    }
}


/**
 * Use an item from the inventory
 */
Outcome * ItemUseAction::_execute()
{

    Outcome * o;
   
    o = game_state->selected_inventory_item->usable->use();

    return o;

    /*
    GamePhase next_phase;

    // Build message
    std::ostringstream stringStream;

    // Remove from equipment slots before unequipping
    if (game_state->selected_inventory_item->item->equipped)
    {
    }

    // TODO must come from item's usable component
    //next_phase = ENEMY_TURN;
    next_phase = TARGETING;

    // TODO must come from item's usable component
    stringStream << "Select target ";

    // Add message to message log
    MessageLog::singleton().add_message(
        {stringStream.str(), TCODColor::yellow});

    // Return outcome
    // TODO enable message log
    Outcome * outcome = new Outcome(
        next_phase,
        false,
        false);

    return outcome;
    */
}


ItemResolveTargetingAction::ItemResolveTargetingAction(int x, int y) : x(x), y(y)
{
}

    
/**
 * Use an item from the inventory
 */
Outcome * ItemResolveTargetingAction::_execute()
{

    GamePhase next_phase;

    // Build message
    //std::ostringstream stringStream;

    for (int i=0; i<(int)game_state->selected_inventory_item->usable->effects.size(); i++)
    {
        Effect * eff = game_state->selected_inventory_item->usable->effects[i];
        eff->x = x; 
        eff->y = y; 

        eff->apply(player, game_map, game_state);
    }

    // Remove object from player's inventory
    // TODO reduce quantity instead
    if (game_state->selected_inventory_item->usable->is_consumable)
    {
        player->inventory->remove_item(game_state->selected_inventory_item);
    }
    
    next_phase = ENEMY_TURN;

    // Return outcome
    // TODO check if everything needs to be redrawn
    Outcome * outcome = new Outcome(
        next_phase,
        true,
        true);

    return outcome;
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
        game_state->selected_inventory_item, game_map);

    //next_phase = ENEMY_TURN;
    next_phase = INVENTORY_MENU;

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

    //next_phase = ENEMY_TURN;
    next_phase = INVENTORY_MENU;

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
            //next_phase = ENEMY_TURN;
            next_phase = INVENTORY_MENU;

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

Outcome * TransferItemAction::_execute()
{

    // Retrieve item from a container
    Entity * item = game_state->selected_inventory_item;

    if (player->inventory->is_in_inventory(item))
    {
        return _execute_store();
    }
    else
    {
        return _execute_retrieve();
    }
}

Outcome * TransferItemAction::_execute_retrieve()
{

    // Retrieve item from a container
    Entity * item = game_state->selected_inventory_item;

    GamePhase next_phase = CONTAINER_MENU;

    try
    {
        player->inventory->retrieve(item, game_state->entity_interacted->container);
        
        // Build message
        std::ostringstream stringStream;
        stringStream << "You retrieve a " << item->name << ".";

        // Add message to message log
        MessageLog::singleton().add_message(
            {stringStream.str(), TCODColor::white});

    }
    catch (const InventoryFullException& e) 
    {
        // Build message
        std::ostringstream stringStream;
        stringStream << "Unable to retrieve " << item->name << ": inventory full.";

        // Add message to message log
        MessageLog::singleton().add_message(
            {stringStream.str(), TCODColor::yellow});

        next_phase = CONTAINER_MENU;
    }
     
    // Return outcome
    Outcome * outcome = new Outcome(
        next_phase,
        true,
        true);

    return outcome;
}

Outcome * TransferItemAction::_execute_store()
{

    // Retrieve item from a container
    Entity * item = game_state->selected_inventory_item;

    GamePhase next_phase = CONTAINER_MENU;

    try
    {
        player->inventory->store(item, game_state->entity_interacted->container);
        
        // Build message
        std::ostringstream stringStream;
        stringStream << "You store a " << item->name << ".";

        // Add message to message log
        MessageLog::singleton().add_message(
            {stringStream.str(), TCODColor::white});

    }
    // TODO change this in case containers have a limited capacity
    catch (const InventoryFullException& e) 
    {
        // Build message
        std::ostringstream stringStream;
        stringStream << "Unable to store " << item->name << ": inventory full.";

        // Add message to message log
        MessageLog::singleton().add_message(
            {stringStream.str(), TCODColor::yellow});

        next_phase = PLAYERS_TURN;
    }
     
    // Return outcome
    Outcome * outcome = new Outcome(
        next_phase,
        true,
        true);

    return outcome;
}

/////////////////////////////////
///// SELECT CONTAINER ITEM /////
/////////////////////////////////

SelectContainerItemAction::SelectContainerItemAction(char item_letter) :
    item_letter(item_letter)
{
}

Outcome * SelectContainerItemAction::_execute()
{
    Entity * aux;
    bool found = false;

    // First look in the container's list of stored items
    for (int i=0; i<(int)game_state->entity_interacted->container->items.size(); i++)
    {
        // shortcut to entity
        aux = game_state->entity_interacted->container->items[i];

        if (aux->item->item_letter == item_letter)
        {
            game_state->selected_inventory_item = aux;
            found = true;
            break;
        }
    }

    // Then check in player's inventory
    for (int i=0; i<(int)player->inventory->items.size(); i++)
    {
        // shortcut to entity
        aux = player->inventory->items[i];

        if (aux->item->item_letter == item_letter)
        {
            game_state->selected_inventory_item = aux;
            found = true;
            break;
        }
    }

    // TODO maybe also check in equipped items?


    if (found)
    {
        Outcome * outcome = new Outcome(
            CONTAINER_ITEM_MENU, false, false);

        return outcome;
    }
    else
    {
        return nullptr;
    }

}

