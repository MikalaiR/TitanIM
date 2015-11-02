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

#include "linkparser.h"

LinkItem LinkParser::parser(const QVariantMap &item)
{
    LinkItem link = LinkItem::create();

    QString title = item.value("title").toString();
    QString description = item.value("description").toString();
    QUrl url = item.value("url").toUrl();

    link->setTitle(title);
    link->setDescription(description);
    link->setUrl(url);

    return link;
}
