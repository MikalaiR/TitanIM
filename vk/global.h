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
    Offline,
    Online
};

enum Sex
{
    Unknown,
    Woman,
    Man
};

const int GROUP_CHAT_OFFSET = 2000000000;

#endif // GLOBAL_H
