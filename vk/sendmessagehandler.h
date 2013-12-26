#ifndef SENDMESSAGEHANDLER_H
#define SENDMESSAGEHANDLER_H

#include <QObject>
#include <QQueue>
#include "connection.h"
#include "messageitem.h"

class SendMessageHandler : public QObject
{
    Q_OBJECT

public:
    explicit SendMessageHandler(Connection *connection);
    void send(MessageItem message);

private:
    Connection *_connection;
    QQueue<MessageItem> _messageQuery;
    bool _isProcessing;

private:
    void execSendMessageQuery();
    void sendMessage();

private slots:
    void sendMessageFinished(const Packet *sender, const QVariantMap &result);

signals:
    void sending();
    void successfullyMessageSent(const int internalMid, const int serverMid);
};

#endif // SENDMESSAGEHANDLER_H
