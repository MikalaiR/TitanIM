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

#ifndef AUDIOMODEL_H
#define AUDIOMODEL_H

#include <QAbstractListModel>
#include "vk/audiolist.h"

class AudioModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum dialogsRole
    {
        ArtistRole = Qt::UserRole,
        DurationRole,
        DurationStrRole,
        UrlRole
    };

    explicit AudioModel(QObject *parent = 0);
    ~AudioModel();
    void append(const AudioList items);
    void append(const AudioItem item, const bool replace=false);
    void replaceAll(const AudioList items);
    bool remove(int row, int count);
    void removeAll();
    AudioItem at(const int row);
    QHash<int, QByteArray> roleNames() const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant& value, int role = Qt::EditRole);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QList<AudioItem> toList() const;

private:
    AudioList _items;

protected slots:
    void onItemChanged(const int i);
};

#endif // AUDIOMODEL_H
