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

#include "dialogshandler.h"

DialogsHandler::DialogsHandler()
{
    _unreadDialogs = 0;
    _flagMarkAsRead = false;

    _model = new DialogsModel(this);
    connect(_model, SIGNAL(unreadDialogs(int)), this, SLOT(setUnreadDialogs(int)));

    _proxy = new QSortFilterProxyModel(_model);
    _proxy->setDynamicSortFilter(true);
    _proxy->setSortCaseSensitivity(Qt::CaseInsensitive);
    _proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
    _proxy->setSortRole(DialogsModel::dateRole);
    _proxy->sort(0, Qt::DescendingOrder);
    _proxy->setSourceModel(_model);

    connect(Client::instance()->longPoll(), SIGNAL(messageInAdded(DialogItem)), this, SLOT(onLongPollMessageInAdded(DialogItem)));
    connect(Client::instance()->longPoll(), SIGNAL(messageOutAdded(DialogItem)), this, SLOT(onLongPollMessageOutAdded(DialogItem)));
    connect(Client::instance()->longPoll(), SIGNAL(chatTyping(int,int,int)), this, SLOT(onLongPollChatTyping(int,int,int)));
    connect(Client::instance()->longPoll(), SIGNAL(userStatusChanged(int,bool)), this, SLOT(onUserStatusChanged(int,bool)));
    connect(Client::instance()->longPoll(), SIGNAL(unreadDialogs(int)), this, SLOT(setUnreadDialogs(int)));
    connect(Client::instance()->longPoll(), SIGNAL(inMessagesRead(int,int)), this, SLOT(onInMessagesRead(int,int)));
    connect(Client::instance()->longPoll(), SIGNAL(messageFlagsSet(int,int,int)), this, SLOT(onMessageFlagsSet(int,int,int)));
    connect(Client::instance()->longPoll(), SIGNAL(messageFlagsReseted(int,int,int,uint)), this, SLOT(onMessageFlagsReseted(int,int,int,uint)));

    qRegisterMetaType<DialogsModel*>("DialogsModel*");

    startTimer(500); //typing animation
}

DialogsHandler::~DialogsHandler()
{
    delete _proxy;
    delete _model;
}

DialogsModel *DialogsHandler::model() const
{
    return _model;
}

QSortFilterProxyModel *DialogsHandler::proxy() const
{
    return _proxy;
}

int DialogsHandler::unreadDialogs() const
{
    return _unreadDialogs;
}

QString DialogsHandler::typingText() const
{
    return _typingText;
}

DialogItem DialogsHandler::dialog(const int index, const bool isProxyIndex) const
{
    if (isProxyIndex)
    {
        QModelIndex proxyIndex = _proxy->index(index, 0);
        QModelIndex modelIndex = _proxy->mapToSource(proxyIndex);

        return _model->at(modelIndex);
    }
    else
    {
        return _model->at(index);
    }
}

int DialogsHandler::indexOf(const int id, const bool proxyIndex) const
{
    int index = _model->indexOf(id);

    if (proxyIndex)
    {
        QModelIndex modelIndex = _model->index(index, 0);
        QModelIndex proxyIndex = _proxy->mapFromSource(modelIndex);

        return proxyIndex.row();
    }
    else
    {
        return index;
    }
}

void DialogsHandler::incUnreadDialogs()
{
    setUnreadDialogs(_unreadDialogs + 1);
}

void DialogsHandler::decUnreadDialogs()
{
    setUnreadDialogs(_unreadDialogs - 1);
}

void DialogsHandler::setUnreadDialogs(const int unreadDialogs)
{
    //todo fixme
    int serverUnreadDialogs = unreadDialogs;

    if (_flagMarkAsRead)
    {
        _flagMarkAsRead = false;
        serverUnreadDialogs--;
    }

    if (_unreadDialogs != serverUnreadDialogs)
    {
        _unreadDialogs = serverUnreadDialogs;
        emit unreadDialogsChanged(_unreadDialogs);
    }
}

