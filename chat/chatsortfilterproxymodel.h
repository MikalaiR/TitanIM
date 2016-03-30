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

#ifndef CHATSORTFILTERPROXYMODEL_H
#define CHATSORTFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QDateTime>
#include "chatmodel.h"

class ChatSortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    ChatSortFilterProxyModel(QObject *parent = 0);

protected:
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const;
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
};

#endif // CHATSORTFILTERPROXYMODEL_H
