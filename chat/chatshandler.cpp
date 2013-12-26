#include "chatshandler.h"

ChatsHandler::ChatsHandler()
{
    _historyPacket = new HistoryPacket(Client::instance()->connection());
    connect(_historyPacket, SIGNAL(history(const HistoryPacket*,int,MessageList)), this, SLOT(onHistoryLoaded(const HistoryPacket*,int,MessageList)));
}

ChatsHandler::~ChatsHandler()
{
    delete _historyPacket;
    clear();
}

void ChatsHandler::create(const DialogItem dialog)
{
    int id = dialog->id();

    _chats[id] = new Chat(dialog);
    _historyPacket->load(id);
}

bool ChatsHandler::contains(const int id) const
{
    return _chats.contains(id);
}

Chat *ChatsHandler::chat(const int id) const
{
    return _chats.value(id);
}

void ChatsHandler::clear()
{
    _chats.clear();
    //todo память
}

void ChatsHandler::onHistoryLoaded(const HistoryPacket *sender, const int id, const MessageList &messages)
{
    if (contains(id))
    {
        chat(id)->model()->replace(messages);
    }
}
