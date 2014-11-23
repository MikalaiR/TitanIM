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

#include "attachmentsmodel.h"

AttachmentsModel::AttachmentsModel(QObject *parent) :
    QAbstractListModel(parent)
{
    _attachments = 0;
}

void AttachmentsModel::setAttachments(AttachmentList *attachments)
{
    clear();

    if (_attachments)
    {
        disconnect(_attachments, SIGNAL(itemChanged(int)), this, SLOT(onItemChanged(int)));
    }

    if (attachments && attachments->count())
    {
        beginInsertRows(QModelIndex(), 0, attachments->count() - 1);
        _attachments = attachments;
        connect(_attachments, SIGNAL(itemChanged(int)), this, SLOT(onItemChanged(int)));
        endInsertRows();
    }
    else
    {
        _attachments = 0;
    }
}

void AttachmentsModel::clear()
{
    beginRemoveRows(QModelIndex(), 0, rowCount());
    endRemoveRows();
}

QHash<int, QByteArray> AttachmentsModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractListModel::roleNames();

    roles[TypeRole] = "attachmentType";

    return roles;
}

QVariant AttachmentsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= rowCount())
    {
        return QVariant();
    }

    AttachmentItem attachmentBase = _attachments->at(index.row());

    //global role
    switch (role)
    {
    case TypeRole:
        return attachmentBase->attachmentType();
    }

    //type role
    switch (attachmentBase->attachmentType()) {
    case Attachment::Photo:
    {
        PhotoItem photo = qobject_cast<PhotoItem>(attachmentBase);

        switch (role)
        {
        case Qt::DisplayRole:
            return "Photo";

        case Qt::DecorationRole:
            return photo->src();
        }

        break;
    }
    }

    return QVariant();
}

bool AttachmentsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() && !value.isValid() && index.row() >= rowCount())
    {
        return false;
    }

    return false;
}

int AttachmentsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return _attachments ? _attachments->count() : 0;
}

Qt::ItemFlags AttachmentsModel::flags(const QModelIndex &index) const
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

void AttachmentsModel::onItemChanged(const int i)
{
    QModelIndex idx = index(i, 0);
    emit dataChanged(idx, idx);
}
