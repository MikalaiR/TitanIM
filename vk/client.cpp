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
        delete aInstance, aInstance = nullptr;
}

Client::Client()
{
    _connection = new Connection(clientId, clientSecret);
    connect(_connection, SIGNAL(authorized(int,QString,QString)), this, SLOT(onAuthorized(int,QString,QString)));
    connect(_connection, SIGNAL(logout(int)), this, SLOT(onLogout(int)));
    connect(_connection, SIGNAL(error(ErrorResponse::Error,QString,bool,bool)), this, SLOT(onError(ErrorResponse::Error,QString,bool,bool)));
    connect(_connection, SIGNAL(networkOnlineChanged(bool)), this, SLOT(onNetworkOnlineChanged(bool)));
    connect(_connection, SIGNAL(validation(QString)), this, SLOT(onValidation()));
    connect(_connection, SIGNAL(verification(QString,QString,bool,QString)), this, SLOT(onValidation()));
    connect(_connection, SIGNAL(sessionChanged(int,QString,QString)), this, SLOT(onSessionChanged(int,QString,QString)));

    _engine = new Engine(_connection);
    _longPoll = new LongPoll(_connection, _engine);
    _pushSettings = new PushSettings(_connection);

    connect(_longPoll, SIGNAL(userOnline(int)), _engine, SLOT(setUserOnline(int)));
    connect(_longPoll, SIGNAL(userOffline(int,bool)), _engine, SLOT(setUserOffline(int,bool)));
    connect(_longPoll, SIGNAL(obsoleteFriendsOnline()), _engine, SLOT(getFriendsOnline()));
    connect(_longPoll, SIGNAL(silenceModeUpdated(int,bool,uint)), _pushSettings, SLOT(longPollSilenceModeUpdated(int,bool,uint)));

    _authSignup = 0;

    _timerOnline = new QTimer(this);
    _timerOnline->setInterval(USER_OFFLINE_AWAY * 1000);
    connect(_timerOnline, SIGNAL(timeout()), this, SLOT(onKeepOnlineTimerTimeout()));
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
