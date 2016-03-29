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
    connect(Client::instance()->connection(), SIGNAL(sessionChanged(int,QString,QString)), this, SLOT(saveSession(int,QString,QString)));

    connect(Client::instance()->connection(), SIGNAL(authorized(int,QString,QString)), this, SIGNAL(authorized(int,QString,QString)));
    connect(Client::instance()->connection(), SIGNAL(logout(int)), this, SIGNAL(logout(int)));
    connect(Client::instance()->connection(), SIGNAL(captcha(QString,QString)), this, SIGNAL(captcha(QString,QString)));
    connect(Client::instance()->connection(), SIGNAL(validation(QString)), this, SIGNAL(validation(QString)));
    connect(Client::instance()->connection(), SIGNAL(verification(QString,QString,bool,QString)), this, SIGNAL(verification(QString,QString,bool,QString)));
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

void Authorization::disconnectVk()
{
    Client::instance()->connection()->disconnectVk();
}

void Authorization::setCaptcha(const QString &sid, const QString &text)
{
    Client::instance()->connection()->setCaptcha(sid, text);
}

void Authorization::cancelCaptcha()
{
    Client::instance()->connection()->cancelCaptcha();
}

void Authorization::setVerificationCode(const QString &sid, const QString &code)
{
    Client::instance()->connection()->setVerificationCode(sid, code);
}

void Authorization::checkUrlValidation(const QUrl &url)
{
    if (url.fileName() == "blank.html")
    {
        QString fragment = url.fragment();

        if (fragment.contains("success=1"))
        {
            SessionVars session;
            QUrlQuery urlQuery = QUrlQuery(fragment);

            session.access_token = urlQuery.queryItemValue("access_token");
            session.user_id = urlQuery.queryItemValue("user_id").toInt();
            session.expires_in = 0;
            session.secret = fragment.contains("secret") ? urlQuery.queryItemValue("secret") : "";

            Client::instance()->connection()->setSession(session);
            emit showMainPage();
        }
        else if (fragment.contains("cancel=1"))
        {
            SessionVars session = Client::instance()->connection()->session();
            Client::instance()->connection()->setSession(session);
            emit showMainPage();
        }
        else
        {
            disconnectVk();
        }
    }
}

void Authorization::showValidation(const QString &validationUri)
{
    emit validation(validationUri);
}

void Authorization::keepOnline(const bool on)
{
    Client::instance()->keepOnline(on);

    if (!on)
    {
        Client::instance()->setOffline();
    }
}

void Authorization::onAuthorized(const int uid, const QString &token, const QString &secret)
{
    saveSession(uid, token, secret);
    emit showMainPage();

    bool keepOnline = Settings::instance()->loadProfile("main/keepOnline", true).toBool();
    Client::instance()->keepOnline(keepOnline);
}

void Authorization::saveSession(const int uid, const QString &token, const QString &secret)
{
    Settings::instance()->setCurrentUid(QString::number(uid));
    Settings::instance()->saveProfile("main/uid", uid);
    Settings::instance()->saveProfile("main/token", token);
    Settings::instance()->saveProfile("main/secret", secret);
    Settings::instance()->saveMain("profiles/last", uid);
}

void Authorization::onLogout(const int uid)
{
    Settings::instance()->saveProfile("main/token", "", QString::number(uid));
    Settings::instance()->saveProfile("main/secret", "", QString::number(uid));
    Settings::instance()->saveMain("profiles/last", "");

    emit showAuthPage();
}

void Authorization::onError(const ErrorResponse::Error &error, const QString &text, const bool global, const bool fatal)
{
    switch (error) {
    case ErrorResponse::UserAuthorizationFailed:
    case ErrorResponse::LoadTokenFailed:
    {
        //todo remove token and secret
        emit showAuthPage();
        break;
    }

    case ErrorResponse::UnknownErrorOccured:
    {
        emit incorrectCode();
        break;
    }

    default:
    {
        break;
    }
    }
}
