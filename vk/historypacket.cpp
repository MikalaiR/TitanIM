#include "historypacket.h"

HistoryPacket::HistoryPacket(Connection *connection)
{
    _connection = connection;
}

void HistoryPacket::load(const int id, const int offset, const int count)
{
    Packet *packet = new Packet("messages.getHistory");
    packet->addParam("offset", offset);
    packet->addParam("count", count);
    packet->addParam("user_id", id);
    packet->setId(id);
    connect(packet, SIGNAL(finished(const Packet*,QVariantMap)), this, SLOT(loadFinished(const Packet*,QVariantMap)));
    _connection->appendQuery(packet);
}

void HistoryPacket::loadFinished(const Packet *sender, const QVariantMap &result)
{
    QVariantMap response = result.value("response").toMap();
    MessageList messageList = MessageParser::parser(response.value("items").toList());

    emit history(this, sender->id(), messageList);
    delete sender;
}
