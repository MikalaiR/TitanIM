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

#ifndef MESSAGEITEM_H
#define MESSAGEITEM_H

#include <QObject>
#include <QSharedPointer>
#include <QDateTime>

class MessageItemPrivate : public QObject
{
    Q_OBJECT

public:
    MessageItemPrivate();
    ~MessageItemPrivate();
    int mid() const;
    void setMid(const int mid);
    int uid() const;
    void setUid(const int uid);
    QDateTime date() const;
    void setDate(const QDateTime &date);
    bool isUnread() const;
    void setIsUnread(const bool isUnread);
    bool isOut() const;
    void setIsOut(const bool isOut);
    bool isError() const;
    void setIsError(const bool isError);
    bool isSending() const;
    bool deliveryReport() const;
    void setDeliveryReport(const bool deliveryReport);
    QString title() const;
    void setTitle(const QString &title);
    QString body() const;
    void setBody(const QString &body);
    int chatId() const;
    void setChatId(const int chatId);
    bool isGroupChat() const;

private:
    int _mid;
    int _uid;
    QDateTime _date;
    bool _isUnread;
    bool _isOut;
    bool _isError;
    bool _deliveryReport;
    QString _title;
    QString _body;
    int _chatId;
//    AttachmentList _attachments;

signals:
    void propertyChanged(const int mid, const QString &propertyName);
};

typedef QSharedPointer<MessageItemPrivate> MessageItem;

#endif // MESSAGEITEM_H
