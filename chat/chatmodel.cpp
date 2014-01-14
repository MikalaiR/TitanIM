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

ChatModel::ChatModel(const DialogItem dialog, const ProfileItem ownProfile, QObject *parent) :
    QAbstractListModel(parent)
{
    _dialog = dialog;
    _ownProfile = ownProfile;

    _messages = MessageList::create();
    connect(_messages.data(), SIGNAL(itemChanged(int)), this, SLOT(onItemChanged(int)));

    connect(this, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(onRowsChanged(QModelIndex,int,int)));
    connect(this, SIGNAL(rowsRemoved(QModelIndex,int,int)), this, SLOT(onRowsChanged(QModelIndex,int,int)));
}

ChatModel::~ChatModel()
{
}

void ChatModel::append(const MessageList items)
{
    if (!items->count())
        return;

    beginInsertRows(QModelIndex(), _messages->count(), _messages->count() + items->count() - 1);
    _messages->add(items->toList());
    endInsertRows();
}

void ChatModel::append(const MessageItem item, const bool replace)
{
    if (replace && _messages->replace(item))
    {
        return;
    }

    beginInsertRows(QModelIndex(), _messages->count(), _messages->count());
    _messages->add(item);
    endInsertRows();
}

void ChatModel::prepend(const MessageItem item, const bool replace)
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

MessageItem ChatModel::at(const int row) const
{
    return _messages->at(row);
}

int ChatModel::indexOf(const int mid) const
{
    return _messages->indexOf(mid);
}

QHash<int, QByteArray> ChatModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractListModel::roleNames();

    roles[bodyRole] = "body";
    roles[dateRole] = "date";
    roles[dateStrRole] = "dateStr";
    roles[uidRole] = "uid";
    roles[midRole] = "mid";
    roles[isUnreadRole] = "isUnread";
    roles[isOutRole] = "isOut";

    return roles;
}

QVariant ChatModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= _messages->count())
    {
        return QVariant();
    }

    MessageItem message = _messages->at(index.row());
    ProfileItem profile = _dialog->isGroupChat() ? _dialog->groupChatHandler()->user(message->uid()) : _dialog->profile();

    switch (role)
    {
    case Qt::DisplayRole:
        return message->body();

    case Qt::DecorationRole:
        return message->isOut() ? _ownProfile->photoMediumRect() : profile->photoMediumRect();

    case bodyRole:
        return message->body();

    case dateRole:
        return message->date();

    case dateStrRole:
        return Utils::dateToText(message->date());

    case uidRole:
        return message->uid();

    case midRole:
        return message->id();

    case isUnreadRole:
        return message->isUnread();

    case isOutRole:
        return message->isOut();
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

void ChatModel::onItemChanged(const int i)
{
    QModelIndex idx = index(i, 0);
    emit dataChanged(idx, idx);

    if (i == 0)
    {
        _dialog->setMessage(_messages->at(0));
    }
}

void ChatModel::onRowsChanged(const QModelIndex &parent, int first, int last)
{
    if (first == 0 || last == 0)
    {
        if (_messages->count())
        {
            _dialog->setMessage(_messages->at(0));
        }
        else
        {
            MessageItem emptyMessage = MessageItem::create();
            _dialog->setMessage(emptyMessage);
        }
    }
}
