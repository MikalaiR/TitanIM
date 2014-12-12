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

#include "longpoll.h"

LongPoll::LongPoll(Connection *connection)
{
    _connection = connection;

    _longPollVars.wait = 25;
    _running = false;

    httpLongPoll = new QNetworkAccessManager(this);
    connect(httpLongPoll, SIGNAL(finished(QNetworkReply*)), this, SLOT(longPollResponse(QNetworkReply*)));
}

int LongPoll::wait() const
{
    return _longPollVars.wait;
}

void LongPoll::setWait(const int sec)
{
    _longPollVars.wait = sec;
}

bool LongPoll::isRunning() const
{
    return _running;
}

void LongPoll::setRunning(const bool running)
{
    if (running != _running)
    {
        _running = running;
        onRunningChanged(_running);
    }
}

void LongPoll::getLongPollServer()
{
    if (!isRunning())
    {
        return;
    }

    bool isHttps = _connection->isHttps();

    Packet *packet = new Packet("messages.getLongPollServer");
    packet->addParam("use_ssl", QString::number(isHttps));
    packet->setDataUser(isHttps ? "https://" : "http://");
    connect(packet, SIGNAL(finished(const Packet*,QVariantMap)), this, SLOT(getLongPollServerFinished(const Packet*,QVariantMap)));
    _connection->appendQuery(packet);
}

void LongPoll::getLongPollServerFinished(const Packet *sender, const QVariantMap &result)
{
    QVariantMap response = result.value("response").toMap();

    _longPollVars.server = sender->dataUser() + response.value("server").toString();
    _longPollVars.key = response.value("key").toString();
    _longPollVars.ts = response.value("ts").toString();

    delete sender;

    longPoll();
}

void LongPoll::longPoll()
{
    if (!isRunning())
    {
        return;
    }

    QString requestUrl = QString("%1?act=a_check&key=%2&ts=%3&wait=%4&mode=2")
            .arg(_longPollVars.server)
            .arg(_longPollVars.key)
            .arg(_longPollVars.ts)
            .arg(_longPollVars.wait);

    QNetworkRequest request;
    request.setUrl(requestUrl);
    httpLongPoll->get(request);
}

void LongPoll::longPollResponse(QNetworkReply *networkReply)
{
    if (!isRunning())
    {
        return;
    }

    if (networkReply->error() != QNetworkReply::NoError)
    {
        getLongPollServer();
        return;
    }

    QVariantMap response = Utils::parseJSON(networkReply->readAll()).toMap();
    networkReply->deleteLater();

    if (response.isEmpty() || response.contains("failed"))
    {
        getLongPollServer();
        return;
    }

    _longPollVars.ts = response.value("ts").toString();
    QVariantList updates = response.value("updates").toList();

    handler(updates);
    longPoll();
}

void LongPoll::handler(const QVariantList &updates)
{
    for (int i = 0; i < updates.size(); i++) {
        QVariantList update = updates.at(i).toList();
        int updateType = update.value(0, -1).toInt();

        switch (updateType) {
        case MessageDeleted:
        {
            onMessageDeleted(update);
            break;
        }
            
        case MessageFlagsReplaced:
        {
            onMessageFlagsReplaced(update);
            break;
        }
            
        case MessageFlagsSet:
        {
            onMessageFlagsSet(update);
            break;
        }
            
        case MessageFlagsReseted:
        {
            onMessageFlagsReseted(update);
            break;
        }
            
        case MessageAdded:
        {
            onMessageAdded(update);
            break;
        }

        case InMessagesRead:
        {
            onInMessagesRead(update);
            break;
        }

        case OutMessagesRead:
        {
            onOutMessagesRead(update);
            break;
        }

        case UserOnline:
        {
            onUserOnline(update);
            break;
        }
            
        case UserOffline:
        {
            onUserOffline(update);
            break;
        }
            
        case GroupChatUpdated:
        {
            onGroupChatUpdated(update);
            break;
        }
            
        case ChatTyping:
        {
            onChatTyping(update);
            break;
        }
            
        case GroupChatTyping:
        {
            onGroupChatTyping(update);
            break;
        }
            
        case UserCall:
        {
            onUserCall(update);
            break;
        }

        case UnreadDialogs:
        {
            onUnreadDialogs(update);
            break;
        }

        default:
            break;
        }
    }
}

