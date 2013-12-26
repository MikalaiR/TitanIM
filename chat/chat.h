#ifndef CHAT_H
#define CHAT_H

#include <QObject>
#include <QDateTime>
#include "vk/client.h"
#include "vk/dialogitem.h"
#include "vk/sendmessagehandler.h"
#include "chatmodel.h"

class Chat : public QObject
{
    Q_OBJECT

public:
    Chat();
    Chat(const DialogItem dialog);
    ~Chat();
    int id() const;
    bool isGroupChat() const;
    DialogItem dialog() const;
    ChatModel* model() const;

private:
    DialogItem _dialog;
    ChatModel *_model;
    SendMessageHandler *_sendMessageHandler;
    static int _internalMessageId;

public slots:
    void sendMessage(const QString &text);
};

#endif // CHAT_H
