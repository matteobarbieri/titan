#include "GameMessages.hpp"

#include <algorithm>

MessageLog::MessageLog(int n_visible_messages) :
    n_visible_messages(n_visible_messages)
{
}

void MessageLog::add_message(Message m)
{
    messages.push_back(m);
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
