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

#include "audiomodel.h"

AudioModel::AudioModel(QObject *parent) :
    QAbstractListModel(parent)
{
    _items = AudioList::create();
    connect(_items.data(), &AudioListPrivate::itemChanged, this, &AudioModel::onItemChanged);
}

AudioModel::~AudioModel()
{
}

void AudioModel::append(const AudioList items)
{
    if (!items->count())
        return;

    beginInsertRows(QModelIndex(), _items->count(), _items->count() + items->count() - 1);
    _items->add(items->toList());
    endInsertRows();
}

void AudioModel::append(const AudioItem item, const bool replace)
{
    if (replace && _items->replace(item))
    {
        return;
    }

    beginInsertRows(QModelIndex(), _items->count(), _items->count());
    _items->add(item);
    endInsertRows();
}

void AudioModel::replaceAll(const AudioList items)
{
    removeAll();
    append(items);
}

bool AudioModel::remove(int row, int count)
{
    if (count <= 0 || row < 0)
        return false;

    beginRemoveRows(QModelIndex(), row, row + count - 1);

    for (int i = 0; i < count; ++i)
    {
        _items->removeAt(row);
    }

    endRemoveRows();

    return true;
}

void AudioModel::removeAll()
{
    beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
    _items->clear();
    endRemoveRows();
}

AudioItem AudioModel::at(const int row)
{
    return _items->at(row);
}

QHash<int, QByteArray> AudioModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractListModel::roleNames();

    roles[ArtistRole] = "artist";
    roles[DurationRole] = "duration";
    roles[DurationStrRole] = "durationStr";
    roles[UrlRole] = "url";

    return roles;
}

QVariant AudioModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= _items->count())
    {
        return QVariant();
    }

    AudioItem audio = _items->at(index.row());

    switch (role)
    {
    case Qt::DisplayRole:
        return audio->title();

    case ArtistRole:
        return audio->artist();

    case DurationRole:
        return audio->duration();

    case DurationStrRole:
        return audio->durationStr();

    case UrlRole:
        return audio->url();
    }

    return QVariant();
}

bool AudioModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() && !value.isValid() && index.row() >= _items->count())
    {
        return false;
    }

    return false;
}

int AudioModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return _items->count();
}

Qt::ItemFlags AudioModel::flags(const QModelIndex &index) const
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

QList<AudioItem> AudioModel::toList() const
{
    return _items->toList();
}

void AudioModel::onItemChanged(const int i)
{
    QModelIndex idx = index(i, 0);
    emit dataChanged(idx, idx);
}
