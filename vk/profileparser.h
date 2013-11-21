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

#ifndef PROFILEPARSER_H
#define PROFILEPARSER_H

#include <QVariantMap>
#include "profileitem.h"
#include "profilelist.h"
#include "utils.h"

class ProfileParser : public QObject
{
public:
    static ProfileItem parser(const QVariantMap &item);
    static ProfileList parser(const QVariantList &items);
};

#endif // PROFILEPARSER_H
