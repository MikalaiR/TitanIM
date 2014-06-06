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

#ifndef DIALOGSMODEL_H
#define DIALOGSMODEL_H

#include <QAbstractListModel>
#include "vk/client.h"
#include "vk/dialogspacket.h"

class DialogsModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum dialogsRole
    {
        BodyRole = Qt::UserRole,
        DateRole,
        DateStrRole,
        UidRole,
        MidRole,
        IdRole,
        UnreadCountRole,
        IsOutRole,
        OnlineRole,
        TypingRole
    };

    explicit DialogsModel(QObject *parent = 0);
    ~DialogsModel();
    void load(const int count=20);
    void loadNext(const int count=20);
    void append(const DialogList items);
    void append(const DialogItem item, const bool replace=false);
    void replaceAll(const DialogList items);
    bool remove(int row, int count);
    DialogItem at(const int row);
    DialogItem at(const QModelIndex &index);
    int indexOf(const int id) const;
    uint endDate() const;
    QHash<int, QByteArray> roleNames() const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant& value, int role = Qt::EditRole);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

private:
    DialogList _dialogs;
    DialogsPacket *_dialogsPacket;
    int _serverCount;
    mutable bool _isLoading;
    uint _endDate;

protected:
    bool canFetchMore(const QModelIndex &parent) const;
    void fetchMore(const QModelIndex &parent);

protected slots:
    void onDialogsLoaded(const DialogsPacket *sender, const DialogList &dialogs);
    void onItemChanged(const int i);

signals:
    void unreadDialogs(const int count);
};

#endif // DIALOGSMODEL_H
