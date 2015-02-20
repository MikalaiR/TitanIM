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

#ifndef ROSTERHANDLER_H
#define ROSTERHANDLER_H

#include <QObject>
#include <QSortFilterProxyModel>
#include "rostermodel.h"
#include "vk/client.h"

class RosterHandler : public QObject
{
    Q_OBJECT

    Q_PROPERTY(RosterModel* model READ model CONSTANT)
    Q_PROPERTY(QSortFilterProxyModel* proxy READ proxy CONSTANT)

public:
    RosterHandler();
    ~RosterHandler();
    RosterModel* model() const;
    QSortFilterProxyModel* proxy() const;
    ProfileItem profile(const int index, const bool isProxyIndex=true) const;
    Q_INVOKABLE int indexOf(const int id, const bool proxyIndex=true) const;
    ProfileItem item(const int id) const;

private:
    RosterModel *_model;
    QSortFilterProxyModel *_proxy;

public slots:
    void getFriendsOnline();

protected slots:
    void setFilterWildcard(const QString &pattern);
    void onUserStatusChanged(const int uid, const bool online);
    void onFriendsOnline(const Packet *sender, const QVariantMap &result);
};

#endif // ROSTERHANDLER_H
