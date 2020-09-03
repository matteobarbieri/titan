#ifndef ROGUE_2077_AI
#define ROGUE_2077_AI

#include "../nlohmann/json.hpp"

using json = nlohmann::json;

// Forward declarations
class Entity;
class GameMap;
class Outcome;

///////////////////////////////////
//////////// AI STATE /////////////
///////////////////////////////////

enum class AIState {
    IDLE,
    SHOOTING,
    HUNTING
};

///////////////////////////////////
//////////// AI ACTION ////////////
///////////////////////////////////

// TODO move somewhere else
class AIAction
{
    protected:

        virtual void _execute();

    public:

        Entity * monster;
        Entity * player;
        GameMap * game_map;

        AIAction(Entity * monster, Entity * player, GameMap * game_map);
        void execute();
    
};

///////////////////////////////////
///// ATTACK PLAYER AI ACTION /////
///////////////////////////////////

class AttackPlayerAIAction : public AIAction
{
    protected:

        void _execute();

    public:

        AttackPlayerAIAction(Entity * monster, Entity * player, GameMap * game_map);
};

///////////////////////////////////
///// ATTACK PLAYER AI ACTION /////
///////////////////////////////////

class ReloadWeaponAIAction : public AIAction
{
    protected:

        void _execute();

    public:

        Entity * weapon;

        ReloadWeaponAIAction(Entity * monster, Entity * weapon);
};

///////////////////////////////////
///// MOVE TOWARDS AI ACTION //////
///////////////////////////////////

class MoveTowardsAIAction : public AIAction
{
    protected:

        void _execute();

    public:

        int target_x;
        int target_y;

        MoveTowardsAIAction(Entity * monster, Entity * player, GameMap * game_map, int, int);
};

///////////////////////////////////
/////////// MONSTER AI ////////////
///////////////////////////////////

class MonsterAi
{

    public:

        AIState state;

        Entity * owner;

        virtual AIAction * pick_action(Entity * player, GameMap * game_map) = 0;

        MonsterAi();

        /**
         * Creates a json representation of the component
         */
        virtual json to_json();

        static MonsterAi * from_json(json j);
};

///////////////////////////////////
////////// IMMOBILE AI ////////////
///////////////////////////////////

class ImmobileAi : public MonsterAi
{

    public:

        AIAction * pick_action(Entity * player, GameMap * game_map);

        ImmobileAi();

        /**
         * Creates a json representation of the component
         */
        json to_json();

        static ImmobileAi * from_json(json j);
};
//
///////////////////////////////////
//////////// SEEKER AI ////////////
///////////////////////////////////

/**
 * A monster which will try and get in melee range of the player as soon as he
 * sees him.
 */
class SeekerAi : public MonsterAi
{
    public:

        int player_last_seen_x;
        int player_last_seen_y;

        AIAction * pick_action(Entity * player, GameMap * game_map) override;

        SeekerAi();

        /**
         * Creates a json representation of the component
         */
        json to_json() override;

        static SeekerAi * from_json(json j);
};

///////////////////////////////////
//////////// SEEKER AI ////////////
///////////////////////////////////

/**
 * A monster which will try and get in melee range of the player as soon as he
 * sees him.
 */
class RangedAi : public MonsterAi
{
    public:

        int player_last_seen_x;
        int player_last_seen_y;

        AIAction * pick_action(Entity * player, GameMap * game_map) override;

        RangedAi();

        int getRange();
        Entity * mustReload();

        /**
         * Creates a json representation of the component
         */
        json to_json() override;

        static RangedAi * from_json(json j);
};

#endif /* ROGUE_2077_AI */
