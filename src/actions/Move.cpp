#include "Move.hpp"

#include "../GamePhase.hpp"
#include "../Entity.hpp"
#include "../GameMessages.hpp"
#include "../GameState.hpp"

#include "../map/GameMap.hpp"

#include "Outcome.hpp"

// TODO move somewhere else

Entity * get_blocking_entities_at_location(
        std::vector<Entity *> entities, int destination_x, int destination_y)
{
    for (int i=0; i<(int)entities.size(); i++)
    {
        if (entities[i]->blocks() &&
            entities[i]->x == destination_x &&
            entities[i]->y == destination_y)

            return entities[i];
    }

    return 0;
}

MoveAction::MoveAction(int dx, int dy) :
    Action(), dx(dx), dy(dy)
{
}

Outcome * MoveAction::_execute()
{

        // Save the original coordinates of the player
        int source_x = player->x;
        int source_y = player->y;

        // Compute destination coordinates
        int destination_x = player->x + dx;
        int destination_y = player->y + dy;

        // TODO enable messages
        //messages = list()

        // If it is not blocked, do something, either move to a new location,
        // attack an enemy or interact with an entity
        if (! game_map->is_blocked(destination_x, destination_y))
        {
            Entity * target = get_blocking_entities_at_location(
                game_map->entities(), destination_x, destination_y);

            if (target != 0)
            {
                player->interact_with(target);
            }
            else
            {
                // Update player's position
                player->x = destination_x;
                player->y = destination_y;
            }
        }

        else

        {
            // Stupid things said when bumping into walls
            // TODO enable message log
            //possible_messages = ['Ouch!', 'Hey!', 'Stop it!']
            //messages.append(
                //Message(random.choice(possible_messages), libtcod.yellow))

            //game_state->message_log->add_message(
                //Message("Ouch!", TCODColor::yellow));
            
            // TODO replace with a random one
            game_state->message_log->add_message({"Ouch!", TCODColor::yellow});
        }

        // Check if the position has changed
        bool position_changed = (
            source_x != player->x or source_y != player->y);

        // Return outcome
        // TODO enable message log
        Outcome * outcome = new Outcome(
            ENEMY_TURN, // TODO this is the right one
            //PLAYERS_TURN, // TODO this is the WRONG one (for debug purposes)
            position_changed,
            position_changed);

        //# TODO check terrain/enemies!!!

        return outcome;

}
