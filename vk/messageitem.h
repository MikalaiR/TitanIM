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
#include <QDateTime>
#include "global.h"

class MessageItem : public QObject
{
    Q_OBJECT

public:
    MessageItem();
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
    QString firstName() const;
    void setFirstName(const QString &firstName);
    QString lastName() const;
    void setLastName(const QString &lastName);
    QString displayName() const;
    void setDisplayName(const QString &displayName);
    Sex sex() const;
    void setSex(const Sex sex);
    QString photoMediumRec() const;
    void setPhotoMediumRec(const QString &photoMediumRec);
    QString photoMediumRec2() const;
    void setPhotoMediumRec2(const QString &photoMediumRec2);
    QString photoMediumRec3() const;
    void setPhotoMediumRec3(const QString &photoMediumRec3);
    QString photoMediumRec4() const;
    void setPhotoMediumRec4(const QString &photoMediumRec4);
    bool online() const;
    void setOnline(const bool online);
    int lastSeen() const;
    void setLastSeen(const int lastSeen);

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
    QString _chatActive;
    int _chatUsersCount;
    QString _firstName;
    QString _lastName;
    QString _displayName;
    Sex _sex;
    QString _photoMediumRec;
    QString _photoMediumRec2;
    QString _photoMediumRec3;
    QString _photoMediumRec4;
    bool _online;
    int _lastSeen;
//    AttachmentList _attachments;
};

#endif // MESSAGEITEM_H
