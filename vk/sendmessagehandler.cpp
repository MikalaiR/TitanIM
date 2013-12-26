#include "sendmessagehandler.h"

SendMessageHandler::SendMessageHandler(Connection *connection)
{
    _connection = connection;
    _isProcessing = false;

}

void SendMessageHandler::send(MessageItem message)
{
    _messageQuery.enqueue(message);

    if (!_isProcessing)
    {
        execSendMessageQuery();
    }
}

void SendMessageHandler::execSendMessageQuery()
{
    if (_messageQuery.count() == 0)
    {
        return;
    }

    _isProcessing = true;

    //todo добавить обработку аттачей
//    MessageItem message = _messageQuery.head();

//    if (message.attachments && message.attachments.count() > 0)
//    {
//        _uploadAttachments.setAttachments(message.attachments);
//        _uploadAttachments.upload();
//        return;
//    }

    sendMessage();
}

void SendMessageHandler::sendMessage()
{
    MessageItem message = _messageQuery.dequeue();

    Packet *packet = new Packet("messages.send");
    packet->addParam("message", message->body());

    if (message->isGroupChat())
    {
        packet->addParam("chat_id", message->chatId());
    }
    else
    {
        packet->addParam("user_id", message->uid());
    }

//    //todo добавить обработку аттачей
//    if (message.attachments)
//    {

//    }

    packet->setId(message->mid());
    connect(packet, SIGNAL(finished(const Packet*,QVariantMap)), this, SLOT(sendMessageFinished(const Packet*,QVariantMap)));
    emit sending();
    _connection->appendQuery(packet);

    _isProcessing = false;
    execSendMessageQuery();
}

void SendMessageHandler::sendMessageFinished(const Packet *sender, const QVariantMap &result)
{
    int response = result.value("response").toInt();

    int internalMid = sender->id();
    int serverMid = response;

    emit successfullyMessageSent(internalMid, serverMid);
    delete sender;
}
