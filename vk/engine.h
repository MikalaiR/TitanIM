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

#ifndef ENGINE_H
#define ENGINE_H

#include <QObject>
#include <QQmlEngine>
#include <QHash>
#include "connection.h"
#include "profileitem.h"

class Engine : public QObject
{
    Q_OBJECT

public:
    Engine(Connection *connection);
    ~Engine();

private:
    Connection *_connection;
    ProfileItem _selfProfile;
    QHash<int, ProfileItem> *_profiles;
    int _maxMsgId;

public slots:
    int uid() const;
    void setUserOnline(const int uid);
    void setUserOffline(const int uid, const bool isAway);
    void getFriendsOnline();
    ProfileItem getProfile() const;
    QVariant getUser();
    ProfileItem getProfile(const int id, ProfileItem defaultValue=ProfileItem());
    QVariant getUser(const int id);
    int maxMsgId() const;
    void setMaxMsgId(const int mid);

protected slots:
    void onFriendsOnline(const Packet *sender, const QVariantMap &result);

private slots:
    void onAuthorized(const int uid, const QString &token, const QString &secret);
    void onLogout(const int uid);
};

#endif // ENGINE_H
