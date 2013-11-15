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
    void load(const int offset, const int count);
    int offset() const;
    int count() const;

private:
    Connection *_connection;
    int _offset;
    int _count;

private slots:
    void loadFinished(const Packet *sender, const QVariantMap &response);

signals:
    void dialogs(const DialogsPacket *sender, const MessageList *dialogs);
};

#endif // DIALOGSPACKET_H
