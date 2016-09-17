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
    connect(_model, &DialogsModel::unreadDialogs, this, &DialogsHandler::setUnreadDialogs);

    _proxy = new QSortFilterProxyModel(_model);
    _proxy->setDynamicSortFilter(true);
    _proxy->setSortCaseSensitivity(Qt::CaseInsensitive);
    _proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
    _proxy->setSortRole(DialogsModel::DateRole);
    _proxy->sort(0, Qt::DescendingOrder);
    _proxy->setSourceModel(_model);

    LongPoll *longPoll = Client::instance()->longPoll();

    connect(longPoll, &LongPoll::obsoleteFriendsOnline, this, &DialogsHandler::getCounterUnreadDialogs);
    connect(longPoll, &LongPoll::messageInAdded, this, &DialogsHandler::onLongPollMessageInAdded);
    connect(longPoll, &LongPoll::messageOutAdded, this, &DialogsHandler::onLongPollMessageOutAdded);
    connect(longPoll, &LongPoll::chatTyping, this, &DialogsHandler::onLongPollChatTyping);
    connect(longPoll, &LongPoll::unreadDialogs, this, &DialogsHandler::setUnreadDialogs);
    connect(longPoll, &LongPoll::inMessagesRead, this, &DialogsHandler::onInMessagesRead);
    connect(longPoll, &LongPoll::outMessagesRead, this, &DialogsHandler::onOutMessagesRead);
    connect(longPoll, &LongPoll::messageFlagsSet, this, &DialogsHandler::onMessageFlagsSet);
    connect(longPoll, &LongPoll::messageFlagsReseted, this, &DialogsHandler::onMessageFlagsReseted);
    connect(longPoll, &LongPoll::groupChatUpdated, this, &DialogsHandler::onGroupChatUpdated);

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
    connect(packet, &Packet::finished, this, &DialogsHandler::onCounterUnreadDialogs);
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
        Notificator::instance()->setBadge(_unreadDialogs);
        emit unreadDialogsChanged(_unreadDialogs);
    }
}

void DialogsHandler::onLongPollMessageInAdded(const int id, const MessageItem message, const ProfileItem profile)
{
    int i = _model->indexOf(id);

    //todo fixme
    if (id == Chats::instance()->currentChatId() && message->isUnread() && Utils::applicationIsActive()
            && Settings::instance()->loadProfile("chat/autoRead", true).toBool())
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
        bool isMute = Client::instance()->pushSettings()->isMuteUser(id);

        if (!_flagMarkAsRead)
        {
            dialog->incUnreadDialogs();

            if (!isMute && (!Utils::applicationIsActive() || id != Chats::instance()->currentChatId()))
            {
                Notificator::instance()->showNotification(id, message->id(), Emoticons::instance()->toEmoji(dialog->displayName()),message->plainBody(), true);
            }
        }

        if (!isMute)
        {
            Notificator::instance()->playSoundMessageIn();
        }
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

void DialogsHandler::onGroupChatUpdated(const int chatId, const bool self)
{
    int i = _model->indexOf(chatId + GROUP_CHAT_OFFSET);

    if (i > -1)
    {
        DialogItem dialog = _model->at(i);
        dialog->groupChatHandler()->getAllFields();
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
