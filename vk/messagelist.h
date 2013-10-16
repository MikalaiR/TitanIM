#ifndef MESSAGELIST_H
#define MESSAGELIST_H

#include <QObject>
#include <QMap>
#include "messageitem.h"

class MessageList : public QObject
{
    Q_OBJECT

public:
    MessageList();
    void add(MessageItem *message);
    void add(MessageList *messages);
    MessageItem* at(const int mid) const;
    int count() const;

private:
    QMap<int, MessageItem*> _messages;
};

#endif // MESSAGELIST_H
