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
    int leftTypy = sourceModel()->data(left, ChatModel::MessageTypeRole).toInt();
    int rightType = sourceModel()->data(right, ChatModel::MessageTypeRole).toInt();

    if (leftTypy == MessageBase::Text && rightType == MessageBase::Text)
    {
        bool leftIsError = sourceModel()->data(left, ChatModel::IsError).toBool();
        bool rightIsError = sourceModel()->data(right, ChatModel::IsError).toBool();

        if (!leftIsError && !rightIsError) //sort by id
        {
            int leftId = sourceModel()->data(left, ChatModel::IdRole).toInt();
            int rightId = sourceModel()->data(right, ChatModel::IdRole).toInt();

            if ((leftId > 0 && rightId > 0) || (leftId < 0 && rightId < 0))
            {
                return abs(leftId) < abs(rightId);
            }
            else
            {
                return rightId < 0; //not bug
            }
        }
    }

    //sort by date
    QDateTime leftDate = sourceModel()->data(left, ChatModel::DateRole).toDateTime();
    QDateTime rightDate = sourceModel()->data(right, ChatModel::DateRole).toDateTime();

    return leftDate < rightDate;
}

bool ChatSortFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    const QVariant &data = sourceModel()->index(source_row, 0, source_parent).data(ChatModel::DeletedRole);
    return !data.toBool();
}
