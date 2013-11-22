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

#ifndef ROSTERMODEL_H
#define ROSTERMODEL_H

#include <QAbstractListModel>
#include "vk/client.h"
#include "vk/rosterpacket.h"

class RosterModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum rosterRole
    {
        uidRole = Qt::UserRole,
        onlineRole,
        activityRole,
        alphabetRole
    };

    explicit RosterModel(QObject *parent = 0);
    ~RosterModel();
    void load(const int count=0);
    void loadNext(const int count=20);
    void append(const ProfileList items);
    void replace(const ProfileList items);
    bool remove(int row, int count);
    ProfileItem at(const int row);
    QHash<int, QByteArray> roleNames() const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant& value, int role = Qt::EditRole);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

private:
    ProfileList _roster;
    RosterPacket *_rosterPacket;

protected slots:
    void onRosterLoaded(const RosterPacket *sender, const ProfileList &roster);
    void onItemChanged(const int i);
};

#endif // ROSTERMODEL_H
