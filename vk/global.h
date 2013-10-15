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

#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>

struct LoginVars
{
    QString grant_type;
    QString client_id;
    QString client_secret;
    QString username;
    QString password;
    QString scope;
    QString captcha_sid;
    QString captcha_key;
};

struct SessionVars
{
    QString access_token;
    QString secret;
    QString expires_in;
    int user_id;
};

struct UrlServers
{
    QString auth_server;
    QString api_server;
};

struct LongPollVars
{
    QString server;
    QString key;
    QString ts;
    int wait;
};

enum Status
{
    offline,
    online
};

enum Sex
{
    unknown,
    woman,
    man
};

enum Error
{
    loadTokenFailed = -3,
    timeoutLongPollServer = -2,
    serverIsNotAvailable = -1,
    unknownErrorOccured = 1,
    applicationIsDisabled = 2,
    unknownMethodPassed = 3,
    incorrectSignature = 4,
    userAuthorizationFailed = 5,
    tooManyRequestsPerSecond = 6,
    deniedByUser = 7,
    internalServerError = 10,
    captchaNeeded = 14,
    accessDenied = 15,
    httpAuthorizationFailed = 16,
    validationRequired = 17,
    outOfLimits = 103,
    phoneUsedAnotherUser = 1004,
    tryLater = 1112
};

#endif // GLOBAL_H
