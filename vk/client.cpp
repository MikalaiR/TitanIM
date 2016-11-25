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

Client *Client::aInstance = nullptr;
QString Client::clientId = "1950109";
QString Client::clientSecret = "bJKfYSu0LS6N52M0HnBo";

Client *Client::instance()
{
    return aInstance ? aInstance : aInstance = new Client;
}

void Client::destroy()
{
    delete aInstance;
    aInstance = nullptr;
}

Client::Client()
{
    _connection = new Connection(clientId, clientSecret);
    connect(_connection, &Connection::authorized, this, &Client::onAuthorized);
    connect(_connection, &Connection::logout, this, &Client::onLogout);
    connect(_connection, &Connection::error, this, &Client::onError);
    connect(_connection, &Connection::networkOnlineChanged, this, &Client::onNetworkOnlineChanged);
    connect(_connection, &Connection::validation, this, &Client::onValidation);
    connect(_connection, &Connection::verification, this, &Client::onValidation);
    connect(_connection, &Connection::sessionChanged, this, &Client::onSessionChanged);

    _engine = new Engine(_connection);
    _longPoll = new LongPoll(_connection, _engine);
    _pushSettings = new PushSettings(_connection);

    connect(_longPoll, &LongPoll::userOnline, _engine, &Engine::setUserOnline);
    connect(_longPoll, &LongPoll::userOffline, _engine, &Engine::setUserOffline);
    connect(_longPoll, &LongPoll::obsoleteFriendsOnline, _engine, &Engine::getFriendsOnline);
    connect(_longPoll, &LongPoll::silenceModeUpdated, _pushSettings, &PushSettings::longPollSilenceModeUpdated);

    _authSignup = 0;

    _timerOnline = new QTimer(this);
    _timerOnline->setInterval(USER_OFFLINE_AWAY * 1000);
    connect(_timerOnline, &QTimer::timeout, this, &Client::onKeepOnlineTimerTimeout);
}

Client::~Client()
{
    delete _pushSettings;
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

PushSettings *Client::pushSettings() const
{
    return _pushSettings;
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

void Client::trackVisitor()
{
    Packet *visitor = new Packet("stats.trackVisitor");
    _connection->appendQuery(visitor);
}

void Client::setOnline()
{
    Packet *account = new Packet("account.setOnline");
    account->setPerishable(true);
    _connection->appendQuery(account);
}

void Client::setOffline()
{
    Packet *account = new Packet("account.setOffline");
    account->setPerishable(true);
    _connection->appendQuery(account);
}

void Client::keepOnline(const bool on)
{
    if (on)
    {
        setOnline();
        _timerOnline->start();
    }
    else
    {
        _timerOnline->stop();
    }
}

void Client::onAuthorized(const int uid, const QString &token, const QString &secret)
{
    trackVisitor();
    _longPoll->start();
}

void Client::onLogout(const int uid)
{
    _longPoll->stop();
    keepOnline(false);
}

void Client::onError(const ErrorResponse::Error &error, const QString &text, const bool global, const bool fatal)
{
}

void Client::onNetworkOnlineChanged(const bool isOnline)
{
    if (isOnline)
    {
        _longPoll->resume();

        if (_timerOnline->isActive())
        {
            setOnline();
        }
    }
    else
    {
        _longPoll->stop();
    }
}

void Client::onValidation()
{
    _longPoll->stop();
}

void Client::onSessionChanged(const int uid, const QString &token, const QString &secret)
{
    _longPoll->resume();

    if (_timerOnline->isActive())
    {
        setOnline();
    }
}

void Client::onKeepOnlineTimerTimeout()
{
    if (_longPoll->isRunning())
    {
        setOnline();
    }
}
