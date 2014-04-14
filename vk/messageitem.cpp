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
    _uid = 0;
    _isError = false;
    _chatId = 0;
    _attachments = 0;
}

MessageItemPrivate::~MessageItemPrivate()
{
    if (_attachments) delete _attachments;
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
        emitPropertyChanged("uid");
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
        emitPropertyChanged("date");
    }
}

uint MessageItemPrivate::unixtime() const
{
    return _date.toTime_t();
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
        emitPropertyChanged("isUnread");
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
        emitPropertyChanged("isOut");
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
        emitPropertyChanged("isError");
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
        emitPropertyChanged("deliveryReport");
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
        emitPropertyChanged("title");
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
        emitPropertyChanged("body");
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
        emitPropertyChanged("chatId");
    }
}

bool MessageItemPrivate::isGroupChat() const
{
    return _chatId == 0 ? false : true;
}

AttachmentList* MessageItemPrivate::attachments() const
{
    return _attachments;
}

void MessageItemPrivate::setAttachments(AttachmentList *attachments)
{
    if (_attachments != attachments)
    {
        if (_attachments) delete _attachments;

        _attachments = attachments;
        emitPropertyChanged("attachments");
    }
}
