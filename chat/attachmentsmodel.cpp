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

    connect(this, &AttachmentsModel::rowsInserted, this, &AttachmentsModel::attachmentsCountChanged);
    connect(this, &AttachmentsModel::rowsRemoved, this, &AttachmentsModel::attachmentsCountChanged);
    connect(this, &AttachmentsModel::modelReset, this, &AttachmentsModel::attachmentsCountChanged);
}

void AttachmentsModel::setAttachments(AttachmentList *attachments)
{
    if (_attachments == 0 && attachments == 0)
        return;

    if (_attachments)
    {
        disconnect(_attachments, &AttachmentList::itemChanged, this, &AttachmentsModel::onItemChanged);
    }

    beginResetModel();
    _attachments = attachments;
    endResetModel();

    if (attachments)
    {
        connect(attachments, &AttachmentList::itemChanged, this, &AttachmentsModel::onItemChanged);
    }
}

bool AttachmentsModel::remove(int row, int count)
{
    if (count <= 0 || row < 0 || row >= rowCount())
        return false;

    beginRemoveRows(QModelIndex(), row, row + count - 1);

    for (int i = 0; i < count; ++i)
    {
        _attachments->removeAt(row);
    }

    endRemoveRows();

    return true;
}

QHash<int, QByteArray> AttachmentsModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractListModel::roleNames();

    roles[TypeRole] = "attachmentType";
    roles[CountFwdMsgRole] = "countFwdMsg";

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
            return "";

        case Qt::DecorationRole:
            return photo->src();
        }

        break;
    }

    case Attachment::Video:
    {
        VideoItem video = qobject_cast<VideoItem>(attachmentBase);

        switch (role)
        {
        case Qt::DisplayRole:
            return video->title();

        case Qt::DecorationRole:
            return "images/upload_video.png";
        }

        break;
    }

    case Attachment::Audio:
    {
        AudioItem audio = qobject_cast<AudioItem>(attachmentBase);

        switch (role)
        {
        case Qt::DisplayRole:
            return audio->title();

        case Qt::DecorationRole:
            return "images/upload_audio.png";
        }

        break;
    }

    case Attachment::Doc:
    {
        DocItem doc = qobject_cast<DocItem>(attachmentBase);

        switch (role)
        {
        case Qt::DisplayRole:
            return doc->title();

        case Qt::DecorationRole:
            return "images/upload_doc.png";
        }

        break;
    }

    case Attachment::Fwd_messages:
    {
        FwdMsgItem fwdMsg = qobject_cast<FwdMsgItem>(attachmentBase);
        int count = fwdMsg->countMessages();

        switch (role)
        {
        case Qt::DisplayRole:
            return Utils::pluralForm(count, QObject::tr("message", "1"), QObject::tr("messages", "2"), QObject::tr("messages", "5"));

        case CountFwdMsgRole:
            return count;
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
