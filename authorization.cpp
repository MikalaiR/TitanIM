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
    connect(Client::instance()->connection(), SIGNAL(authorized(int,QString,QString)), this, SLOT(onAuthorized(int,QString,QString)));
    connect(Client::instance()->connection(), SIGNAL(logout(int)), this, SLOT(onLogout(int)));
    connect(Client::instance()->connection(), SIGNAL(error(ErrorResponse::Error,QString,bool,bool)), this, SLOT(onError(ErrorResponse::Error,QString,bool,bool)));
    connect(Client::instance()->connection(), SIGNAL(captcha(QString,QString)), this, SIGNAL(captcha(QString,QString)));
    connect(Client::instance()->connection(), SIGNAL(validation(QString)), this, SLOT(onValidation(QString)));
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
        QString token = Settings::instance()->loadProfile("main/token", "none").toString();
        QString secret = Settings::instance()->loadProfile("main/secret", "").toString();

        Client::instance()->connection()->connectToVk(uid.toInt(), token, secret);
    }
}

void Authorization::connectToVk(const QString &username, const QString &password)
{
    Client::instance()->connection()->connectToVk(username, password);
}

void Authorization::setCaptcha(const QString &sid, const QString &text)
{
    Client::instance()->connection()->setCaptcha(sid, text);
}

void Authorization::cancelCaptcha()
{
    Client::instance()->connection()->cancelCaptcha();
}

void Authorization::onAuthorized(const int uid, const QString &token, const QString &secret)
{
    qDebug() << "authorized" << " " << token << " " << secret;

    Settings::instance()->setCurrentUid(QString::number(uid));
    Settings::instance()->saveProfile("main/uid", uid);
    Settings::instance()->saveProfile("main/token", token);
    Settings::instance()->saveProfile("main/secret", secret);
    Settings::instance()->saveMain("profiles/last", uid);

    emit showMainPage();
}

void Authorization::onLogout(const int uid)
{
    qDebug() << "logout";
}

void Authorization::onError(const ErrorResponse::Error &error, const QString &text, const bool global, const bool fatal)
{
    if (error == ErrorResponse::UserAuthorizationFailed || error == ErrorResponse::LoadTokenFailed)
    {
        //todo remove token and secret
        emit showAuthPage();
    }
}

void Authorization::onValidation(const QString &validationUri)
{
    qDebug() << "VALIDATION:" << validationUri; //todo
}
