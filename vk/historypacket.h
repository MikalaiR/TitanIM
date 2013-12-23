#ifndef HISTORYPACKET_H
#define HISTORYPACKET_H

#include <QObject>
#include "connection.h"
#include "messageparser.h"

class HistoryPacket : public QObject
{
    Q_OBJECT

public:
    explicit HistoryPacket(Connection *connection);
    void load(const int id, const int offset=0, const int count=20);

private:
    Connection *_connection;

private slots:
    void loadFinished(const Packet *sender, const QVariantMap &result);

signals:
    void history(const HistoryPacket *sender, const int id, const MessageList &messages);
};

#endif // HISTORYPACKET_H
