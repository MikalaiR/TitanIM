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

#ifndef NOTIFICATOR_H
#define NOTIFICATOR_H

#include <QObject>
#include <QPixmap>
#include <QtMultimedia/QMediaPlayer>
#include "vk/utils.h"
#include "settings.h"
#include <memory>

#if defined(Q_OS_LINUX)
#define LINUX_NATIVE_ASEMAN_NOTIFICATION
#elif defined(Q_OS_MAC)
#define MAC_NATIVE_ASEMAN_NOTIFICATION
#else
#define NATIVE_ASEMAN_NOTIFICATION
#endif

#include "asemantools/asemannotification.h"

class Notificator : public QObject
{
    Q_OBJECT

public:
    static Notificator *instance();
    static void destroy();
    void showNotification(const int peer, const int mid, const QString &title, const QString &message, const bool withReply=false, const QPixmap &pixmap=QPixmap());

private:
    Notificator();
    ~Notificator();

public slots:
    void playSoundMessageIn();
    void setBadge(const int count);

private:
    QMap<uint, QPair<int, int>> nl;
    static Notificator *aInstance;
    std::unique_ptr<AsemanNotification> p;
    std::unique_ptr<QMediaPlayer> player;

private slots:
    void notifyAction(uint id, const QString &act);

signals:
    void notificationClicked(const int peer, const int mid);
    void notificationReplied(const int peer, const int mid, const QString &response);
};

#endif // NOTIFICATOR_H
