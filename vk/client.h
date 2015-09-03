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

#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include "connection.h"
#include "longpoll.h"
#include "signup.h"
#include "engine.h"

class Client : public QObject
{
    Q_OBJECT

public:
    static Client *instance();
    static void destroy();
    Connection* connection() const;
    Engine* engine() const;
    LongPoll* longPoll() const;
    Signup* authSignup() const;
    int uid() const;
    ProfileItem profile() const;

private:
    Client();
    ~Client();

private:
    static Client *aInstance;
    static QString clientId;
    static QString clientSecret;
    Connection *_connection;
    Engine *_engine;
    LongPoll *_longPoll;
    Signup *_authSignup;

public slots:
    void trackVisitor();

private slots:
    void onAuthorized(const int uid, const QString &token, const QString &secret);
    void onLogout(const int uid);
    void onError(const ErrorResponse::Error &error, const QString &text, const bool global, const bool fatal);
    void onNetworkOnlineChanged(const bool isOnline);
};

#endif // CLIENT_H