void DialogsHandler::onLongPollMessageInAdded(const DialogItem dialog)
{
    int id = dialog->id();
    int i = _model->indexOf(id);

    //todo fixme
    if (id == Chats::instance()->currentChatId())
    {
        _flagMarkAsRead = true;
        Chats::instance()->currentChat()->markAsRead();
    }

    if (i > -1)
    {
        _model->at(i)->setMessage(dialog->message());

        if (!_flagMarkAsRead)
        {
            _model->at(i)->incUnreadDialogs();
        }
    }
    else
    {
        if (!_flagMarkAsRead)
        {
            dialog->incUnreadDialogs();
        }

        dialog->getMessage(Client::instance()->connection()); //update unread

        if (dialog->isGroupChat())
        {
            dialog->groupChatHandler()->getAllFields(Client::instance()->connection());
        }

        _model->append(dialog);
    }

    static QString fileName = Settings::instance()->dataDir() + "/sounds/message.wav";
    static QString cmd = Settings::instance()->loadMain("main/cmdSound", "aplay -q").toString();
    Utils::playSound(fileName, cmd);
}

void DialogsHandler::onLongPollMessageOutAdded(const DialogItem dialog)
{
    int id = dialog->id();
    int i = _model->indexOf(id);

    if (i > -1)
    {
        _model->at(i)->setMessage(dialog->message());
    }
    else
    {
        if (dialog->isGroupChat())
        {
            dialog->groupChatHandler()->getAllFields(Client::instance()->connection());
        }

        _model->append(dialog);
    }
}

void DialogsHandler::onLongPollChatTyping(const int id, const int uid, const int chatId)
{
    int i = _model->indexOf(id);

    if (i > -1)
    {
        _model->at(i)->typing(uid);
    }
}

void DialogsHandler::onUserStatusChanged(const int uid, const bool online)
{
    int i = _model->indexOf(uid);

    if (i > -1)
    {
        _model->at(i)->profile()->setOnline(online);
    }
}

void DialogsHandler::onInMessagesRead(const int id, const int mid)
{
    int i = _model->indexOf(id);

    if (i > -1)
    {
        DialogItem dialog = _model->at(i);

        if (dialog->message()->id() == mid)
        {
            dialog->setUnreadCount(0);
        }
        else
        {
            dialog->getMessage(Client::instance()->connection());
        }
    }
}

void DialogsHandler::onMessageFlagsSet(const int mid, const int mask, const int id)
{
    if ((mask & LongPoll::Deleted) || (mask & LongPoll::Spam))
    {
        int i = _model->indexOf(id);

        if (i > -1)
        {
            _model->at(i)->getMessage(Client::instance()->connection());
        }
    }
}

void DialogsHandler::onMessageFlagsReseted(const int mid, const int mask, const int id, const uint date)
{
    if ((mask & LongPoll::Deleted) || (mask & LongPoll::Spam))
    {
        int i = _model->indexOf(id);

        if (i > -1)
        {
            _model->at(i)->getMessage(Client::instance()->connection());
        }
        else if (_model->endDate() < date)
        {
            //recovery dialog
            DialogsPacket *dialogsPacket = new DialogsPacket(Client::instance()->connection());
            connect(dialogsPacket, SIGNAL(dialogs(const DialogsPacket*,DialogList)), SLOT(onRecoveryDialog(const DialogsPacket*,DialogList)));
            dialogsPacket->load(mid);
        }
    }
}

void DialogsHandler::onRecoveryDialog(const DialogsPacket *sender, const DialogList &dialogs)
{
    _model->append(dialogs->at(0));
    delete sender;
}

void DialogsHandler::timerEvent(QTimerEvent *event)
{
    static int k = 1;
    static const QString typing = tr("typing");
    static const int len = typing.length();

    k++;
    if (k == 4) k = 1;

    _typingText = typing.leftJustified(len + k, '.');

    emit typingTextChanged(_typingText);
}
