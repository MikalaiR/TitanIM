#ifndef DIALOGSPACKET_H
#define DIALOGSPACKET_H

#include <QObject>
#include "connection.h"
#include "profileparser.h"
#include "messageparser.h"

class DialogsPacket : public QObject
{
    Q_OBJECT

public:
    DialogsPacket(Connection *connection);
    void load(const int offset=0, const int count=20);

private:
    Connection *_connection;

private slots:
    void loadFinished(const Packet *sender, const QVariantMap &response);

signals:
    void dialogs(const MessageList *dialogs);
};

#endif // DIALOGSPACKET_H
