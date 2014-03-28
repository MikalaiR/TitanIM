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

#include "rostermodel.h"

RosterModel::RosterModel(QObject *parent) :
    QAbstractListModel(parent)
{
    _roster = ProfileList::create();
    connect(_roster.data(), SIGNAL(itemChanged(int)), this, SLOT(onItemChanged(int)));

    _rosterPacket = new RosterPacket(Client::instance()->connection());
    _rosterPacket->setNeedFavorites(true);
    connect(_rosterPacket, SIGNAL(roster(const RosterPacket*,const ProfileList)), SLOT(onRosterLoaded(const RosterPacket*,const ProfileList)));
}

RosterModel::~RosterModel()
{
    delete _rosterPacket;
}

void RosterModel::load(const int count)
{
    _rosterPacket->load(0, count);
}

void RosterModel::loadNext(const int count)
{
    _rosterPacket->load(_roster->count(), count);
}

void RosterModel::append(const ProfileList items)
{
    if (!items->count())
        return;

    beginInsertRows(QModelIndex(), _roster->count(), _roster->count() + items->count() - 1);
    _roster->add(items->toList());
    endInsertRows();
}

void RosterModel::replaceAll(const ProfileList items)
{
    remove(0, rowCount());
    append(items);
}

bool RosterModel::remove(int row, int count)
{
    if (count <= 0 || row < 0)
        return false;

    beginRemoveRows(QModelIndex(), row, row + count - 1);

    for (int i = 0; i < count; ++i)
    {
        _roster->removeAt(row);
    }

    endRemoveRows();

    return true;
}

ProfileItem RosterModel::at(const int row)
{
    return _roster->at(row);
}

ProfileItem RosterModel::at(const QModelIndex &index)
{
    return _roster->at(index.row());
}

int RosterModel::indexOf(const int id) const
{
    return _roster->indexOf(id);
}

QHash<int, QByteArray> RosterModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractListModel::roleNames();

    roles[uidRole] = "uid";
    roles[onlineRole] = "online";
    roles[activityRole] = "activity";
    roles[alphabetRole] = "alphabet";

    return roles;
}

QVariant RosterModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= _roster->count())
    {
        return QVariant();
    }

    ProfileItem profile = _roster->at(index.row());

    switch (role)
    {
    case Qt::DisplayRole:
        return profile->fullName().isEmpty() ? QString::number(profile->id()) : profile->fullName();

    case Qt::DecorationRole:
        return profile->photoMediumRect();

    case uidRole:
        return profile->id();

    case onlineRole:
        return profile->online();

    case activityRole:
        return profile->activity();

    case alphabetRole:
        return profile->alphabet();
    }

    return QVariant();
}

bool RosterModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() && !value.isValid() && index.row() >= _roster->count())
    {
        return false;
    }

    return false;
}

int RosterModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return _roster->count();
}

Qt::ItemFlags RosterModel::flags(const QModelIndex &index) const
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

void RosterModel::onRosterLoaded(const RosterPacket *sender, const ProfileList &roster)
{
    if (!sender->offset())
    {
        replaceAll(roster);
    }
    else
    {
        append(roster);
    }
}

void RosterModel::onItemChanged(const int i)
{
    QModelIndex idx = index(i, 0);
    emit dataChanged(idx, idx);
}
