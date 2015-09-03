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

#ifndef CHAT_H
#define CHAT_H

#include <QObject>
#include <QDateTime>
#include "vk/client.h"
#include "vk/dialogitem.h"
#include "vk/servicemsgitem.h"
#include "vk/sendmessagehandler.h"
#include "chatmodel.h"

class Chat : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int outAttachmentsCount READ outAttachmentsCount NOTIFY outAttachmentsChanged)
    Q_PROPERTY(QString textMessage READ textMessage WRITE setTextMessage NOTIFY textMessageChanged)

public:
    Chat();
    Chat(const DialogItem dialog);
    ~Chat();
    int id() const;
    bool isGroupChat() const;
    bool isCurrent() const;
    bool isBusy() const;
    DialogItem dialog() const;
    ChatModel* model() const;
    int countUnsent() const;
    QString textMessage() const;
    void setTextMessage(const QString &text);
    AttachmentList* outAttachments() const;
    int outAttachmentsCount() const;

private:
    DialogItem _dialog;
    ChatModel *_model;
    SendMessageHandler *_sendMessageHandler;
    static int _internalMessageId;
    int _countUnsent;
    QVector<MessageItem> _tempOutMessageQueue;
    QString _textMessage;
    AttachmentList *_outAttachments;

public slots:
    void addInMessage(const MessageItem message);
    void addOutMessage(const MessageItem message);
    void addTempOutMessageQueue(MessageItem message);//todo const?
    void addTyping(const TypingItem typing);
    void addTyping(const QList<TypingItem> &typingList);
    void addServiceMsg(const ServiceMsgItem serviceMsg);
    void sendMessage(const QString &text);
    void sendTyping();
    void addAttachments(const QList<QUrl> &list);
    void removeAttachment(const int index);
    void markAsRead();
    void refreshHistory();
    QString actionToString(const QString &author, const int act, const QString &arg, const int sex);

protected slots:
    void onMessageSending(const int internalMid);
    void onSuccessfullyMessageSent(const int internalMid, const int serverMid);
    void onUnsuccessfullyMessageSent(const int internalMid);
    void onModelRowsAllReplaced();

signals:
    void outAttachmentsChanged(AttachmentList *_outAttachments);
    void textMessageChanged(QString text);
};

#endif // CHAT_H
