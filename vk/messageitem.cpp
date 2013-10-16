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

MessageItem::MessageItem()
{
    _chatId = -1;
}

int MessageItem::mid() const
{
    return _mid;
}

void MessageItem::setMid(const int mid)
{
    if (_mid != mid)
    {
        _mid = mid;
    }
}

int MessageItem::uid() const
{
    return _uid;
}

void MessageItem::setUid(const int uid)
{
    if (_uid != uid)
    {
        _uid = uid;
    }
}

QDateTime MessageItem::date() const
{
    return _date;
}

void MessageItem::setDate(const QDateTime &date)
{
    if (_date != date)
    {
        _date = date;
    }
}

bool MessageItem::isUnread() const
{
    return _isUnread;
}

void MessageItem::setIsUnread(const bool isUnread)
{
    if (_isUnread != isUnread)
    {
        _isUnread = isUnread;
    }
}

bool MessageItem::isOut() const
{
    return _isOut;
}

void MessageItem::setIsOut(const bool isOut)
{
    if (_isOut != isOut)
    {
        _isOut = isOut;
    }
}

bool MessageItem::isError() const
{
    return _isError;
}

void MessageItem::setIsError(const bool isError)
{
    if (_isError != isError)
    {
        _isError = isError;
    }
}

bool MessageItem::isSending() const
{
    return _mid < 0;
}

bool MessageItem::deliveryReport() const
{
    return _deliveryReport;
}

void MessageItem::setDeliveryReport(const bool deliveryReport)
{
    if (_deliveryReport != deliveryReport)
    {
        _deliveryReport = deliveryReport;
    }
}

QString MessageItem::title() const
{
    return _title;
}

void MessageItem::setTitle(const QString &title)
{
    if (_title != title)
    {
        _title = title;
    }
}

QString MessageItem::body() const
{
    return _body;
}

void MessageItem::setBody(const QString &body)
{
    if (_body != body)
    {
        _body = body;
    }
}

int MessageItem::chatId() const
{
    return _chatId;
}

void MessageItem::setChatId(const int chatId)
{
    if (_chatId != chatId)
    {
        _chatId = chatId;
    }
}

bool MessageItem::isGroupChat() const
{
    return _chatId == -1 ? false : true;
}

QString MessageItem::firstName() const
{
    return _firstName;
}

void MessageItem::setFirstName(const QString &firstName)
{
    if (_firstName != firstName)
    {
        _firstName = firstName;
    }
}

QString MessageItem::lastName() const
{
    return _lastName;
}

void MessageItem::setLastName(const QString &lastName)
{
    if (_lastName != lastName)
    {
        _lastName = lastName;
    }
}

QString MessageItem::displayName() const
{
    return _displayName;
}

void MessageItem::setDisplayName(const QString &displayName)
{
    if (_displayName != displayName)
    {
        _displayName = displayName;
    }
}

Sex MessageItem::sex() const
{
    return _sex;
}

void MessageItem::setSex(const Sex sex)
{
    if (_sex != sex)
    {
        _sex = sex;
    }
}

QString MessageItem::photoMediumRec() const
{
    return _photoMediumRec;
}

void MessageItem::setPhotoMediumRec(const QString &photoMediumRec)
{
    if (_photoMediumRec != photoMediumRec)
    {
        _photoMediumRec = photoMediumRec;
    }
}

QString MessageItem::photoMediumRec2() const
{
    return _photoMediumRec2;
}

void MessageItem::setPhotoMediumRec2(const QString &photoMediumRec2)
{
    if (_photoMediumRec2 != photoMediumRec2)
    {
        _photoMediumRec2 = photoMediumRec2;
    }
}

QString MessageItem::photoMediumRec3() const
{
    return _photoMediumRec3;
}

void MessageItem::setPhotoMediumRec3(const QString &photoMediumRec3)
{
    if (_photoMediumRec3 != photoMediumRec3)
    {
        _photoMediumRec3 = photoMediumRec3;
    }
}

QString MessageItem::photoMediumRec4() const
{
    return _photoMediumRec4;
}

void MessageItem::setPhotoMediumRec4(const QString &photoMediumRec4)
{
    if (_photoMediumRec4 != photoMediumRec4)
    {
        _photoMediumRec4 = photoMediumRec4;
    }
}

bool MessageItem::online() const
{
    return _online;
}

void MessageItem::setOnline(const bool online)
{
    if (_online != online)
    {
        _online = online;
    }
}

int MessageItem::lastSeen() const
{
    return _lastSeen;
}

void MessageItem::setLastSeen(const int lastSeen)
{
    if (_lastSeen != lastSeen)
    {
        _lastSeen = lastSeen;
    }
}
