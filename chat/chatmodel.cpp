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

#include "chatmodel.h"

ChatModel::ChatModel(const DialogItem dialog, QObject *parent) :
    QAbstractListModel(parent)
{
    _dialog = dialog;
    _ownProfile = Client::instance()->profile();

    _messages = MessageList::create();
    connect(_messages.data(), SIGNAL(itemChanged(int)), this, SLOT(onItemChanged(int)));

    _historyPacket = new HistoryPacket(Client::instance()->connection());
    connect(_historyPacket, SIGNAL(history(const HistoryPacket*,int,MessageList)), this, SLOT(onHistoryLoaded(const HistoryPacket*,int,MessageList)));

    _serverCount = 0;
    _isLoading = false;
}

ChatModel::~ChatModel()
{
    delete _historyPacket;
}

void ChatModel::load(const int count)
{
    _historyPacket->load(_dialog->id(), 0, count);
}

void ChatModel::loadNext(const int count)
{
    _historyPacket->load(_dialog->id(), _messages->deliveredMsgCount(), count);
}

void ChatModel::append(const MessageList items)
{
    if (!items->count())
        return;

    beginInsertRows(QModelIndex(), _messages->count(), _messages->count() + items->count() - 1);
    _messages->add(items->toList());
    endInsertRows();
}

void ChatModel::append(const MessageBaseItem item, const bool replace)
{
    if (replace && _messages->replace(item))
    {
        return;
    }

    beginInsertRows(QModelIndex(), _messages->count(), _messages->count());
    _messages->add(item);
    endInsertRows();
}

void ChatModel::prepend(const MessageBaseItem item, const bool replace)
{
    if (replace && _messages->replace(item))
    {
        return;
    }

    beginInsertRows(QModelIndex(), 0, 0);
    _messages->prepend(item);
    endInsertRows();
}

void ChatModel::replaceAll(const MessageList items)
{
    remove(0, rowCount());
    append(items);

    emit rowsAllReplaced();
}

bool ChatModel::remove(int row, int count)
{
    if (count <= 0 || row < 0)
        return false;

    beginRemoveRows(QModelIndex(), row, row + count - 1);

    for (int i = 0; i < count; ++i)
    {
        _messages->removeAt(row);
    }

    endRemoveRows();

    return true;
}

MessageBaseItem ChatModel::at(const int row) const
{
    return _messages->at(row);
}

int ChatModel::indexOf(const int id) const
{
    return _messages->indexOf(id);
}

void ChatModel::markAsRead(const int id)
{
    int i = _messages->indexOf(id);

    if (i > -1)
    {
        qobject_cast<MessageItem>(_messages->at(i))->setIsUnread(false);
        onItemChanged(i);
    }
}

QHash<int, QByteArray> ChatModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractListModel::roleNames();

    roles[IdRole] = "id";
    roles[MessageTypeRole] = "messageType";
    roles[BodyRole] = "body";
    roles[DateRole] = "date";
    roles[TimeStrRole] = "timeStr";
    roles[AttachmentsRole] = "attachments";
    roles[UidRole] = "uid";
    roles[IsUnreadRole] = "isUnread";
    roles[IsOutRole] = "isOut";
    roles[IsSendingRole] = "isSending";
    roles[DeletedRole] = "deleted";
    roles[OnlineRole] = "online";
    roles[SectionRole] = "section";

    return roles;
}

QVariant ChatModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= _messages->count())
    {
        return QVariant();
    }

    MessageBaseItem messageBase = _messages->at(index.row());

    //global role
    switch (role)
    {
    case IdRole:
        return messageBase->id();

    case MessageTypeRole:
        return messageBase->messageType();

    case DateRole:
        return messageBase->date();

    case DeletedRole:
        return messageBase->deleted();

    case SectionRole:
        return Utils::dateToSection(messageBase->date());
    }

    //type role
    switch (messageBase->messageType()) {
    case MessageBase::Text:
    {
        MessageItem message = qobject_cast<MessageItem>(messageBase);
        ProfileItem profile = _dialog->isGroupChat() ? _dialog->groupChatHandler()->user(message->uid()) : _dialog->profile();

        switch (role)
        {
        case Qt::DisplayRole:
            return message->isOut() ? _ownProfile->fullName() : profile->fullName();

        case Qt::DecorationRole:
            return message->isOut() ? _ownProfile->photoMediumRect() : profile->photoMediumRect();

        case BodyRole:
            return message->body();

        case TimeStrRole:
            return messageBase->date().toString("hh:mm");

        case AttachmentsRole:
            return QVariant::fromValue(message->attachments());

        case UidRole:
            return message->uid();

        case IsUnreadRole:
            return message->isUnread();

        case IsOutRole:
            return message->isOut();

        case IsSendingRole:
            return message->isSending();

        case OnlineRole:
            return message->isOut() ? _ownProfile->online() : profile->online();
        }

        break;
    }

    case MessageBase::Typing:
    {
        TypingItem typing = qobject_cast<TypingItem>(messageBase);
        ProfileItem profile = _dialog->isGroupChat() ? _dialog->groupChatHandler()->user(typing->uid()) : _dialog->profile();

        switch (role)
        {
        case Qt::DecorationRole:
            return profile->photoMediumRect();

        case UidRole:
            return typing->uid();
        }

        break;
    }

    case MessageBase::Service:
    {
        break;
    }
    }

    return QVariant();
}

bool ChatModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() && !value.isValid() && index.row() >= _messages->count())
    {
        return false;
    }

    return false;
}

int ChatModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return _messages->count();
}

Qt::ItemFlags ChatModel::flags(const QModelIndex &index) const
{
    if (index.isValid())
    {
        return (Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    }
    else
    {
        return Qt::NoItemFlags;
    }
}

bool ChatModel::canFetchMore(const QModelIndex &parent) const
{
    if (_isLoading || _messages->deliveredMsgCount() >= _serverCount)
    {
        return false;
    }

    _isLoading = true;
    return true;
}

void ChatModel::fetchMore(const QModelIndex &parent)
{
    loadNext();
}

void ChatModel::onHistoryLoaded(const HistoryPacket *sender, const int id, const MessageList &messages)
{
    _serverCount = sender->serverCount();

    if (!sender->offset())
    {
        replaceAll(messages);
    }
    else
    {
        append(messages);
    }

    _isLoading = false;
}

void ChatModel::onItemChanged(const int i)
{
    QModelIndex idx = index(i, 0);
    emit dataChanged(idx, idx);
}
