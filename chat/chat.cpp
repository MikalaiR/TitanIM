#include "chat.h"

int Chat::_internalMessageId = 0;

Chat::Chat()
{
    _sendMessageHandler = new SendMessageHandler(Client::instance()->connection());
}

Chat::Chat(const DialogItem dialog)
{
    _dialog = dialog;
    _model = new ChatModel(_dialog, Client::instance()->profile(), this);
    _sendMessageHandler = new SendMessageHandler(Client::instance()->connection());
}

Chat::~Chat()
{
    delete _sendMessageHandler;
    delete _model;
}

int Chat::id() const
{
    return _dialog->id();
}

bool Chat::isGroupChat() const
{
    return _dialog->isGroupChat();
}

DialogItem Chat::dialog() const
{
    return _dialog;
}

ChatModel *Chat::model() const
{
    return _model;
}

void Chat::sendMessage(const QString &text)
{
    if (text.isEmpty() /*&& outAttachmentsCount == 0*/)//todo
    {
        return;
    }

    MessageItem message = MessageItem::create();
    message->setMid(--_internalMessageId);

    if (isGroupChat())
    {
        message->setChatId(_dialog->groupChatHandler()->chatId());
        message->setUid(Client::instance()->uid());
    }
    else
    {
        message->setUid(_dialog->profile()->uid());
    }

    message->setDate(QDateTime::currentDateTime());
    message->setIsUnread(true);
    message->setIsOut(true);
    message->setIsError(false);
    message->setBody(text);

//    message->setAttachments(_outAttachments);
//    _outAttachments = null;
//    addMessage(message);

    _sendMessageHandler->send(message);
}
