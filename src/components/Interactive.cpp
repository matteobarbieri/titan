#include "../libtcod.hpp"

#include "Interactive.hpp"

#include "../Entity.hpp"
//#include "../GamePhase.hpp"
#include "../GameState.hpp"
#include "../map/GameMap.hpp"

#include "../components/Inventory.hpp"
#include "../components/Item.hpp"

#include "../GameMessages.hpp"

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

TerminalFunction::TerminalFunction(
        std::string command, 
        int command_shortcut, 
        void (*execute)(Entity *, GameMap *, GameState *),
        bool enabled) :
    command(command), command_shortcut(command_shortcut), execute(execute),
    enabled(enabled)
{
}

InteractiveTerminal::InteractiveTerminal(bool is_active) : is_active(is_active)
{
}

void InteractiveTerminal::interact(Entity * player, GameMap * game_map, GameState * game_state)
{

    // First check if the player is approaching the panel from the right
    // direction
    if (is_active)
    {
        // Set current terminal as entity with which to interact
        game_state->entity_interacted = owner;
        game_state->game_phase = TERMINAL_MENU;

        //void fff() = terminal_options[0];


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
