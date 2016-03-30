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
    Q_ENUMS(Extension)
    Q_PROPERTY(AttachmentType type READ attachmentType CONSTANT)
    Q_PROPERTY(int ownerId READ ownerId) //todo
    Q_PROPERTY(int uploadProgress READ uploadProgress NOTIFY uploadProgressChanged)
    Q_PROPERTY(bool isUploadError READ isUploadError NOTIFY isUploadErrorChanged)

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
        Fwd_messages = 64,
        Gift = 128,
        Wall = 256,
        Link = 512
    };

    enum Extension
    {
        Jpeg = 1,
        Jpg = 2,
        Png = 4,
        Bmp = 8,
        Gif = 16,
        Avi = 32,
        Mpg = 64,
        Mp3 = 128
    };

    Attachment();
    AttachmentType attachmentType() const;
    int ownerId() const;
    void setOwnerId(const int ownerId);
    int uploadProgress() const;
    bool isUploading() const;
    bool isUploadError() const;
    QString uploadError() const;
    void setUploadError(const QString &msg);
    QString toString() const;
    static QMetaEnum metaEnumerator(const QString &name);

protected:
    void setAttachmentType(const AttachmentType attachmentType);

private:
    AttachmentType _attachmentType;
    int _ownerId;
    int _uploadProgress;
    QString _uploadError;

public slots:
    void setUploadProgress(const int progress);

signals:
    void uploadProgressChanged();
    void isUploadErrorChanged();
};

typedef QSharedPointer<Attachment> AttachmentItem;

#endif // ATTACHMENTITEM_H
