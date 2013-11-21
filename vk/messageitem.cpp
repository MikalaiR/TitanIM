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

#include "messageitem.h"

MessageItemPrivate::MessageItemPrivate()
{
    _chatId = -1;
}

int MessageItemPrivate::mid() const
{
    return _mid;
}

void MessageItemPrivate::setMid(const int mid)
{
    if (_mid != mid)
    {
        _mid = mid;
    }
}

int MessageItemPrivate::uid() const
{
    return _uid;
}

void MessageItemPrivate::setUid(const int uid)
{
    if (_uid != uid)
    {
        _uid = uid;
    }
}

QDateTime MessageItemPrivate::date() const
{
    return _date;
}

void MessageItemPrivate::setDate(const QDateTime &date)
{
    if (_date != date)
    {
        _date = date;
    }
}

bool MessageItemPrivate::isUnread() const
{
    return _isUnread;
}

void MessageItemPrivate::setIsUnread(const bool isUnread)
{
    if (_isUnread != isUnread)
    {
        _isUnread = isUnread;
    }
}

bool MessageItemPrivate::isOut() const
{
    return _isOut;
}

void MessageItemPrivate::setIsOut(const bool isOut)
{
    if (_isOut != isOut)
    {
        _isOut = isOut;
    }
}

bool MessageItemPrivate::isError() const
{
    return _isError;
}

void MessageItemPrivate::setIsError(const bool isError)
{
    if (_isError != isError)
    {
        _isError = isError;
    }
}

bool MessageItemPrivate::isSending() const
{
    return _mid < 0;
}

bool MessageItemPrivate::deliveryReport() const
{
    return _deliveryReport;
}

void MessageItemPrivate::setDeliveryReport(const bool deliveryReport)
{
    if (_deliveryReport != deliveryReport)
    {
        _deliveryReport = deliveryReport;
    }
}

QString MessageItemPrivate::title() const
{
    return _title;
}

void MessageItemPrivate::setTitle(const QString &title)
{
    if (_title != title)
    {
        _title = title;
    }
}

QString MessageItemPrivate::body() const
{
    return _body;
}

void MessageItemPrivate::setBody(const QString &body)
{
    if (_body != body)
    {
        _body = body;
    }
}

int MessageItemPrivate::chatId() const
{
    return _chatId;
}

void MessageItemPrivate::setChatId(const int chatId)
{
    if (_chatId != chatId)
    {
        _chatId = chatId;
    }
}

bool MessageItemPrivate::isGroupChat() const
{
    return _chatId == -1 ? false : true;
}

QString MessageItemPrivate::firstName() const
{
    return _firstName;
}

void MessageItemPrivate::setFirstName(const QString &firstName)
{
    if (_firstName != firstName)
    {
        _firstName = firstName;
    }
}

QString MessageItemPrivate::lastName() const
{
    return _lastName;
}

void MessageItemPrivate::setLastName(const QString &lastName)
{
    if (_lastName != lastName)
    {
        _lastName = lastName;
    }
}

QString MessageItemPrivate::displayName() const
{
    return _displayName;
}

void MessageItemPrivate::setDisplayName(const QString &displayName)
{
    if (_displayName != displayName)
    {
        _displayName = displayName;
    }
}

Sex MessageItemPrivate::sex() const
{
    return _sex;
}

void MessageItemPrivate::setSex(const Sex sex)
{
    if (_sex != sex)
    {
        _sex = sex;
    }
}

QString MessageItemPrivate::photoMediumRect() const
{
    return _photoMediumRec;
}

void MessageItemPrivate::setPhotoMediumRect(const QString &photoMediumRec)
{
    if (_photoMediumRec != photoMediumRec)
    {
        _photoMediumRec = photoMediumRec;
    }
}

QString MessageItemPrivate::photoMediumRect2() const
{
    return _photoMediumRec2;
}

void MessageItemPrivate::setPhotoMediumRect2(const QString &photoMediumRec2)
{
    if (_photoMediumRec2 != photoMediumRec2)
    {
        _photoMediumRec2 = photoMediumRec2;
    }
}

QString MessageItemPrivate::photoMediumRect3() const
{
    return _photoMediumRec3;
}

void MessageItemPrivate::setPhotoMediumRect3(const QString &photoMediumRec3)
{
    if (_photoMediumRec3 != photoMediumRec3)
    {
        _photoMediumRec3 = photoMediumRec3;
    }
}

QString MessageItemPrivate::photoMediumRect4() const
{
    return _photoMediumRec4;
}

void MessageItemPrivate::setPhotoMediumRect4(const QString &photoMediumRec4)
{
    if (_photoMediumRec4 != photoMediumRec4)
    {
        _photoMediumRec4 = photoMediumRec4;
    }
}

bool MessageItemPrivate::online() const
{
    return _online;
}

void MessageItemPrivate::setOnline(const bool online)
{
    if (_online != online)
    {
        _online = online;
    }
}

int MessageItemPrivate::lastSeen() const
{
    return _lastSeen;
}

void MessageItemPrivate::setLastSeen(const int lastSeen)
{
    if (_lastSeen != lastSeen)
    {
        _lastSeen = lastSeen;
    }
}
