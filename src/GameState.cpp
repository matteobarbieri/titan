#include "GameState.hpp"

#include "actions/Outcome.hpp"

GameState::GameState()
{
    // TODO make the number of visible message come from constants
    current_turn = 1;
}

bool GameState::is_players_turn()
{
    return 
        game_phase == PLAYERS_TURN ||
        game_phase == ENTITY_INFO ||
        game_phase == CHARACTER_SCREEN ||
        game_phase == TARGETING_ITEM ||
        game_phase == TARGETING_SKILL ||
        game_phase == INVENTORY_ITEM_MENU ||
        game_phase == INVENTORY_MENU ||
        game_phase == TERMINAL_MENU ||
        game_phase == CONTAINER_MENU ||
        game_phase == CONTAINER_ITEM_MENU ||
        game_phase == POPUP_MESSAGE ||
        game_phase == LEVEL_SUMMARY ||
        game_phase == PLAYER_DEAD;
}

bool GameState::is_enemies_turn()
{
    return game_phase == ENEMY_TURN;
}

// TODO does it need also message_log ?
void GameState::update(Outcome * outcome, bool & fov_recompute,
        bool & redraw_terrain)
{

    //DEBUG();
    // Update game phase
    game_phase = outcome->next_phase;

    // Update focused entity
    if (outcome->entity_focused != nullptr)
        entity_focused = outcome->entity_focused;

    // Update targeted entity
    if (outcome->entity_targeted != nullptr)
        entity_targeted = outcome->entity_targeted;

    // Determine whether to recompute fov...
    fov_recompute = outcome->fov_recompute;

    // or redraw terrain
    redraw_terrain = outcome->redraw_terrain;
}


json GameState::to_json()
{
    json j;

    j["game_phase"] = game_phase;
    j["current_turn"] = current_turn;

    return j;
}

GameState * GameState::from_json(json j)
{
    GameState * gs = new GameState();

    gs->game_phase = j["game_phase"];
    gs->current_turn = j["current_turn"];

    return gs;
}
