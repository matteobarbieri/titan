//#include <algorithm>

#include "Container.hpp"

#include "../Entity.hpp"
#include "../map/GameMap.hpp"

#include "Item.hpp"

Container::Container(bool init_available_letters)
{

        // The list of all letters available for an item
        if (init_available_letters)
        {
            // Skip the 'i'
            for (int i='a'; i<'h'; i++)
            {
                available_letters.push_back(i);
            }
            
            for (int i='j'; i<='z'; i++)
            {
                available_letters.push_back(i);
            }
        }

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

    // Add item letter back to the pool of available letters
    available_letters.push_back(item->item->item_letter);

    // Reset item letter assigned to item
    item->item->item_letter = -1;

    // Sort available item letters
    std::sort(available_letters.begin(), available_letters.end());
    
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

    // Actually add the item to the inventory, Associating it with the
    // first available letter

    // "Manual" pop(0)
    char item_letter = available_letters[0];
    available_letters.erase(available_letters.begin());

    // Mark item as unequipped
    item->item->equipped = false;

    // Assign item letter to item component
    item->item->item_letter = item_letter;

    //DEBUG("Assigned letter " << item_letter);

    // Add to the vector of items
    items.push_back(item);
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

    // Available letters
    json j_available_letters;

    for (int i=0; i<(int)available_letters.size(); i++)
    {
        j_available_letters.push_back(available_letters[i]);
    }

    j["available_letters"] = j_available_letters;

    return j;
}

Container * Container::from_json(json j)
{

    // Initialize inventory component WITHOUT also initializing the list of
    // available letters (must be restored from json object).
    Container * c = new Container(false);

    // Items
    for (int i=0; i<(int)j["items"].size(); i++)
    {
        c->items.push_back(Entity::from_json(j["items"][i]));
    }
    
    // Items letters
    for (int i=0; i<(int)j["available_letters"].size(); i++)
    {
        c->available_letters.push_back((int)j["available_letters"][i]);
    }

    return c;
}
