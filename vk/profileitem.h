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
    Q_PROPERTY(QString fullName READ fullName NOTIFY fullNameChanged)
    Q_PROPERTY(QString photoMediumRect READ photoMediumRect NOTIFY photoMediumRectChanged)
    Q_PROPERTY(int sex READ sex NOTIFY sexChanged)
    Q_PROPERTY(bool online READ online WRITE setOnline NOTIFY onlineChanged)
    Q_PROPERTY(QString bdate READ bdate NOTIFY bdateChanged)
    Q_PROPERTY(QString domain READ domain NOTIFY domainChanged)
    Q_PROPERTY(QString city READ city NOTIFY cityChanged)
    Q_PROPERTY(QString mobilePhone READ mobilePhone NOTIFY mobilePhoneChanged)

public:
    enum Sex
    {
        Unknown,
        Woman,
        Man
    };

    ProfileItemPrivate();
    QString firstName() const;
    void setFirstName(const QString &firstName);
    QString lastName() const;
    void setLastName(const QString &lastName);
    QString fullName() const;
    void setFullName(const QString &firstName, const QString &lastName="");
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
    QString bdate() const;
    void setBdate(const QString &bdate);
    QString domain() const;
    void setDomain(const QString &domain);
    QString city() const;
    void setCity(const QString &city);
    QString mobilePhone() const;
    void setMobilePhone(const QString &mobilePhone);
    QString alphabet() const;
    void setAlphabet(const QString &alphabet);
    bool isLoading() const;
    bool isEmpty() const;

private:
    QString _firstName;
    QString _lastName;
    Sex _sex;
    QString _photoMediumRect;
    bool _online;
    int _lastSeen;
    QString _activity;
    QString _bdate;
    QString _domain;
    QString _city;
    QString _mobilePhone;
    QString _alphabet;
    bool _isLoading;

public slots:
    void join(const ProfileItem other);
    void getAllFields(Connection *connection);
    void getLastActivity(Connection *connection);
    void updateLastSeenText();

protected slots:
    void setIsLoading(const bool isLoading);
    void loadFinished(const Packet *sender, const QVariantMap &result);
    void getLastActivityFinished(const Packet *sender, const QVariantMap &result);

signals:
    void fullNameChanged();
    void photoMediumRectChanged();
    void sexChanged();
    void onlineChanged();
    void bdateChanged();
    void domainChanged();
    void cityChanged();
    void mobilePhoneChanged();
};

#endif // PROFILEITEM_H