void LongPoll::onRunningChanged(const bool running)
{
    if (running)
    {
        getLongPollServer();
    }
}

void LongPoll::onMessageDeleted(const QVariantList &update)
{
    int mid = update.value(1).toInt();

    emit messageDeleted(mid);
}

void LongPoll::onMessageFlagsReplaced(const QVariantList &update)
{
    int mid = update.value(1).toInt();
    int flags = update.value(2).toInt();

    emit messageFlagsReplaced(mid, flags);
}

void LongPoll::onMessageFlagsSet(const QVariantList &update)
{
    int mid = update.value(1).toInt();
    int mask = update.value(2).toInt();
    int id = update.value(3, 0).toInt();

    emit messageFlagsSet(mid, mask, id);
}

void LongPoll::onMessageFlagsReseted(const QVariantList &update)
{
    int mid = update.value(1).toInt();
    int mask = update.value(2).toInt();
    int id = update.value(3, 0).toInt();
    uint date = update.value(4, 0).toUInt();

    emit messageFlagsReseted(mid, mask, id, date);
}

void LongPoll::onMessageAdded(const QVariantList &update)
{
    MessageItem message = MessageItem::create();

    int mid = update.value(1).toInt();
    int flags = update.value(2).toInt();
    int fromId = update.value(3).toInt();
    QDateTime date = QDateTime::fromTime_t(update.value(4).toUInt()).toLocalTime();
    QString title = update.value(5).toString();
    QString body = update.value(6).toString();
    QVariantMap additional = update.value(7).toMap();
    bool emoji = (additional.contains("emoji") && additional.value("emoji").toInt()) ? true : false;

    message->beginChangeGroupProperties();

    message->setId(mid);
    message->setFromId(fromId);
    message->setDate(date);
    message->setIsUnread(flags & Unread);
    message->setDeliveryReport(true);
    message->setIsOut(flags & Outbox);
    message->setBody(body, emoji, false);
    message->setTitle(title);

    if (additional.contains("from"))
        message->setUid(additional.value("from").toInt());

    message->endChangeGroupProperties();

    if (additional.contains("attach1") || additional.contains("geo"))
    {
        message->getAllFields(_connection);
    }

    if (message->isOut())
    {
        emit messageOutAdded(fromId, message);
    }
    else
    {
        emit messageInAdded(fromId, message);
    }
}

void LongPoll::onInMessagesRead(const QVariantList &update)
{
    int id = update.value(1).toInt();
    int mid = update.value(2).toInt();

    emit inMessagesRead(id, mid);
}

void LongPoll::onOutMessagesRead(const QVariantList &update)
{
    int id = update.value(1).toInt();
    int mid = update.value(2).toInt();

    emit outMessagesRead(id, mid);
}

void LongPoll::onUserOnline(const QVariantList &update)
{
    int uid = qAbs(update.value(1).toInt());
    bool online = true;

    emit userStatusChanged(uid, online);
}

void LongPoll::onUserOffline(const QVariantList &update)
{
    int uid = qAbs(update.value(1).toInt());
    bool online = false;

    emit userStatusChanged(uid, online);
}

void LongPoll::onGroupChatUpdated(const QVariantList &update)
{
    int chatId = update.value(1).toInt();
    bool self = update.value(2).toInt() == 1;

    emit groupChatUpdated(chatId, self);
}

void LongPoll::onChatTyping(const QVariantList &update)
{
    int uid = update.value(1).toInt();

    emit chatTyping(uid, uid, 0);
}

void LongPoll::onGroupChatTyping(const QVariantList &update)
{
    int uid = update.value(1).toInt();
    int chatId = update.value(2).toInt();

    int id = chatId + GROUP_CHAT_OFFSET;

    emit chatTyping(id, uid, chatId);
}

void LongPoll::onUserCall(const QVariantList &update)
{
    int uid = update.value(1).toInt();
    QString callId = update.value(2).toString();

    emit userCall(uid, callId);
}

void LongPoll::onUnreadDialogs(const QVariantList &update)
{
    int count = update.value(1).toInt();

    emit unreadDialogs(count);
}
