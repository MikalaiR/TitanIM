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

#ifndef CHATMODEL_H
#define CHATMODEL_H

#include <QAbstractListModel>
#include <QStringList>
#include "vk/messagelist.h"
#include "vk/dialogitem.h"
#include "vk/typingitem.h"
#include "vk/servicemsgitem.h"
#include "vk/client.h"
#include "vk/historypacket.h"
#include "vk/utils.h"

class ChatModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum chatRole
    {
        IdRole = Qt::UserRole,
        MessageTypeRole,
        DateRole,
        TimeStrRole,
        AttachmentsRole,
        UidRole,
        IsUnreadRole,
        IsOutRole,
        IsError,
        IsSendingRole,
        DeletedRole,
        SectionRole,
        ActionRole,
        ActionMidRole,
        ActionTextRole
    };

    explicit ChatModel(const DialogItem dialog, QObject *parent = 0);
    ~ChatModel();
    void load(const int count=20);
    void loadNext(const int count=20);
    void append(const MessageList items);
    void append(const MessageBaseItem item, const bool replace=false);
    void prepend(const MessageBaseItem item, const bool replace=false);
    void replaceAll(const MessageList items);
    bool remove(int row, int count);
    bool removeNotFake(int row, int count);
    MessageBaseItem at(const int row) const;
    int indexOf(const int id) const;
    void markAsRead(const int id);
    QHash<int, QByteArray> roleNames() const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant& value, int role = Qt::EditRole);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    void setLazyLoad(const bool on);

private:
    MessageList _messages;
    DialogItem _dialog;
    HistoryPacket *_historyPacket;
    int _serverCount;
    bool _lazyLoad;
    mutable bool _isLoading;

protected:
    bool canFetchMore(const QModelIndex &parent) const;
    void fetchMore(const QModelIndex &parent);

protected slots:
    void onHistoryLoaded(const HistoryPacket *sender, const int id, const MessageList &messages);
    void onItemChanged(const int i);

signals:
    void rowsAllReplaced();
};

#endif // CHATMODEL_H
