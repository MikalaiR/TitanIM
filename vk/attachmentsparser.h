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

#ifndef ATTACHMENTSPARSER_H
#define ATTACHMENTSPARSER_H

#include <QObject>
#include <QVariant>
#include <QMetaEnum>
#include <QDebug>
#include "attachmentlist.h"
#include "photoparser.h"

class AttachmentsParser : public QObject
{
public:
    static AttachmentList* parser(const QVariantList &items);
};

#endif // ATTACHMENTSPARSER_H
