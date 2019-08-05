#ifndef ROGUE_20177_GAME_MESSAGES
#define ROGUE_20177_GAME_MESSAGES

#include <string>
#include <vector>

#include "libtcod.hpp"

/**
 * A message
 */
typedef struct {
    std::string text;
    TCODColor color = TCODColor::white;
} Message;

class MessageLog
{
    public:

        int n_visible_messages;

        std::vector<Message> messages;

        MessageLog(int);

        /**
         * Add a message to the log
         */
        void add_message(Message m);

        /**
         * Show the last N messages
         */
        std::vector<Message> visible_messages();
};

#endif /* ifndef ROGUE_20177_GAME_MESSAGES */
