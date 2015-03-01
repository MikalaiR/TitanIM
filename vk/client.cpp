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
    connect(_connection, SIGNAL(error(ErrorResponse::Error,QString,bool,bool)), this, SLOT(onError(ErrorResponse::Error,QString,bool,bool)));

    _engine = new Engine(_connection);
    _longPoll = new LongPoll(_connection, _engine);

    _authSignup = 0;
}

Client::~Client()
{
    delete _longPoll;
    delete _engine;
    delete _connection;
}

Connection *Client::connection() const
{
    return _connection;
}

Engine *Client::engine() const
{
    return _engine;
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
    return _engine->uid();
}

ProfileItem Client::profile() const
{
    return _engine->getProfile();
}

void Client::getServerTime()
{
    Packet *serverTime = new Packet("utils.getServerTime");
    connect(serverTime, SIGNAL(finished(const Packet*,QVariantMap)), this, SLOT(onServerTime(const Packet*,QVariantMap)));
    _connection->appendQuery(serverTime);
}

void Client::trackVisitor()
{
    Packet *visitor = new Packet("stats.trackVisitor");
    _connection->appendQuery(visitor);
}

void Client::onConnected(const int uid, const QString &token, const QString &secret)
{
    getServerTime(); //todo before connected
    trackVisitor();
    _longPoll->start();
}

void Client::onDisconnected()
{
    _longPoll->stop();
}

void Client::onError(const ErrorResponse::Error &error, const QString &text, const bool global, const bool fatal)
{
}

void Client::onServerTime(const Packet *sender, const QVariantMap &result)
{
    uint unixtime = result.value("response").toUInt();
    QDateTime dateTime = QDateTime::fromTime_t(unixtime).toLocalTime();
    Utils::setServerDateTime(dateTime);

    delete sender;
}
