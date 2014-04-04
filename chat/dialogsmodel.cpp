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
    _dialogs = DialogList::create();
    connect(_dialogs.data(), SIGNAL(itemChanged(int)), this, SLOT(onItemChanged(int)));

    _dialogsPacket = new DialogsPacket(Client::instance()->connection());
    connect(_dialogsPacket, SIGNAL(dialogs(const DialogsPacket*,const DialogList)), SLOT(onDialogsLoaded(const DialogsPacket*,const DialogList)));

    _serverCount = 0;
    _isLoading = false;
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

void DialogsModel::append(const DialogList items)
{
    if (!items->count())
        return;

    beginInsertRows(QModelIndex(), _dialogs->count(), _dialogs->count() + items->count() - 1);
    _dialogs->add(items->toList());
    endInsertRows();
}

void DialogsModel::append(const DialogItem item, const bool replace)
{
    if (replace && _dialogs->replace(item))
    {
        return;
    }

    beginInsertRows(QModelIndex(), _dialogs->count(), _dialogs->count());
    _dialogs->add(item);
    endInsertRows();
}

void DialogsModel::replaceAll(const DialogList items)
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
        _dialogs->removeAt(row);
    }

    endRemoveRows();

    return true;
}

DialogItem DialogsModel::at(const int row)
{
    return _dialogs->at(row);
}

DialogItem DialogsModel::at(const QModelIndex &index)
{
    return _dialogs->at(index.row());
}

int DialogsModel::indexOf(const int id) const
{
    return _dialogs->indexOf(id);
}

QHash<int, QByteArray> DialogsModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractListModel::roleNames();

    roles[bodyRole] = "body";
    roles[dateRole] = "date";
    roles[dateStrRole] = "dateStr";
    roles[uidRole] = "uid";
    roles[midRole] = "mid";
    roles[idRole] = "id";
    roles[unreadCountRole] = "unreadCount";
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

    DialogItem dialog = _dialogs->at(index.row());
    MessageItem message = dialog->message();
    ProfileItem profile = dialog->profile();
    GroupChatHandler *groupChatHandler = dialog->groupChatHandler();

    switch (role)
    {
    case Qt::DisplayRole:
        return dialog->displayName();

    case Qt::DecorationRole:
        return dialog->decoration();

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

    case idRole:
        return dialog->id();

    case unreadCountRole:
        return dialog->unreadCount();

    case isOutRole:
        return message->isOut();

    case onlineRole:
        return !dialog->isGroupChat() && profile->online();
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

bool DialogsModel::canFetchMore(const QModelIndex &parent) const
{
    if (_isLoading || _dialogs->count() >= _serverCount)
    {
        return false;
    }

    _isLoading = true;
    return true;
}

void DialogsModel::fetchMore(const QModelIndex &parent)
{
    loadNext();
}

void DialogsModel::onDialogsLoaded(const DialogsPacket *sender, const DialogList &dialogs)
{
    _serverCount = sender->serverCount();

    emit unreadDialogs(sender->unreadDialogs());

    if (!sender->offset())
    {
        replaceAll(dialogs);
    }
    else
    {
        append(dialogs);
    }

    _isLoading = false;
}

void DialogsModel::onItemChanged(const int i)
{
    QModelIndex idx = index(i, 0);
    emit dataChanged(idx, idx);
}
