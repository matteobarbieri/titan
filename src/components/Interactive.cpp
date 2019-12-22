#include "../libtcod.hpp"

#include "Interactive.hpp"

#include "../Entity.hpp"
//#include "../GamePhase.hpp"
#include "../GameState.hpp"
#include "../map/GameMap.hpp"

#include "../components/Inventory.hpp"
#include "../components/Item.hpp"
#include "../components/Container.hpp"

#include "../GameMessages.hpp"

#include "../SaveGame.hpp"

#include "../prefabs/funcs/misc.hpp"

//////////////////////////////////
////////// INTERACTIVE ///////////
//////////////////////////////////

Interactive * Interactive::from_json(json j)
{
    if (j["subclass"] == "InteractiveDoor")
    {
        return InteractiveDoor::from_json(j);
    }

    if (j["subclass"] == "InteractivePanel")
    {
        return InteractivePanel::from_json(j);
    }

    if (j["subclass"] == "InteractiveTerminal")
    {
        return InteractiveTerminal::from_json(j);
    }

    if (j["subclass"] == "InteractiveContainer")
    {
        return InteractiveContainer::from_json(j);
    }

    return nullptr;
}

//////////////////////////////////
//////// INTERACTIVE DOOR ////////
//////////////////////////////////

InteractiveDoor::InteractiveDoor(bool is_open, bool locked, unsigned int key_id) :
    is_open(is_open), locked(locked), key_id(key_id)
{
}

//void InteractiveDoor::unlock(GameMap * game_map)
void InteractiveDoor::unlock()
{
    locked = false;
}

void InteractiveDoor::open(GameMap * game_map)
{
        owner->blocks_sight(false);
        owner->blocks(false);

        owner->symbol = '-';
        game_map->update_fov_map_properties(
            owner->x, owner->y, true, true);
}

bool InteractiveDoor::player_has_key(Entity * player)
{

    // Go through every player's items and check if any of them has a matching
    // key_id
    for (int i=0; i<(int)player->inventory->items.size(); i++)
    {
        // Shortcut
        Item * item_component = player->inventory->items[i]->item;
        if (item_component != nullptr && item_component->key_id == key_id)
        {
            return true;
        }
    }

    return false;
}

void InteractiveDoor::interact(Entity * player, GameMap * game_map, GameState * game_state)
{

    if (!locked)
    {
        open(game_map);
    }
    else
    {
        // Door is locked, look for an appropriate key in player's inventory
        if (key_id > 0)
        {
            if (player_has_key(player))
            {
                //unlock(game_map);
                unlock();
                MessageLog::singleton().add_message(
                    {"You unlock the door",
                    TCODColor::white});
            }
            else
            {
                MessageLog::singleton().add_message(
                    {"This door appears to be locked.",
                    TCODColor::white});
            }
        }
        else
        {
            MessageLog::singleton().add_message({"This door appears to be locked!", TCODColor::white});
        }
    }

    game_state->game_phase = ENEMY_TURN;
}

json InteractiveDoor::to_json()
{
    json json_data;

    json_data["subclass"] = "InteractiveDoor";
    json_data["is_open"] = is_open;
    json_data["locked"] = locked;
    json_data["key_id"] = key_id;

    return json_data;
}

InteractiveDoor * InteractiveDoor::from_json(json j)
{
    InteractiveDoor * c = new InteractiveDoor(
        j["is_open"],
        j["locked"],
        j["key_id"]
    );

    return c;
}


//////////////////////////////////
/////// INTERACTIVE PANEL ////////
//////////////////////////////////

InteractivePanel::InteractivePanel(
        std::string text, TCODColor text_color, Direction * readable_from, bool is_active) :
    text(text), text_color(text_color), readable_from(readable_from), is_active(is_active)
{
}

void InteractivePanel::interact(Entity * player, GameMap * game_map, GameState * game_state)
{

    // First check if the player is approaching the panel from the right
    // direction
    if ((owner->x + readable_from->dx) == player->x &&
        (owner->y + readable_from->dy) == player->y)
    {
        if (is_active)
        {
            MessageLog::singleton().add_message(
                {text, text_color});
        }
        else
        {
            MessageLog::singleton().add_message(
                {"This panel does not seem to be active...",
                 TCODColor::amber});
        }
    }

}

json InteractivePanel::to_json()
{
    json j;

    j["subclass"] = "InteractivePanel";
    j["text"] = text;
    j["text_color"] = tcodcolor_to_json(text_color);
    j["readable_from"] = readable_from->to_json();
    j["is_active"] = is_active;

    return j;
}

