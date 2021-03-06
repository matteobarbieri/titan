#ifndef ROGUE_20177_ACTIONS_MENU
#define ROGUE_20177_ACTIONS_MENU

#include "Action.hpp"
#include "../GamePhase.hpp"


// Forward declarations
class Outcome;

class ShowMenuAction : public Action
{

    public:

        // Constructors
        //ShowMenuAction();
        //~ShowMenuAction();

        Outcome * _execute();

};

class ShowCharacterScreenAction : public Action
{

    public:

        Outcome * _execute();

};

class ShowInventoryAction : public Action
{

    public:

        Outcome * _execute();

};

class SelectInventoryItemAction : public Action
{

    public:

        char item_letter;

        SelectInventoryItemAction(char item_letter);

        Outcome * _execute();

};

class SelectTerminalFunctionAction : public Action
{

    public:

        char command_shortcut;

        SelectTerminalFunctionAction(char command_shortcut);

        Outcome * _execute();

};



class BackToInventoryMenuAction : public Action
{

    public:

        Outcome * _execute();

};

/**
 * Simply reset the state to player's turn
 */
class GoToPhaseAction : public Action
{

    public:

        GamePhase next_phase;

        Outcome * _execute();

        GoToPhaseAction(GamePhase);

};

/**
 * Simply reset the state to player's turn
 */
class BackToGameAction : public Action
{

    public:

        Outcome * _execute();

};

#endif /* ifndef ROGUE_20177_ACTIONS_MENU */
