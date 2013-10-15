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
#include "global.h"

class ProfileItem : public QObject
{
    Q_OBJECT

public:
    ProfileItem();
    int uid();
    void setUid(const int uid);
    QString firstName();
    void setFirstName(const QString &firstName);
    QString lastName();
    void setLastName(const QString &lastName);
    QString fullName();
    Sex sex();
    void setSex(const Sex sex);
    QString photoMediumRect();
    void setPhotoMediumRect(const QString &photoMediumRect);
    bool online();
    void setOnline(const bool online);
    int lastSeen();
    void setLastSeen(const int lastSeen);

private:
    int _uid;
    QString _firstName;
    QString _lastName;
    Sex _sex;
    QString _photoMediumRect;
    bool _online;
    int _lastSeen;
};

#endif // PROFILEITEM_H
