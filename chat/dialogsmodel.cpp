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

#include "dialogsmodel.h"

DialogsModel::DialogsModel(QObject *parent) :
    QAbstractListModel(parent)
{
    _dialogs = MessageList::create();
    connect(_dialogs.data(), SIGNAL(itemChanged(int)), this, SLOT(onItemChanged(int)));

    _dialogsPacket = new DialogsPacket(Client::instance()->connection());
    connect(_dialogsPacket, SIGNAL(dialogs(const DialogsPacket*,const MessageList)), SLOT(onDialogsLoaded(const DialogsPacket*,const MessageList)));
}

DialogsModel::~DialogsModel()
{
    delete _dialogsPacket;
}

void DialogsModel::load(const int count)
{
    _dialogsPacket->load(0, count);
}

void DialogsModel::loadNext(const int count)
{
    _dialogsPacket->load(_dialogs->count(), count);
}

void DialogsModel::append(const MessageList items)
{
    if (!items->count())
        return;

    beginInsertRows(QModelIndex(), _dialogs->count(), _dialogs->count() + items->count() - 1);
    _dialogs->add(items->toVector());
    endInsertRows();
}

void DialogsModel::replace(const MessageList items)
{
    remove(0, rowCount());
    append(items);
}

bool DialogsModel::remove(int row, int count)
{
    if (count <= 0 || row < 0)
        return false;

    beginRemoveRows(QModelIndex(), row, row + count - 1);

    for (int i = 0; i < count; ++i)
    {
        _dialogs->remove(row);
    }

    endRemoveRows();

    return true;
}

MessageItem DialogsModel::at(const int row)
{
    return _dialogs->at(row);
}

QHash<int, QByteArray> DialogsModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractListModel::roleNames();

    roles[bodyRole] = "body";
    roles[dateRole] = "date";
    roles[dateStrRole] = "dateStr";
    roles[uidRole] = "uid";
    roles[midRole] = "mid";
    roles[isUnreadRole] = "isUnread";
    roles[isOutRole] = "isOut";
    roles[onlineRole] = "online";

    return roles;
}

QVariant DialogsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= _dialogs->count())
    {
        return QVariant();
    }

    MessageItem dialog = _dialogs->at(index.row());
    ProfileItem profile = dialog->profile();
    GroupChatHandler *groupChatHandler = dialog->groupChatHandler();

    switch (role)
    {
    case Qt::DisplayRole:
        return dialog->displayName();

    case Qt::DecorationRole:
        return groupChatHandler ? groupChatHandler->avatars() : QStringList(profile->photoMediumRect());

    case bodyRole:
        return dialog->body();

    case dateRole:
        return dialog->date();

    case dateStrRole:
        return Utils::dateToText(dialog->date());

    case uidRole:
        return dialog->uid();

    case midRole:
        return dialog->mid();

    case isUnreadRole:
        return dialog->isUnread();

    case isOutRole:
        return dialog->isOut();

    case onlineRole:
        return profile->online();
    }

    return QVariant();
}

bool DialogsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() && !value.isValid() && index.row() >= _dialogs->count())
    {
        return false;
    }

    return false;
}

int DialogsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return _dialogs->count();
}

Qt::ItemFlags DialogsModel::flags(const QModelIndex &index) const
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

void DialogsModel::onDialogsLoaded(const DialogsPacket *sender, const MessageList &dialogs)
{
    if (!sender->offset())
    {
        replace(dialogs);
    }
    else
    {
        append(dialogs);
    }
}

void DialogsModel::onItemChanged(const int i)
{
    QModelIndex idx = index(i, 0);
    emit dataChanged(idx, idx);
}
