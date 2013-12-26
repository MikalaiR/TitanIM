#include "chats.h"

Chats *Chats::aInstance = 0;

Chats *Chats::instance()
{
    if (!aInstance) aInstance = new Chats();
    return aInstance;
}

void Chats::destroy()
{
    if (aInstance)
        delete aInstance, aInstance = 0;
}

Chats::Chats()
{
    _chatsHandler = new ChatsHandler();

    _currentChatId = 0;
    _dialog = 0;

    _proxy = new QSortFilterProxyModel(this);
    _proxy->setDynamicSortFilter(true);
    _proxy->setSortCaseSensitivity(Qt::CaseInsensitive);
//    _proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
//    _proxy->setSortRole(Qt::UserRole);
//    _proxy->sort(0, Qt::DescendingOrder);

    qmlRegisterType<Chat>("TitanIM", 2, 0, "Chat");
    qmlRegisterType<DialogItemPrivate>("TitanIM", 2, 0, "DialogItem");
    qRegisterMetaType<QSortFilterProxyModel*>("QSortFilterProxyModel*");
}

Chats::~Chats()
{
    delete _chatsHandler;
}

int Chats::currentChatId() const
{
    return _currentChatId;
}

Chat *Chats::currentChat() const
{
    return _chatsHandler->chat(_currentChatId);
}

DialogItemPrivate *Chats::currentChatDialog() const
{
    return _dialog;
}

QSortFilterProxyModel* Chats::currentChatModel() const
{
    return _proxy;
}

void Chats::setCurrentChat(const int id)
{
    if (_currentChatId != id)
    {
        _currentChatId = id;
        _proxy->setSourceModel(_chatsHandler->chat(id)->model());
        _dialog = _chatsHandler->chat(id)->dialog().data();

        emit currentChatChanged(id);
    }
}

void Chats::openChat(const DialogItem dialog)
{
    int id = dialog->id();

    if (!_chatsHandler->contains(id))
    {
        _chatsHandler->create(dialog);
    }

    setCurrentChat(id);
}
