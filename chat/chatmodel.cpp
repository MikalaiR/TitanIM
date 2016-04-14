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

    _messages = MessageList::create();
    connect(_messages.data(), SIGNAL(itemChanged(int)), this, SLOT(onItemChanged(int)));

    _historyPacket = new HistoryPacket(Client::instance()->connection());
    connect(_historyPacket, SIGNAL(history(const HistoryPacket*,int,MessageList)), this, SLOT(onHistoryLoaded(const HistoryPacket*,int,MessageList)));

    _serverCount = 0;
    _lazyLoad = true;
    _isLoading = false;
    _selectedCount = 0;
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
    _historyPacket->load(_dialog->id(), _messages->notFakeMsgCount(), count);
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
    removeNotFake(0, rowCount());
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

bool ChatModel::removeNotFake(int row, int count)
{
    if (count <= 0 || row < 0)
        return false;

    int n = row;
    int k = 0;

    for (int i = 0; i < count; i++)
    {
        if ( (_messages->at(n + k)->isFake()) && (_messages->at(n + k)->messageType() != MessageBase::Typing))
        {
            if (k > 0)
            {
                remove(n, k);
            }

            n++;
            k = 0;
        }
        else
        {
            k++;
        }
    }

    if (k > 0)
    {
        remove(n, k);
    }
}

MessageBaseItem ChatModel::at(const int row) const
{
    return _messages->at(row);
}

int ChatModel::indexOf(const int id) const
{
    return _messages->indexOf(id);
}

MessageList ChatModel::getSelectedItems()
{
    MessageList list = MessageList::create();

    for (int i = _messages->count() - 1; i >= 0; i--)
    {
        MessageBaseItem msg = _messages->at(i);

        if (msg->isChecked())
        {
            msg->setChecked(false);
            list->add(msg);
        }
    }

    setSelectedCount(0);
    return list;
}

void ChatModel::deleteSelectedItems()
{
    QStringList list;

    for (int i = 0; i < _messages->count(); i++)
    {
        MessageBaseItem msg = _messages->at(i);

        if (msg->isChecked())
        {
            msg->setChecked(false);
            msg->setDeleted(true);
            list.append(QString::number(msg->id()));
        }
    }

    Packet *packet = new Packet("messages.delete");
    packet->addParam("message_ids", list.join(','));
    Client::instance()->connection()->appendQuery(packet);

    setSelectedCount(0);
}

void ChatModel::copyTextSelectedItems()
{
    QStringList list;
    Engine *engine = Client::instance()->engine();

    for (int i = _messages->count() - 1; i >= 0; i--)
    {
        MessageItem msg = qobject_cast<MessageItem>(_messages->at(i));

        if (msg->isChecked())
        {
            msg->setChecked(false);

            QString name = msg->isOut() ? engine->getProfile()->fullName()
                                        : engine->getProfile(msg->uid())->fullName();

            QString date = msg->date().toString(", [d.MM.yyyy, hh:mm]");

            list.append(name + date);
            list.append(msg->plainBody());
            list.append("");
        }
    }

    QGuiApplication::clipboard()->setText(list.join('\n'));
    setSelectedCount(0);
}

void ChatModel::clearSelected()
{
    for (int i = 0; i < _messages->count(); i++)
    {
        _messages->at(i)->setChecked(false);
    }

    setSelectedCount(0);
}

int ChatModel::selectedCount() const
{
    return _selectedCount;
}

void ChatModel::setSelectedCount(const int count)
{
    if (_selectedCount != count)
    {
        _selectedCount = count;
        emit selectedCountChanged(count);
    }
}

bool ChatModel::markAsRead(const int id)
{
    int i = _messages->indexOf(id);

    if (i > -1)
    {
        qobject_cast<MessageItem>(_messages->at(i))->setIsUnread(false);
        onItemChanged(i);
        return true;
    }

    return false;
}

bool ChatModel::markAsDeleted(const int id, const bool isDeleted)
{
    int i = _messages->indexOf(id);

    if (i > -1)
    {
        qobject_cast<MessageItem>(_messages->at(i))->setDeleted(isDeleted);
        onItemChanged(i);
        return true;
    }

    return false;
}

QHash<int, QByteArray> ChatModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractListModel::roleNames();

    roles[IdRole] = "id";
    roles[MessageTypeRole] = "messageType";
    roles[DateRole] = "date";
    roles[TimeStrRole] = "timeStr";
    roles[AttachmentsRole] = "attachments";
    roles[UidRole] = "uid";
    roles[IsUnreadRole] = "isUnread";
    roles[IsOutRole] = "isOut";
    roles[IsError] = "isError";
    roles[IsSendingRole] = "isSending";
    roles[IsGift] = "isGift";
    roles[IsSticker] = "isSticker";
    roles[IsSingle] = "isSingle";
    roles[IsDeletedRole] = "isDeleted";
    roles[SectionRole] = "section";
    roles[ActionRole] = "action";
    roles[ActionMidRole] = "actionMid";
    roles[ActionTextRole] = "actionText";
    roles[IsCheckedRole] = "isChecked";

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

    case UidRole:
        return messageBase->uid();

    case DateRole:
        return messageBase->date();

    case IsDeletedRole:
        return messageBase->isDeleted();

    case SectionRole:
        return Utils::dateToSection(messageBase->date());

    case IsCheckedRole:
        return messageBase->isChecked();
    }

    //type role
    switch (messageBase->messageType()) {
    case MessageBase::Text:
    {
        MessageItem message = qobject_cast<MessageItem>(messageBase);
        bool isAttachments = message->attachments();

        switch (role)
        {
        case Qt::DisplayRole:
            return message->body();

        case TimeStrRole:
            return messageBase->date().toString("hh:mm");

        case AttachmentsRole:
            return QVariant::fromValue(message->attachments());

        case IsUnreadRole:
            return message->isUnread();

        case IsOutRole:
            return message->isOut();

        case IsError:
            return message->isError();

        case IsSendingRole:
            return message->isSending();

        case IsGift:
            return isAttachments && message->attachments()->filterByType(Attachment::Gift).count();

        case IsSticker:
            return isAttachments && message->attachments()->filterByType(Attachment::Sticker).count();

        case IsSingle:
            return isAttachments && message->body().isEmpty() && message->attachments()->count() == 1
                    && message->attachments()->filterByType(Attachment::Photo).count() == 1;

        case ActionRole:
            return message->action();

        case ActionMidRole:
            return message->actionMid();

        case ActionTextRole:
            return message->actionText();
        }

        break;
    }

    case MessageBase::Typing:
    {
        break;
    }

    case MessageBase::Service:
    {
        ServiceMsgItem serviceMsg = qobject_cast<ServiceMsgItem>(messageBase);

        switch (role)
        {
        case Qt::DisplayRole:
            return serviceMsg->body();
        }

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

    MessageBaseItem messageBase = _messages->at(index.row());

    switch (role)
    {
    case IsCheckedRole:
        if (messageBase->isChecked() != value.toBool())
        {
            messageBase->setChecked(value.toBool());

            if (value.toBool())
                setSelectedCount(_selectedCount + 1);
            else
                setSelectedCount(_selectedCount - 1);

            return true;
        }
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

void ChatModel::setLazyLoad(const bool on)
{
    _lazyLoad = on;
}

bool ChatModel::canFetchMore(const QModelIndex &parent) const
{
    if (!_lazyLoad || _isLoading || _messages->notFakeMsgCount() >= _serverCount)
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
