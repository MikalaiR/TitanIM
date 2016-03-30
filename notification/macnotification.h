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

#ifndef MACNOTIFICATION
#define MACNOTIFICATION

#include <QObject>
#include <QtMac>
#include <QPixmap>
#include <Cocoa/Cocoa.h>

class MacNotification : public QObject
{
    Q_OBJECT

public:
    static MacNotification *instance();
    static void destroy();
    void showNotification(const int peer, const int mid, const QString &title, const QString &message, const bool withReply=false, const QPixmap &pixmap=QPixmap());
    void clearAllNotifications();
    bool hasUserNotificationCenterSupport(void);

private:
    MacNotification();

private:
    static MacNotification *aInstance;

signals:
    void notificationClicked(const int peer, const int mid);
    void notificationReplied(const int peer, const int mid, const QString &response);
};

#endif // MACNOTIFICATION