InteractivePanel * InteractivePanel::from_json(json j)
{

    InteractivePanel * ip = new InteractivePanel(
        j["text"],
        json_to_tcodcolor(j["text_color"]),
        Direction::from_json(j["readable_from"]),
        j["is_active"]
    );

    return ip;
}

//////////////////////////////////
////// INTERACTIVE TERMINAL //////
//////////////////////////////////

TerminalFunction::TerminalFunction(
        std::string command, 
        int command_shortcut, 
        bool enabled) :
    command(command), command_shortcut(command_shortcut),
    enabled(enabled)
{
}

json TerminalFunction::to_json()
{
    json j;

    j["command"] = command;
    j["command_shortcut"] = command_shortcut;
    j["enabled"] = enabled;

    json j_effects;

    for (int i=0; i<(int)effects.size(); i++)
    {
        j_effects.push_back(effects[i]->to_json());
    }

    j["effects"] = j_effects;

    return j;
}

TerminalFunction * TerminalFunction::from_json(json j)
{
    TerminalFunction * tf = new TerminalFunction(
        j["command"],
        j["command_shortcut"],
        j["enabled"]
    );

    // Restore effects
    for (int i=0; i<(int)j["effects"].size(); i++)
    {
        tf->effects.push_back(
            Effect::from_json(j["effects"][i]));
    }

    return tf;
}

InteractiveTerminal::InteractiveTerminal(bool is_active) : is_active(is_active)
{
}

void InteractiveTerminal::interact(Entity * player, GameMap * game_map, GameState * game_state)
{
    if (is_active)
    {
        // Set current terminal as entity with which to interact
        game_state->entity_interacted = owner;
        game_state->game_phase = TERMINAL_MENU;

        MessageLog::singleton().add_message(
            {"[PH] Interacting with terminal", TCODColor::lightGreen});
    }
    else
    {
        MessageLog::singleton().add_message(
            {"This terminal does not seem to be active...",
             TCODColor::amber});
    }

}

void TerminalFunction::execute(Entity * player, GameMap * game_map, GameState * game_state)
{
    for (int i=0; i<(int)effects.size(); i++)
    {
        effects[i]->apply(player, game_map, game_state);
    }
}

json InteractiveTerminal::to_json()
{
    json j;

    j["subclass"] = "InteractiveTerminal";

    j["is_active"] = is_active;

    json jtf;
    for (int i=0; i<(int)terminal_functions.size(); i++)
    {
        jtf.push_back(terminal_functions[i]->to_json());
    }

    j["terminal_functions"] = jtf;

    return j;
}

InteractiveTerminal * InteractiveTerminal::from_json(json j)
{

    InteractiveTerminal * it = new InteractiveTerminal(
        j["is_active"]
    );

    for (int i=0; i<(int)j["terminal_functions"].size(); i++)
    {
        it->terminal_functions.push_back(
            TerminalFunction::from_json(j["terminal_functions"][i]));
    }

    return it;
}


//////////////////////////////////
////// INTERACTIVE CONTAINER /////
//////////////////////////////////

InteractiveContainer::InteractiveContainer(bool locked, unsigned int key_id) :
    locked(locked), key_id(key_id)
{
}

void InteractiveContainer::interact(Entity * player, GameMap * game_map, GameState * game_state)
{

    if (!locked)
    {
        // Set the container as entity with which to interact
        game_state->entity_interacted = owner;
        game_state->game_phase = CONTAINER_MENU;

        // Assign letters to items inside the container based on player's
        // current inventory.
        for (int i=0; i<(int)owner->container->items.size(); i++)
        {
            owner->container->items[i]->item->item_letter =
                player->inventory->available_letters[i];
        }

        MessageLog::singleton().add_message(
            {"[PH] Interacting with container", TCODColor::lightGreen});
    }
    else
    // TODO actually handle lock and key mechanism
    {
        // TODO improve message with actual container's name
        MessageLog::singleton().add_message(
            {"This terminal does not seem to be active...",
             TCODColor::amber});
    }

}

json InteractiveContainer::to_json()
{
    json j;

    j["subclass"] = "InteractiveContainer";

    j["locked"] = locked;
    j["key_id"] = key_id;

    return j;
}

InteractiveContainer * InteractiveContainer::from_json(json j)
{

    InteractiveContainer * ic = new InteractiveContainer(
        j["locked"],
        j["key_id"]
    );

    return ic;
}
