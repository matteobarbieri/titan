#include "Constants.h"
#include "GameMessages.hpp"

#include <algorithm>

// Private constructor/destructor
MessageLog::MessageLog() :
    n_visible_messages(MESSAGE_LOG_N_ROWS)
{
}

MessageLog::~MessageLog()
{
}

void MessageLog::add_message(Message m)
{
    messages.push_back(m);
}

/*! The static method to access the singleton */
MessageLog & MessageLog::singleton()
{
    static MessageLog instance;

    return instance;
}

// TODO improve this one
std::vector<Message> MessageLog::visible_messages()
{
    std::vector<Message> vm;
    
    int n_messages = messages.size();
    int start = std::max(n_messages - n_visible_messages, 0);
    for (int i=start; i<n_messages; i++)
    {
        vm.push_back(messages[i]);
    }

    return vm;
}
