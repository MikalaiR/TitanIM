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

#include "client.h"
#include "signup.h"

Client *Client::aInstance = 0;
QString Client::clientId = "1950109";
QString Client::clientSecret = "bJKfYSu0LS6N52M0HnBo";

Client *Client::instance()
{
    if (!aInstance) aInstance = new Client();
    return aInstance;
}

void Client::destroy()
{
    if (aInstance)
        delete aInstance, aInstance = 0;
}

Client::Client()
{
    _connection = new Connection(clientId, clientSecret);
    connect(_connection, SIGNAL(connected(int,QString,QString)), this, SLOT(onConnected(int,QString,QString)));
    connect(_connection, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    connect(_connection, SIGNAL(error(Error,QString,bool,bool)), this, SLOT(onError(Error,QString,bool,bool)));

    _longPoll = new LongPoll(_connection);

    _authSignup = 0;
    _uid = 0;
}

Client::~Client()
{
    delete _longPoll;
    delete _connection;
}

Connection *Client::connection() const
{
    return _connection;
}

LongPoll *Client::longPoll() const
{
    return _longPoll;
}

Signup *Client::authSignup() const
{
    if (!_authSignup)
    {
//        _authSignup = new Signup(clientId, clientSecret, this);
    }

    return _authSignup;
}

int Client::uid() const
{
    return _uid;
}

void Client::getServerTime()
{
    Packet *serverTime = new Packet("utils.getServerTime");
    connect(serverTime, SIGNAL(finished(const Packet*,QVariantMap)), this, SLOT(onServerTime(const Packet*,QVariantMap)));
    _connection->appendQuery(serverTime);
}

ProfileItem Client::profile() const
{
    return _profile;
}

void Client::getProfile()
{
    Packet *selfProfile = new Packet("users.get");
    selfProfile->addParam("uids", _uid);
    selfProfile->addParam("fields", "photo_100");

    connect(selfProfile, SIGNAL(finished(const Packet*,QVariantMap)), this, SLOT(onSelfProfile(const Packet*,QVariantMap)));
    _connection->appendQuery(selfProfile);
}

void Client::onConnected(const int uid, const QString &token, const QString &secret)
{
    _uid = uid;
    getServerTime(); //todo before connected
    getProfile();
    _longPoll->setRunning(true);
}

void Client::onDisconnected()
{
    _uid = 0;
    _profile.clear();

    _longPoll->setRunning(false);
}

void Client::onError(const Error &error, const QString &text, const bool global, const bool fatal)
{
}

void Client::onServerTime(const Packet *sender, const QVariantMap &result)
{
    uint unixtime = result.value("response").toUInt();
    QDateTime dateTime = QDateTime::fromTime_t(unixtime).toLocalTime();
    Utils::setServerDateTime(dateTime);

    delete sender;
}

void Client::onSelfProfile(const Packet *sender, const QVariantMap &result)
{
    QVariantList response = result.value("response").toList();
    _profile = ProfileParser::parser(response.at(0).toMap());

    delete sender;
}
