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

#include "authorization.h"

Authorization::Authorization()
{
    connect(Client::instance()->connection(), SIGNAL(connected(int,QString,QString)), this, SLOT(onConnected(int,QString,QString)));
    connect(Client::instance()->connection(), SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    connect(Client::instance()->connection(), SIGNAL(error(ErrorResponse::Error,QString,bool,bool)), this, SLOT(onError(ErrorResponse::Error,QString,bool,bool)));
}

void Authorization::connectToVk()
{
    if (Settings::instance()->lastUid().isEmpty())
    {
        emit showAuthPage();
    }
    else
    {
        emit showMainPage();

        QString uid = Settings::instance()->lastUid();
        Settings::instance()->setCurrentUid(uid);
        QString token = Settings::instance()->loadProfile("main/token", "").toString();
        QString secret = Settings::instance()->loadProfile("main/secret", "").toString();

        Client::instance()->connection()->connectToVk(uid.toInt(), token, secret);
    }
}

void Authorization::connectToVk(const QString &username, const QString &password)
{
    Client::instance()->connection()->connectToVk(username, password);
}

void Authorization::onConnected(const int uid, const QString &token, const QString &secret)
{
    qDebug() << "connecting" << " " << token << " " << secret;

    Settings::instance()->setCurrentUid(QString::number(uid));
    Settings::instance()->saveProfile("main/uid", uid);
    Settings::instance()->saveProfile("main/token", token);
    Settings::instance()->saveProfile("main/secret", secret);
    Settings::instance()->saveMain("profiles/last", uid);

    emit showMainPage();
}

void Authorization::onDisconnected()
{
    qDebug() << "disconnecting";
}

void Authorization::onError(const ErrorResponse::Error &error, const QString &text, const bool global, const bool fatal)
{
    if (error == ErrorResponse::UserAuthorizationFailed || error == ErrorResponse::LoadTokenFailed)
    {
        //todo remove token and secret
        emit showAuthPage();
    }
}
