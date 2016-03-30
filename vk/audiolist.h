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

#ifndef AUDIOLIST_H
#define AUDIOLIST_H

#include <QSharedPointer>
#include "observablecollection.h"
#include "audioitem.h"

class AudioListPrivate : public ObservableCollection<AudioItem>
{
    Q_OBJECT

public:
    AudioListPrivate();
};

typedef QSharedPointer<AudioListPrivate> AudioList;

#endif // AUDIOLIST_H
