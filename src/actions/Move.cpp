#include "Move.hpp"

#include "../utils.hpp"

#include "../GamePhase.hpp"
#include "../Entity.hpp"
#include "../GameMessages.hpp"
#include "../GameState.hpp"

#include "../map/GameMap.hpp"

#include "Outcome.hpp"


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

        bool position_changed = false;
        bool interacted_with_something = false;
        
        //bool res;
        //res = game_map->path_astar->compute(6, 18, 6, 16);
        //DEBUG("Path found: " << res);
        //res = game_map->path_astar->compute(6, 19, 6, 16);
        //DEBUG("Path found: " << res);
        //res = game_map->path_astar->compute(6, 16, 6, 19);
        //DEBUG("Path found: " << res);
        //res = game_map->path_astar->compute(6, 16, 6, 18);
        //DEBUG("Path found: " << res);
        //res = game_map->path_astar->compute(6, 17, 6, 19);
        //DEBUG("Path found: " << res);
        //res = game_map->path_astar->compute(6, 17, 6, 18);
        //DEBUG("Path found: " << res);

        // If it is not blocked, do something, either move to a new location,
        // attack an enemy or interact with an entity
        if (! game_map->is_blocked(destination_x, destination_y))
        {
            Entity * target = get_blocking_entities_at_location(
                game_map->entities(), destination_x, destination_y);

            if (target != nullptr)
            {
                player->interact_with(target, game_map, game_state);
                interacted_with_something = true;
            }
            else
            {

                // Previous tile is now walkable again
                game_map->fov_map->setProperties(
                    player->x, player->y,
                    game_map->fov_map->isTransparent(
                        player->x, player->y),
                    true);

                // Update player's position
                player->x = destination_x;
                player->y = destination_y;

                // New tile is now not walkable
                game_map->fov_map->setProperties(
                    player->x, player->y,
                    game_map->fov_map->isTransparent(
                        player->x, player->y),
                    false);

                // Change phase to enemy turn manually!
                game_state->game_phase = ENEMY_TURN;

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
            MessageLog::singleton().add_message({"Ouch!", TCODColor::yellow});
        }

        // Check if the position has changed
        position_changed = (
            source_x != player->x or source_y != player->y);

        bool redraw_terrain = position_changed || interacted_with_something;
        bool fov_recompute = redraw_terrain;

        // Return outcome
        Outcome * outcome = new Outcome(
            // ENEMY_TURN, // TODO this is the right one
            game_state->game_phase, // TODO this is the right one
            fov_recompute,
            redraw_terrain);

        //# TODO check terrain/enemies!!!

        return outcome;

}


Outcome * WaitAction::_execute()
{

        // Return outcome
        Outcome * outcome = new Outcome(
            ENEMY_TURN, // TODO this is the right one
            true,
            true);

        return outcome;

}
