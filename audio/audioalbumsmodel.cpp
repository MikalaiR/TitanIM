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

#include "audioalbumsmodel.h"

AudioAlbumsModel::AudioAlbumsModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

AudioAlbumsModel::~AudioAlbumsModel()
{
}

void AudioAlbumsModel::append(const QVector<QPair<int, QString> > &items)
{
    if (!items.count())
        return;

    beginInsertRows(QModelIndex(), _items.count(), _items.count() + items.count() - 1);
    _items.append(items);
    endInsertRows();
}

void AudioAlbumsModel::append(const QPair<int, QString> &item)
{
    beginInsertRows(QModelIndex(), _items.count(), _items.count());
    _items.append(item);
    endInsertRows();
}

void AudioAlbumsModel::replaceAll(const QVector<QPair<int, QString> > &items)
{
    removeAll();
    append(items);
}

bool AudioAlbumsModel::remove(int row, int count)
{
    if (count <= 0 || row < 0)
        return false;

    beginRemoveRows(QModelIndex(), row, row + count - 1);

    for (int i = 0; i < count; ++i)
    {
        _items.removeAt(row);
    }

    endRemoveRows();

    return true;
}

void AudioAlbumsModel::removeAll()
{
    beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
    _items.clear();
    endRemoveRows();
}

QPair<int, QString> AudioAlbumsModel::at(const int row)
{
    return _items.at(row);
}

QHash<int, QByteArray> AudioAlbumsModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractListModel::roleNames();

    roles[AlbumIdRole] = "albumId";
    roles[SectionRole] = "section";

    return roles;
}

QVariant AudioAlbumsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= _items.count())
    {
        return QVariant();
    }

    QPair<int, QString> album = _items.at(index.row());

    switch (role)
    {
    case Qt::DisplayRole:
        return album.second;

    case Qt::DecorationRole:
        return "images/playlist.png";

    case AlbumIdRole:
        return album.first;

    case SectionRole:
        return index.row() < 4 ? "" : tr("Albums");
    }

    return QVariant();
}

bool AudioAlbumsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() && !value.isValid() && index.row() >= _items.count())
    {
        return false;
    }

    return false;
}

int AudioAlbumsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return _items.count();
}

Qt::ItemFlags AudioAlbumsModel::flags(const QModelIndex &index) const
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
