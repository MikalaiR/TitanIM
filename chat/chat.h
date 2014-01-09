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
#include "vk/sendmessagehandler.h"
#include "chatmodel.h"

class Chat : public QObject
{
    Q_OBJECT

public:
    Chat();
    Chat(const DialogItem dialog);
    ~Chat();
    int id() const;
    bool isGroupChat() const;
    DialogItem dialog() const;
    ChatModel* model() const;
    int countUnsent() const;
    void addTempMessageQueue(MessageItem message);

private:
    DialogItem _dialog;
    ChatModel *_model;
    SendMessageHandler *_sendMessageHandler;
    static int _internalMessageId;
    int _countUnsent;
    QVector<MessageItem> _tempMessageQueue;

public slots:
    void sendMessage(const QString &text);

protected slots:
    void onMessageSending(const int internalMid);
    void onSuccessfullyMessageSent(const int internalMid, const int serverMid);
};

#endif // CHAT_H
