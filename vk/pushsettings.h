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

#ifndef PUSHSETTINGS_H
#define PUSHSETTINGS_H

#include <QObject>
#include "connection.h"

class PushSettings : public QObject
{
    Q_OBJECT

public:
    PushSettings(Connection *connection);
    void getSettings();

private:
    Connection *_connection;
    QSet<int> _mute;

public slots:
    void longPollSilenceModeUpdated(const int uid, const bool isMute, const uint disabledUntil);
    bool isMuteUser(const int id) const;
    void setMuteUser(const int id, const bool isMute);

protected slots:
    void onGetSettings(const Packet *sender, const QVariantMap &result);

private slots:
    void onAuthorized(const int uid, const QString &token, const QString &secret);
    void onLogout(const int uid);

signals:
    void muteUserChanged(const int id, const bool isMute);
};

#endif // PUSHSETTINGS_H
