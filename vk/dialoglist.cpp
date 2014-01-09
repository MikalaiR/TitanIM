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

#include "dialoglist.h"

DialogListPrivate::DialogListPrivate()
{
}

DialogListPrivate::~DialogListPrivate()
{

}

void DialogListPrivate::add(DialogItem dialog)
{
    _dialogs.append(dialog);
    connect(dialog.data(), SIGNAL(propertyChanged(int,QString)), this, SLOT(onItemChanged(int,QString)));
}

void DialogListPrivate::add(const QVector<DialogItem> &dialogs)
{
    for (int i = 0; i < dialogs.count(); i++)
    {
        add(dialogs.at(i));
    }
}

bool DialogListPrivate::replace(const DialogItem item)
{
    int i = indexOf(item->id());

    if (i > -1)
    {
        replace(i, item);
        return true;
    }

    return false;
}

void DialogListPrivate::replace(const int i, const DialogItem item)
{
    disconnect(_dialogs.at(i).data(), SIGNAL(propertyChanged(int,QString)), this, SLOT(onItemChanged(int,QString)));
    _dialogs.replace(i, item);
    connect(item.data(), SIGNAL(propertyChanged(int,QString)), this, SLOT(onItemChanged(int,QString)));

    emit itemChanged(i);
}

int DialogListPrivate::indexOf(const int id) const
{
    for (int i = 0; i < _dialogs.count(); i++)
    {
        if (_dialogs.at(i)->id() == id)
            return i;
    }

    return -1;
}

DialogItem DialogListPrivate::at(const int i) const
{
    return _dialogs.at(i);
}

DialogItem DialogListPrivate::item(const int id) const
{
    foreach (DialogItem item, _dialogs)
    {
        if (item->id() == id)
            return item;
    }

    return DialogItem();
}

void DialogListPrivate::remove(const int i)
{
    disconnect(_dialogs.at(i).data(), SIGNAL(propertyChanged(int,QString)), this, SLOT(onItemChanged(int,QString)));
    _dialogs.remove(i);
}

int DialogListPrivate::count() const
{
    return _dialogs.count();
}

QVector<DialogItem> DialogListPrivate::toVector() const
{
    return _dialogs;
}

void DialogListPrivate::onItemChanged(const int id, const QString &propertyName)
{
    emit itemChanged(indexOf(id));
}
