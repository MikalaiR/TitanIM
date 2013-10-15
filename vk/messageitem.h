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
    int mid();
    void setMid(const int mid);
    int uid();
    void setUid(const int uid);
    QDateTime date();
    void setDate(const QDateTime &date);
    bool isUnread();
    void setIsUnread(const bool isUnread);
    bool isOut();
    void setIsOut(const bool isOut);
    bool isError();
    void setIsError(const bool isError);
    bool isSending();
    bool deliveryReport();
    void setDeliveryReport(const bool deliveryReport);
    QString title();
    void setTitle(const QString &title);
    QString body();
    void setBody(const QString &body);
    int chatId();
    void setChatId(const int chatId);
    QString chatActive();
    void setChatActive(const QString &chatActive);
    bool isGroupChat();
    int chatUsersCount();
    void setChatUsersCount(const int chatUsersCount);
    QString firstName();
    void setFirstName(const QString &firstName);
    QString lastName();
    void setLastName(const QString &lastName);
    QString displayName();
    void setDisplayName(const QString &displayName);
    Sex sex();
    void setSex(const Sex sex);
    QString photoMediumRec();
    void setPhotoMediumRec(const QString &photoMediumRec);
    QString photoMediumRec2();
    void setPhotoMediumRec2(const QString &photoMediumRec2);
    QString photoMediumRec3();
    void setPhotoMediumRec3(const QString &photoMediumRec3);
    QString photoMediumRec4();
    void setPhotoMediumRec4(const QString &photoMediumRec4);
    bool online();
    void setOnline(const bool online);
    int lastSeen();
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
