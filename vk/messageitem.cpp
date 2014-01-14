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
    _id = 0;
    _uid = -1;
    _isError = false;
    _chatId = -1;
}

MessageItemPrivate::~MessageItemPrivate()
{
}

int MessageItemPrivate::id() const
{
    return _id;
}

void MessageItemPrivate::setId(const int id)
{
    if (_id != id)
    {
        _id = id;
        emit propertyChanged(_id, "id");
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
        emit propertyChanged(_id, "uid");
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
        emit propertyChanged(_id, "date");
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
        emit propertyChanged(_id, "isUnread");
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
        emit propertyChanged(_id, "isOut");
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
        emit propertyChanged(_id, "isError");
    }
}

bool MessageItemPrivate::isSending() const
{
    return _id < 0;
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
        emit propertyChanged(_id, "deliveryReport");
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
        emit propertyChanged(_id, "title");
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
        emit propertyChanged(_id, "body");
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
        emit propertyChanged(_id, "chatId");
    }
}

bool MessageItemPrivate::isGroupChat() const
{
    return _chatId == -1 ? false : true;
}
