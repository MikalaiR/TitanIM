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

#ifndef AUTHORIZATION_H
#define AUTHORIZATION_H

#include <QObject>
#include "vk/client.h"
#include "settings.h"

class Authorization : public QObject
{
    Q_OBJECT

public:
    Authorization();

public slots:
    void connectToVk();
    void connectToVk(const QString &username, const QString &password);
    void setCaptcha(const QString &sid, const QString &text);
    void cancelCaptcha();

private slots:
    void onConnected(const int uid, const QString &token, const QString &secret);
    void onDisconnected();
    void onError(const ErrorResponse::Error &error, const QString &text, const bool global, const bool fatal);
    void onValidation(const QString &validationUri);

signals:
    void captcha(const QString &captchaSid, const QString &captchaImg);
    void showAuthPage();
    void showMainPage();
};

#endif // AUTHORIZATION_H
