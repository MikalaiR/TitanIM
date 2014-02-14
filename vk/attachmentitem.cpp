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
    _attachmentType = unknown;
}

Attachment::AttachmentType Attachment::attachmentType() const
{
    return _attachmentType;
}

void Attachment::setAttachmentType(const AttachmentType attachmentType)
{
    if (_attachmentType != attachmentType)
    {
        _attachmentType = attachmentType;
        emitPropertyChanged("attachmentType");
    }
}
