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
#include <QDebug>
#include "connection.h"
#include "longpoll.h"
#include "signup.h"
#include "global.h"

class Client : public QObject
{
    Q_OBJECT

public:
    static Client *instance();
    static void destroy();
    Connection* connection() const;
    LongPoll* longPoll() const;
    Signup* authSignup() const;
    int uid() const;
    QString fullName() const;
    QString photoMediumRec() const;
    void getProfileSelf();

private:
    Client();
    ~Client();

private:
    static Client *aInstance;
    static QString clientId;
    static QString clientSecret;
    Connection *_connection;
    LongPoll *_longPoll;
    Signup *_authSignup;
    int _uid;
    QString _fullName;
    QString _photoMediumRec;

private slots:
    void onConnected(const int uid, const QString &token, const QString &secret);
    void onDisconnected();
    void onError(const Error &error, const QString &text, const bool global, const bool fatal);
    void onSelfProfile(const Packet *sender, const QVariantMap &result);
};

#endif // CLIENT_H
