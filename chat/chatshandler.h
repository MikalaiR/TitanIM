#ifndef CHATSHANDLER_H
#define CHATSHANDLER_H

#include <QObject>
#include <QMap>
#include "chat.h"
#include "vk/client.h"
#include "vk/historypacket.h"

class ChatsHandler : public QObject
{
    Q_OBJECT

public:
    ChatsHandler();
    ~ChatsHandler();
    void create(const DialogItem dialog);
    bool contains(const int id) const;
    Chat* chat(const int id) const;
    void clear();

private:
    QMap<int, Chat*> _chats;
    HistoryPacket *_historyPacket;

protected slots:
    void onHistoryLoaded(const HistoryPacket *sender, const int id, const MessageList &messages);
};

#endif // CHATSHANDLER_H
