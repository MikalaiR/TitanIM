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

#ifndef AUDIOALBUMSMODEL_H
#define AUDIOALBUMSMODEL_H

#include <QObject>
#include <QAbstractListModel>

class AudioAlbumsModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum albumsRole
    {
        AlbumIdRole = Qt::UserRole,
        SectionRole
    };

    AudioAlbumsModel(QObject *parent = 0);
    ~AudioAlbumsModel();
    void append(const QVector<QPair<int, QString>> &items);
    void append(const QPair<int, QString> &item);
    void replaceAll(const QVector<QPair<int, QString>> &items);
    bool remove(int row, int count);
    void removeAll();
    QPair<int, QString> at(const int row);
    QHash<int, QByteArray> roleNames() const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant& value, int role = Qt::EditRole);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

private:
    QVector<QPair<int, QString>> _items;
};

#endif // AUDIOALBUMSMODEL_H
