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
    _proxy->setSortRole(DialogsModel::DateRole);
    _proxy->sort(0, Qt::DescendingOrder);
    _proxy->setSourceModel(_model);

    LongPoll *longPoll = Client::instance()->longPoll();

    connect(longPoll, SIGNAL(obsoleteFriendsOnline()), this, SLOT(getCounterUnreadDialogs()));
    connect(longPoll, SIGNAL(messageInAdded(int,MessageItem,ProfileItem)), this, SLOT(onLongPollMessageInAdded(int,MessageItem,ProfileItem)));
    connect(longPoll, SIGNAL(messageOutAdded(int,MessageItem,ProfileItem)), this, SLOT(onLongPollMessageOutAdded(int,MessageItem,ProfileItem)));
    connect(longPoll, SIGNAL(chatTyping(int,int,int)), this, SLOT(onLongPollChatTyping(int,int,int)));
    connect(longPoll, SIGNAL(unreadDialogs(int)), this, SLOT(setUnreadDialogs(int)));
    connect(longPoll, SIGNAL(inMessagesRead(int,int)), this, SLOT(onInMessagesRead(int,int)));
    connect(longPoll, SIGNAL(outMessagesRead(int,int)), this, SLOT(onOutMessagesRead(int,int)));
    connect(longPoll, SIGNAL(messageFlagsSet(int,int,int)), this, SLOT(onMessageFlagsSet(int,int,int)));
    connect(longPoll, SIGNAL(messageFlagsReseted(int,int,int,uint)), this, SLOT(onMessageFlagsReseted(int,int,int,uint)));

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

void DialogsHandler::clear()
{
    _flagMarkAsRead = false;
    setUnreadDialogs(0);
    _model->removeAll();
}

void DialogsHandler::getCounterUnreadDialogs()
{
    Packet *packet = new Packet("account.getCounters");
    packet->addParam("filter", "messages");
    connect(packet, SIGNAL(finished(const Packet*,QVariantMap)), this, SLOT(onCounterUnreadDialogs(const Packet*,QVariantMap)));
    Client::instance()->connection()->appendQuery(packet);
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

void DialogsHandler::onLongPollMessageInAdded(const int id, const MessageItem message, const ProfileItem profile)
{
    int i = _model->indexOf(id);

    //todo fixme
    if (id == Chats::instance()->currentChatId() && message->isUnread() && Utils::applicationIsActive())
    {
        _flagMarkAsRead = true;
        Chats::instance()->currentChat()->markAsRead();
    }

    DialogItem dialog;

    if (i > -1)
    {
        dialog = _model->at(i);
        dialog->setMessage(message);
    }
    else
    {
        dialog = DialogItem::create();
        dialog->setProfile(profile);
        dialog->setMessage(message);
        dialog->createStructure();
        dialog->getAllFields(Client::instance()->connection(), true);

        _model->append(dialog);
    }

    if (message->isUnread())
    {
        if (!_flagMarkAsRead)
        {
            dialog->incUnreadDialogs();
            Notificator::instance()->showNotification(id, message->id(), dialog->displayName(), message->plainBody(), true);
        }

        Notificator::instance()->playSoundMessageIn();
    }

    emit newMessage(false, id);
}

void DialogsHandler::onLongPollMessageOutAdded(const int id, const MessageItem message, const ProfileItem profile)
{
    int i = _model->indexOf(id);

    if (i > -1)
    {
        _model->at(i)->setMessage(message);
    }
    else
    {
        DialogItem dialog = DialogItem::create();
        dialog->setProfile(profile);
        dialog->setMessage(message);
        dialog->createStructure();
        dialog->getAllFields(Client::instance()->connection(), true);

        _model->append(dialog);
    }

    emit newMessage(true, id);
}

void DialogsHandler::onLongPollChatTyping(const int id, const int uid, const int chatId)
{
    int i = _model->indexOf(id);

    if (i > -1)
    {
        _model->at(i)->typing(uid);
    }
}

void DialogsHandler::onInMessagesRead(const int id, const int mid)
{
    int i = _model->indexOf(id);

    if (i > -1)
    {
        DialogItem dialog = _model->at(i);
        int lastMid = dialog->message()->id();

        if ((lastMid == mid) || (dialog->message()->isOut() && lastMid - 1 == mid))
        {
            if (!dialog->message()->isOut())
            {
                dialog->message()->setIsUnread(false);
            }

            dialog->setUnreadCount(0);
        }
        else
        {
            dialog->getLastMessage(Client::instance()->connection());
        }
    }
}

void DialogsHandler::onOutMessagesRead(const int id, const int mid)
{
    int i = _model->indexOf(id);

    if (i > -1)
    {
        DialogItem dialog = _model->at(i);

        if (dialog->message()->id() == mid)
        {
            dialog->message()->setIsUnread(false);
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
            _model->at(i)->getLastMessage(Client::instance()->connection());
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
            _model->at(i)->getLastMessage(Client::instance()->connection());
        }
        else if (_model->endDate() < date)
        {
            //recovery dialog
            MessageItem message = MessageItem::create();
            message->setId(mid);
            message->setFromId(id);
            message->setDate(QDateTime::fromTime_t(date).toLocalTime());

            DialogItem dialog = DialogItem::create();
            dialog->setMessage(message);
            dialog->createStructure();
            dialog->getAllFields(Client::instance()->connection());
            _model->append(dialog);
        }
    }
}

void DialogsHandler::onCounterUnreadDialogs(const Packet *sender, const QVariantMap &result)
{
    QVariant response = result.value("response");
    int count = 0;

    if (response.type() == QVariant::Map)
    {
        count = response.toMap().value("messages").toInt();
    }

    setUnreadDialogs(count);

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
