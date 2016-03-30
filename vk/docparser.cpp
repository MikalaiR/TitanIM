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

#include "docparser.h"

DocItem DocParser::parser(const QVariantMap &item)
{
    DocItem doc = DocItem::create();

    int id = item.value("id").toInt();
    int ownerId = item.value("owner_id").toInt();
    QString title = item.value("title").toString();
    int size = item.value("size").toInt();
    QString ext = item.value("ext").toString();
    QUrl url = item.value("url").toUrl();
    QUrl thumb = item.value("photo_130").toUrl();

    doc->setId(id);
    doc->setOwnerId(ownerId);
    doc->setTitle(title);
    doc->setSize(size);
    doc->setExt(ext);
    doc->setUrl(url);
    doc->setThumb(thumb);

    return doc;
}
