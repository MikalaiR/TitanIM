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

#ifndef LONGPOLL_H
#define LONGPOLL_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "connection.h"
#include "engine.h"
#include "global.h"
#include "messageitem.h"
#include "profileitem.h"

class LongPoll : public QObject
{
    Q_OBJECT

public:
    enum UpdateType
    {
        MessageDeleted = 0,
        MessageFlagsReplaced = 1,
        MessageFlagsSet = 2,
        MessageFlagsReseted = 3,
        MessageAdded = 4,
        InMessagesRead = 6,
        OutMessagesRead = 7,
        UserOnline = 8,
        UserOffline = 9,
        GroupChatUpdated = 51,
        ChatTyping = 61,
        GroupChatTyping = 62,
        UserCall = 70,
        UnreadDialogs = 80
    };

    enum MessageFlags
    {
        Unread = 1,
        Outbox = 2,
        Replied = 4,
        Important = 8,
        Chat = 16,
        Friends = 32,
        Spam = 64,
        Deleted = 128,
        Fixed = 256,
        Media = 512
    };

    LongPoll(Connection *connection, Engine *engine);
    int wait() const;
    void setWait(const int sec);
    int maxMsgId() const;
    void setMaxMsgId(const int mid);
    bool isRunning() const;
    void start();
    void stop();
    void resume();

private:
    void setRunning(const bool running);
    void getLongPollServer();
    void longPoll();
    void getLongPollHistory();
    void handler(const QVariantList &updates);
    void messageAdd(const MessageItem message, const ProfileItem profile);

private:
    LongPollVars _longPollVars;
    Connection *_connection;
    Engine *_engine;
    QNetworkAccessManager *httpLongPoll;
    bool _running;

protected:
    void onMessageDeleted(const QVariantList &update);
    void onMessageFlagsReplaced(const QVariantList &update);
    void onMessageFlagsSet(const QVariantList &update);
    void onMessageFlagsReseted(const QVariantList &update);
    void onMessageAdded(const QVariantList &update);
    void onInMessagesRead(const QVariantList &update);
    void onOutMessagesRead(const QVariantList &update);
    void onUserOnline(const QVariantList &update);
    void onUserOffline(const QVariantList &update);
    void onGroupChatUpdated(const QVariantList &update);
    void onChatTyping(const QVariantList &update);
    void onGroupChatTyping(const QVariantList &update);
    void onUserCall(const QVariantList &update);
    void onUnreadDialogs(const QVariantList &update);

private slots:
    void onRunningChanged(const bool running);
    void getLongPollServerFinished(const Packet *sender, const QVariantMap &result);
    void longPollResponse(QNetworkReply *networkReply);
    void getLongPollHistoryFinished(const Packet *sender, const QVariantMap &result);
    void getLongPollHistoryError(const Packet *sender, const ErrorResponse *errorResponse);

signals:
    void resumed();
    void rebase();
    void messageDeleted(const int mid);
    void messageFlagsReplaced(const int mid, const int flags);
    void messageFlagsSet(const int mid, const int mask, const int id);
    void messageFlagsReseted(const int mid, const int mask, const int id, const uint date);
    void messageInAdded(const int id, const MessageItem message, const ProfileItem profile);
    void messageOutAdded(const int id, const MessageItem message, const ProfileItem profile);
    void inMessagesRead(const int id, const int mid);
    void outMessagesRead(const int id, const int mid);
    void userOnline(const int uid);
    void userOffline(const int uid, const bool isAway);
    void groupChatUpdated(const int chatId, const bool self);
    void chatTyping(const int id, const int uid, const int chatId);
    void userCall(const int uid, const QString &callId);
    void unreadDialogs(const int count);
};

#endif // LONGPOLL_H
