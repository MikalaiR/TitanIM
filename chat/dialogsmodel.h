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
        bodyRole = Qt::UserRole,
        dateRole,
        dateStrRole,
        uidRole,
        midRole,
        isUnreadRole,
        isOutRole,
        onlineRole
    };

    explicit DialogsModel(QObject *parent = 0);
    ~DialogsModel();
    void load(const int count=20);
    void loadNext(const int count=20);
    void append(const MessageList items);
    void replace(const MessageList items);
    bool remove(int row, int count);
    QHash<int, QByteArray> roleNames() const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant& value, int role = Qt::EditRole);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

private:
    MessageList _dialogs;
    DialogsPacket *_dialogsPacket;

protected slots:
    void onDialogsLoaded(const DialogsPacket *sender, const MessageList &dialogs);
};

#endif // DIALOGSMODEL_H
