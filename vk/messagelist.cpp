#include "messagelist.h"

MessageList::MessageList()
{
}

void MessageList::add(MessageItem *message)
{
    _messages.append(message);
}

void MessageList::add(const MessageList *messages)
{
    _messages << messages->toVector();
}

int MessageList::indexOf(const int mid) const
{
    for (int i = 0; i < _messages.count(); i++)
    {
        if (_messages.at(i)->mid() == mid)
            return i;
    }
}

MessageItem *MessageList::at(const int i) const
{
    return _messages.at(i);
}

MessageItem *MessageList::item(const int mid) const
{
    foreach (MessageItem *item, _messages)
    {
        if (item->mid() == mid)
            return item;
    }
}

void MessageList::remove(const int i)
{
    _messages.remove(i);
}

int MessageList::count() const
{
    return _messages.count();
}

QVector<MessageItem *> MessageList::toVector() const
{
    return _messages;
}
