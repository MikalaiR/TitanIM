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

#ifndef AUDIOPARSER_H
#define AUDIOPARSER_H

#include <QObject>
#include <QVariantMap>
#include "audioitem.h"
#include "audiolist.h"

class AudioParser : public QObject
{
public:
    static AudioItem parser(const QVariantMap &item);
    static AudioList parser(const QVariantList &items);
};

#endif // AUDIOPARSER_H
