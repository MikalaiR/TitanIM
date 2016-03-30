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

#include "notifypropertybase.h"

NotifyPropertyBase::NotifyPropertyBase()
{
    _id = 0;
    _blockSignalPropertyChanged = false;
}

void NotifyPropertyBase::setId(const int id)
{
    if (_id != id)
    {
        _id = id;
        emitPropertyChanged("id");
    }
}

void NotifyPropertyBase::beginChangeGroupProperties()
{
    _blockSignalPropertyChanged = true;
}

void NotifyPropertyBase::endChangeGroupProperties()
{
    if (_blockSignalPropertyChanged)
    {
        _blockSignalPropertyChanged = false;
        emit propertyChanged(_id, "all");
    }
}
