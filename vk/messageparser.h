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

#ifndef MESSAGEPARSER_H
#define MESSAGEPARSER_H

#include <QVariantMap>
#include "messageitem.h"
#include "utils.h"

class MessageParser : public QObject
{
public:
    static MessageItem* parser(QVariantMap json);
};

#endif // MESSAGEPARSER_H
