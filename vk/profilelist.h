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

#ifndef PROFILELIST_H
#define PROFILELIST_H

#include <QObject>
#include <QSharedPointer>
#include <QVector>
#include "profileitem.h"

class ProfileListPrivate : public QObject
{
    Q_OBJECT

public:
    ProfileListPrivate();
    ~ProfileListPrivate();
    void add(ProfileItem profile);
    void add(const QVector<ProfileItem> &profiles);
    int indexOf(const int uid) const;
    ProfileItem at(const int i) const;
    ProfileItem item(const int uid) const;
    void remove(const int i);
    int count() const;
    QVector<ProfileItem> toVector() const;

private:
    QVector<ProfileItem> _profiles;

protected slots:
    void onItemChanged(const int uid, const QString &propertyName);

signals:
    void itemChanged(const int i);
};

typedef QSharedPointer<ProfileListPrivate> ProfileList;

#endif // PROFILELIST_H
