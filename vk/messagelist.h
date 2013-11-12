#ifndef MESSAGELIST_H
#define MESSAGELIST_H

#include <QObject>
#include <QVector>
#include "messageitem.h"

class MessageList : public QObject
{
    Q_OBJECT

public:
    MessageList();
    void add(MessageItem *message);
    void add(const MessageList *messages);
    int indexOf(const int mid) const;
    MessageItem* at(const int i) const;
    MessageItem* item(const int mid) const;
    void remove(const int i);
    int count() const;
    QVector<MessageItem*> toVector() const;

private:
    QVector<MessageItem*> _messages;
};

#endif // MESSAGELIST_H
