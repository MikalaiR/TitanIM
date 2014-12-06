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
#include "messagebase.h"
#include "attachmentlist.h"
#include "connection.h"
#include "emoticons.h"

class MessageItemPrivate : public MessageBase
{
    Q_OBJECT

public:
    MessageItemPrivate();
    ~MessageItemPrivate();
    int uid() const;
    void setUid(const int uid);
    int fromId() const;
    void setFromId(const int fromId);
    uint unixtime() const;
    bool isUnread() const;
    void setIsUnread(const bool isUnread);
    bool isOut() const;
    void setIsOut(const bool isOut);
    bool isError() const;
    void setIsError(const bool isError);
    bool isLoading() const;
    bool isSending() const;
    bool deliveryReport() const;
    void setDeliveryReport(const bool deliveryReport);
    QString title() const;
    void setTitle(const QString &title);
    QString body() const;
    QString plainBody() const;
    QString shortBody() const;
    void setBody(const QString &body, const bool emoji=false, const bool escaped=true);
    int chatId() const;
    void setChatId(const int chatId);
    bool isGroupChat() const;
    bool emoji() const;
    AttachmentList* attachments() const;
    void setAttachments(AttachmentList *attachments);

private:
    int _uid;
    bool _isUnread;
    bool _isOut;
    bool _isError;
    bool _isLoading;
    bool _deliveryReport;
    QString _title;
    QString _body;
    int _chatId;
    bool _emoji;
    AttachmentList *_attachments;

public slots:
    void getAllFields(Connection *connection);

protected slots:
    void setIsLoading(const bool isLoading);
    void loadFinished(const Packet *sender, const QVariantMap &result);
};

typedef QSharedPointer<MessageItemPrivate> MessageItem;

#endif // MESSAGEITEM_H
