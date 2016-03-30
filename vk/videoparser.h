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

#ifndef VIDEOPARSER_H
#define VIDEOPARSER_H

#include <QObject>
#include <QVariantMap>
#include "videoitem.h"

class VideoParser : public QObject
{
public:
    static VideoItem parser(const QVariantMap &item);
};

#endif // VIDEOPARSER_H
