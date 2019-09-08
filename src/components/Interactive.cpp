#include "../libtcod.hpp"

#include "Interactive.hpp"

#include "../Entity.hpp"
#include "../map/GameMap.hpp"

#include "../components/Inventory.hpp"
#include "../components/Item.hpp"

#include "../GameMessages.hpp"

InteractiveDoor::InteractiveDoor(bool locked, unsigned int key_id) :
    locked(locked), key_id(key_id)
{
}

void InteractiveDoor::unlock(GameMap * game_map)
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

void InteractiveDoor::interact(Entity * player, GameMap * game_map)
{

    if (!locked)
    {
        unlock(game_map);
    }
    else
    {
        // Door is locked, look for an appropriate key in player's inventory
        if (key_id > 0)
        {
            if (player_has_key(player))
            {
                unlock(game_map);
                MessageLog::singleton().add_message(
                    {"You unlock the door",
                    TCODColor::white});
            }
            else
            {
                MessageLog::singleton().add_message(
                    {"You don't have the right key to unlock this door",
                    TCODColor::white});
            }
        }
        else
        {
            MessageLog::singleton().add_message({"This door appears to be locked!", TCODColor::white});
        }
    }

}
