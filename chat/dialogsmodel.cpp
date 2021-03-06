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
    connect(_dialogs.data(), &DialogListPrivate::itemChanged, this, &DialogsModel::onItemChanged);

    _dialogsPacket = new DialogsPacket(Client::instance()->connection());
    connect(_dialogsPacket, &DialogsPacket::dialogs, this, &DialogsModel::onDialogsLoaded);

    connect(Client::instance()->pushSettings(), &PushSettings::muteUserChanged, this, &DialogsModel::onMuteUserChanged);

    _serverCount = 0;
    _isLoading = false;
    _endDate = 0;
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
    removeAll();
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

void DialogsModel::removeAll()
{
    beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
    _dialogs->clear();
    endRemoveRows();
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

uint DialogsModel::endDate() const
{
    return _endDate;
}

QHash<int, QByteArray> DialogsModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractListModel::roleNames();

    roles[BodyRole] = "body";
    roles[DateRole] = "date";
    roles[DateStrRole] = "dateStr";
    roles[UidRole] = "uid";
    roles[FromRole] = "from";
    roles[MidRole] = "mid";
    roles[IdRole] = "id";
    roles[UnreadCountRole] = "unreadCount";
    roles[IsOutRole] = "isOut";
    roles[IsUnreadRole] = "isUnread";
    roles[IsGroupChat] = "isGroupChat";
    roles[IsMuteRole] = "isMuteRole";
    roles[OnlineRole] = "online";
    roles[TypingRole] = "typing";
    roles[EmojiRole] = "emoji";

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

    switch (role)
    {
    case Qt::DisplayRole:
        return dialog->displayName();

    case Qt::DecorationRole:
        return dialog->decoration();

    case BodyRole:
        return message->shortBody();

    case DateRole:
        return message->date();

    case DateStrRole:
        return Utils::dateToText(message->date());

    case UidRole:
        return message->uid();

    case FromRole:
        return message->isOut() ? Client::instance()->uid() : message->uid();

    case MidRole:
        return message->id();

    case IdRole:
        return dialog->id();

    case UnreadCountRole:
        return dialog->unreadCount();

    case IsOutRole:
        return message->isOut();

    case IsUnreadRole:
        return message->isUnread();

    case IsGroupChat:
        return dialog->isGroupChat();

    case IsMuteRole:
        return Client::instance()->pushSettings()->isMuteUser(dialog->id());

    case OnlineRole:
        return !dialog->isGroupChat() && profile->online();

    case TypingRole:
        return dialog->isTyping();

    case EmojiRole:
        return message->emoji();
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

        if (sender->unreadDialogs() > 0)
        {
            Notificator::instance()->playSoundMessageIn();
        }
    }
    else
    {
        append(dialogs);
    }

    _endDate = dialogs->last()->message()->unixtime();

    _isLoading = false;
}

void DialogsModel::onItemChanged(const int i)
{
    if (_dialogs->at(i)->message()->id() == 0 || _dialogs->at(i)->message()->unixtime() < _endDate)
    {
        beginRemoveRows(QModelIndex(), i, i);
        _dialogs->removeAt(i);
        endRemoveRows();
    }
    else
    {
        QModelIndex idx = index(i, 0);
        emit dataChanged(idx, idx);
    }
}

void DialogsModel::onMuteUserChanged(const int id, const bool isMute)
{
    QModelIndex idx = index(_dialogs->indexOf(id), 0);
    emit dataChanged(idx, idx);
}
