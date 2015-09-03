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

#ifndef DIALOGPARSER_H
#define DIALOGPARSER_H

#include <QObject>
#include "client.h"
#include "dialogitem.h"
#include "dialoglist.h"
#include "messageparser.h"
#include "utils.h"

class DialogItemPrivate;

class DialogParser : public QObject
{
private:
    friend class DialogItemPrivate;
    static void parser(const QVariantMap &item, DialogItemPrivate *dialog, const ProfileList &profiles);

public:
    static void parser(const QVariantMap &item, DialogItem dialog, const ProfileList &profiles);
    static DialogItem parser(const QVariantMap &item, const ProfileList &profiles);
    static DialogList parser(const QVariantList &items, const ProfileList &profiles);
};

#endif // DIALOGPARSER_H
