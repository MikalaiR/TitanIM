/*
    Copyright (c) 2013 by Ruslan Nazarov <818151@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/

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
    _currentDialog = 0;

    _proxy = new QSortFilterProxyModel(this);
//    _proxy->setDynamicSortFilter(true);
//    _proxy->setSortCaseSensitivity(Qt::CaseInsensitive);
//    _proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
//    _proxy->setSortRole(Qt::UserRole);
//    _proxy->sort(0, Qt::DescendingOrder);

    qmlRegisterType<Chat>("TitanIM", 2, 0, "Chat");
    qmlRegisterType<DialogItemPrivate>("TitanIM", 2, 0, "DialogItem");
    qRegisterMetaType<QSortFilterProxyModel*>("QSortFilterProxyModel*");
    qmlRegisterType<AttachmentList>("TitanIM", 2, 0, "AttachmentList");
    qmlRegisterType<Attachment>("TitanIM", 2, 0, "Attachment");
    qRegisterMetaType<Attachment::AttachmentType>("Attachment::AttachmentType");
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
    return _currentDialog;
}

QSortFilterProxyModel* Chats::currentChatModel() const
{
    return _proxy;
}

void Chats::setCurrentChat(const int id)
{
    if (_currentChatId != id)
    {
        if (_currentDialog)
        {
            _currentDialog->setCurrent(false);
        }

        Chat *chat = _chatsHandler->chat(id);
        _proxy->setSourceModel(chat->model());
        _currentDialog = chat->dialog().data();
        _currentDialog->setCurrent(true);
        _currentChatId = id;

        chat->markAsRead();

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
