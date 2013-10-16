#include "messagelist.h"

MessageList::MessageList()
{
}

void MessageList::add(MessageItem *message)
{
    _messages[message->mid()] = message;
}

void MessageList::add(MessageList *messages)
{
//    _messages << messages; //todo
}

MessageItem *MessageList::at(const int mid) const
{
    return _messages.value(mid);
}

int MessageList::count() const
{
    return _messages.count();
}
