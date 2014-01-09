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

#include <QObject>
#include <QSharedPointer>
#include <QVector>
#include "dialogitem.h"

class DialogListPrivate : public QObject
{
    Q_OBJECT

public:
    DialogListPrivate();
    ~DialogListPrivate();
    void add(DialogItem dialog);
    void add(const QVector<DialogItem> &dialogs);
    bool replace(const DialogItem item);
    void replace(const int i, const DialogItem item);
    int indexOf(const int id) const;
    DialogItem at(const int i) const;
    DialogItem item(const int id) const;
    void remove(const int i);
    int count() const;
    QVector<DialogItem> toVector() const;

private:
    QVector<DialogItem> _dialogs;

protected slots:
    void onItemChanged(const int id, const QString &propertyName);

signals:
    void itemChanged(const int i);
};

typedef QSharedPointer<DialogListPrivate> DialogList;

#endif // DIALOGLIST_H
