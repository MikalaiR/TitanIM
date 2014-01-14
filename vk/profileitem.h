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

#ifndef PROFILEITEM_H
#define PROFILEITEM_H

#include <QObject>
#include <QSharedPointer>
#include "global.h"

class ProfileItemPrivate : public QObject
{
    Q_OBJECT

public:
    ProfileItemPrivate();
    int id() const;
    void setId(const int id);
    QString firstName() const;
    void setFirstName(const QString &firstName);
    QString lastName() const;
    void setLastName(const QString &lastName);
    QString fullName() const;
    Sex sex() const;
    void setSex(const Sex sex);
    QString photoMediumRect() const;
    void setPhotoMediumRect(const QString &photoMediumRect);
    bool online() const;
    void setOnline(const bool online);
    int lastSeen() const;
    void setLastSeen(const int lastSeen);
    QString activity() const;
    void setActivity(const QString &activity);
    QString alphabet() const;
    void setAlphabet(const QString &alphabet);

private:
    int _id;
    QString _firstName;
    QString _lastName;
    Sex _sex;
    QString _photoMediumRect;
    bool _online;
    int _lastSeen;
    QString _activity;
    QString _alphabet;

signals:
    void propertyChanged(const int id, const QString &propertyName);
};

typedef QSharedPointer<ProfileItemPrivate> ProfileItem;

#endif // PROFILEITEM_H
