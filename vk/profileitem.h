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
#include "notifypropertybase.h"
#include "global.h"
#include "connection.h"

class ProfileItemPrivate;
typedef QSharedPointer<ProfileItemPrivate> ProfileItem;

class ProfileItemPrivate : public NotifyPropertyBase
{
    Q_OBJECT
    Q_PROPERTY(QString fullName READ fullName NOTIFY fullNameChanged) //todo
    Q_PROPERTY(QString photoMediumRect READ photoMediumRect NOTIFY photoMediumRectChanged) //todo

public:
    ProfileItemPrivate();
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
    QString lastSeenText() const;
    QString activity() const;
    void setActivity(const QString &activity);
    QString alphabet() const;
    void setAlphabet(const QString &alphabet);
    bool isLoading() const;

private:
    QString _firstName;
    QString _lastName;
    Sex _sex;
    QString _photoMediumRect;
    bool _online;
    int _lastSeen;
    QString _activity;
    QString _alphabet;
    bool _isLoading;

public slots:
    void join(const ProfileItem other);
    void getAllFields(Connection *connection);
    void updateLastSeenText();

protected slots:
    void setIsLoading(const bool isLoading);
    void loadFinished(const Packet *sender, const QVariantMap &result);

signals:
    void fullNameChanged();
    void photoMediumRectChanged();
};

#endif // PROFILEITEM_H
