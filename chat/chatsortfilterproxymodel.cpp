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

#include "chatsortfilterproxymodel.h"

ChatSortFilterProxyModel::ChatSortFilterProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
    QSortFilterProxyModel::sort(0, Qt::DescendingOrder);
}

bool ChatSortFilterProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    int l = sourceModel()->data(left, ChatModel::MessageTypeRole).toInt();
    int r = sourceModel()->data(right, ChatModel::MessageTypeRole).toInt();

    if (l == MessageBase::Text && r == MessageBase::Text)
    {
        int l = sourceModel()->data(left, ChatModel::IdRole).toInt();
        int r = sourceModel()->data(right, ChatModel::IdRole).toInt();

        if ((l > 0 && r > 0) || (l < 0 && r < 0))
        {
            return abs(l) < abs(r);
        }
        else
        {
            return r < 0; //not bug
        }
    }
    else
    {
        QDateTime l = sourceModel()->data(left, ChatModel::DateRole).toDateTime();
        QDateTime r = sourceModel()->data(right, ChatModel::DateRole).toDateTime();

        return l < r;
    }
}

bool ChatSortFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    const QVariant &data = sourceModel()->index(source_row, 0, source_parent).data(ChatModel::DeletedRole);
    return !data.toBool();
}
