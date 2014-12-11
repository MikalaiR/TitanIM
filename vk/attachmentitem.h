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

#ifndef ATTACHMENTITEM_H
#define ATTACHMENTITEM_H

#include <QObject>
#include <QSharedPointer>
#include <QMetaEnum>
#include "notifypropertybase.h"

class Attachment : public NotifyPropertyBase
{
    Q_OBJECT
    Q_ENUMS(AttachmentType)
    Q_PROPERTY(AttachmentType type READ attachmentType CONSTANT)
    Q_PROPERTY(int ownerId READ ownerId) //todo
    Q_PROPERTY(int uploadProgress READ uploadProgress NOTIFY uploadProgressChanged)

public:
    enum AttachmentType
    {
        Unknown = 0,
        Photo = 1,
        Video = 2,
        Audio = 4,
        Doc = 8,
        Sticker = 16,
        Map = 32,
        Fwd_messages = 64
    };

    Attachment();
    AttachmentType attachmentType() const;
    int ownerId() const;
    void setOwnerId(const int ownerId);
    int uploadProgress() const;
    void setUploadProgress(const int progress);
    bool isUploading() const;
    QString toString() const;

protected:
    void setAttachmentType(const AttachmentType attachmentType);

private:
    AttachmentType _attachmentType;
    int _ownerId;
    int _uploadProgress;

signals:
    void uploadProgressChanged();
};

typedef QSharedPointer<Attachment> AttachmentItem;

#endif // ATTACHMENTITEM_H
