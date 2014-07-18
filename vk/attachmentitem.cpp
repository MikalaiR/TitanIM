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

#include "attachmentitem.h"

Attachment::Attachment()
{
    _attachmentType = Unknown;
    _ownerId = 0;
    _uploadProgress = 0;
}

Attachment::AttachmentType Attachment::attachmentType() const
{
    return _attachmentType;
}

int Attachment::ownerId() const
{
    return _ownerId;
}

void Attachment::setOwnerId(const int ownerId)
{
    if (_ownerId != ownerId)
    {
        _ownerId = ownerId;
        emitPropertyChanged("ownerId");
    }
}

void Attachment::setAttachmentType(const AttachmentType attachmentType)
{
    if (_attachmentType != attachmentType)
    {
        _attachmentType = attachmentType;
        emitPropertyChanged("attachmentType");
    }
}

int Attachment::uploadProgress() const
{
    return _uploadProgress;
}

void Attachment::setUploadProgress(const int progress)
{
    if (_uploadProgress != progress)
    {
        _uploadProgress = progress;
        emitPropertyChanged("uploadProgress");
    }
}

bool Attachment::isUploading() const
{
    return (_uploadProgress > 0) && (_uploadProgress < 100);
}

QString Attachment::toString() const
{
    QMetaEnum metaEnum = staticMetaObject.enumerator(staticMetaObject.indexOfEnumerator("AttachmentType"));
    QString type = metaEnum.valueToKey(_attachmentType);

    return QString("%1%2_%3") //serialization vk
            .arg(type)
            .arg(_ownerId)
            .arg(_id);
}
