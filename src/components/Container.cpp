//#include <algorithm>

#include "Container.hpp"

#include "../Entity.hpp"
#include "../map/GameMap.hpp"

#include "Item.hpp"
#include "Inventory.hpp"

Container::Container()
{
}

/*
 *
 *int Container::get_item_position_in_list(Entity * item)
 *{
 *
 *    // TODO must change, doesn't work well because of equipped status
 *    for (int i=0; i<(int)items.size(); i++)
 *    {
 *        if (items[i] == item)
 *        {
 *            return i;
 *        }
 *    }
 *
 *    DEBUG("Container::get_item_position_in_list should not arrive here...");
 *
 *    // Should really never arrive here
 *    throw new std::exception();
 *
 *}
 *
 */

void Container::get(Entity * item)
{
    // Mark item as unequipped
    item->item->equipped = false;

    // Actually remove item from vector of pointers
    remove_item(item);
}

void Container::remove_item(Entity * item)
{
    // Solution taken from
    // https://stackoverflow.com/questions/3385229/c-erase-vector-element-by-value-rather-than-by-position
    items.erase(
        std::remove(items.begin(), items.end(), item),
        items.end());
}

void Container::put(Entity * item)
{

    // Set item's coordinates to invalid values, since they do not make any
    // sense while the item is in player's inventory
    item->x = -1;
    item->y = -1;

    // Actually add the item to the inventory

    // Mark item as unequipped
    item->item->equipped = false;

    // Add to the vector of items
    items.push_back(item);
}

void Container::refresh_items_letters(Inventory * inventory)
{
    for (int i=0; i<(int)items.size(); i++)
    {
        items[i]->item->item_letter =
            inventory->available_letters[i];
    }
}

json Container::to_json()
{
    json j;

    // Items
    json j_items;

    for (int i=0; i<(int)items.size(); i++)
    {
        j_items.push_back(items[i]->to_json());
    }

    j["items"] = j_items;

    return j;
}

Container * Container::from_json(json j)
{

    // Initialize inventory component WITHOUT also initializing the list of
    // available letters (must be restored from json object).
    Container * c = new Container();

    // Items
    for (int i=0; i<(int)j["items"].size(); i++)
    {
        c->items.push_back(Entity::from_json(j["items"][i]));
    }
    
    return c;
}
