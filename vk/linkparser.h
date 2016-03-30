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

#ifndef LINKPARSER_H
#define LINKPARSER_H

#include <QObject>
#include <QVariantMap>
#include "linkitem.h"

class LinkParser : public QObject
{
public:
    static LinkItem parser(const QVariantMap &item);
};

#endif // LINKPARSER_H
