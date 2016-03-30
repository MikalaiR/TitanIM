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

#include "pushsettings.h"

PushSettings::PushSettings(Connection *connection)
{
    _connection = connection;
    connect(_connection, SIGNAL(authorized(int,QString,QString)), this, SLOT(onAuthorized(int,QString,QString)));
    connect(_connection, SIGNAL(logout(int)), this, SLOT(onLogout(int)));
}

void PushSettings::getSettings()
{
    Packet *packet = new Packet("account.getPushSettings");
    packet->addParam("device_id", "titanim_f883acd927af5ff1"); //todo
    connect(packet, SIGNAL(finished(const Packet*,QVariantMap)), this, SLOT(onGetSettings(const Packet*,QVariantMap)));
    _connection->appendQuery(packet);
}

void PushSettings::longPollSilenceModeUpdated(const int uid, const bool isMute, const uint disabledUntil)
{
    if (isMute)
    {
        if (!_mute.contains(uid)) _mute.insert(uid);
    }
    else
    {
        _mute.remove(uid);
    }

    emit muteUserChanged(uid, isMute);
}

bool PushSettings::isMuteUser(const int id) const
{
    return _mute.contains(id);
}

void PushSettings::setMuteUser(const int id, const bool isMute)
{
    Packet *packet = new Packet("account.setSilenceMode");
    packet->addParam("device_id", "titanim_f883acd927af5ff1"); //todo
    packet->addParam("user_id", id);
    packet->addParam("time", isMute ? -1 : 0);
    packet->addParam("sound", isMute ? 0 : 1);
    _connection->appendQuery(packet);
}

void PushSettings::onGetSettings(const Packet *sender, const QVariantMap &result)
{
    QVariantMap response = result.value("response").toMap();
    QVariantList conversations = response.value("conversations").toMap().value("items").toList();

    _mute.clear();

    for (int i = 0; i < conversations.count(); i++)
    {
        QVariantMap item = conversations[i].toMap();

        if (item.value("sound").toInt() == 0 || item.contains("disabled_until"))
        {
            _mute.insert(item.value("user_id").toInt());
        }
    }

    delete sender;
}

void PushSettings::onAuthorized(const int uid, const QString &token, const QString &secret)
{
    getSettings();
}

void PushSettings::onLogout(const int uid)
{
    _mute.clear();
}

