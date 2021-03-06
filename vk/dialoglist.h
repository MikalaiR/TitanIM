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

#ifndef DIALOGLIST_H
#define DIALOGLIST_H

#include <QSharedPointer>
#include "observablecollection.h"
#include "dialogitem.h"

class DialogListPrivate : public ObservableCollection<DialogItem>
{
    Q_OBJECT

public:
    DialogListPrivate();
};

typedef QSharedPointer<DialogListPrivate> DialogList;

#endif // DIALOGLIST_H
