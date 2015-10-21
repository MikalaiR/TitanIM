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
#include "photospacket.h"

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
    PhotosPacket *_photosPacket;
    int _maxMsgId;
    QList<PhotoItem> _currentPhotosProfile;

public slots:
    int uid() const;
    void setUserOnline(const int uid);
    void setUserOffline(const int uid, const bool isAway);
    void getFriendsOnline();
    void getSelfProfile();
    ProfileItem getProfile() const;
    QVariant getUser();
    ProfileItem getProfile(const int id, ProfileItem defaultValue=ProfileItem());
    QVariant getUser(const int id);
    void getPhotosProfile(const int uid);
    QVariantList photosProfile(const int uid);
    int maxMsgId() const;
    void setMaxMsgId(const int mid);

protected slots:
    void onFriendsOnline(const Packet *sender, const QVariantMap &result);
    void onGetPhotosProfile(const int id, const QList<PhotoItem> &photos);

private slots:
    void onAuthorized(const int uid, const QString &token, const QString &secret);
    void onLogout(const int uid);

signals:
    void photosProfileLoaded(const int uid);
};

#endif // ENGINE_H
