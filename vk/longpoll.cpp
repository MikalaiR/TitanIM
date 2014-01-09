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

    QString requestUrl = QString("%1?act=a_check&key=%2&ts=%3&wait=%4&mode=9")
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
    int uid = update.value(3, 0).toInt();

    emit messageFlagsSet(mid, mask, uid);
}

void LongPoll::onMessageFlagsReseted(const QVariantList &update)
{
    int mid = update.value(1).toInt();
    int mask = update.value(2).toInt();
    int uid = update.value(3, 0).toInt();

    emit messageFlagsReseted(mid, mask, uid);
}

void LongPoll::onMessageAdded(const QVariantList &update)
{
    QVariantMap response = update.value(1).toMap();

    ProfileList profiles = ProfileParser::parser(response.value("profiles").toList());
    DialogItem dialog = DialogParser::parser(response.value("message").toMap(), profiles);

    if (dialog->message()->isOut())
    {
        emit messageOutAdded(dialog);
    }
    else
    {
        emit messageInAdded(dialog);
    }
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
    int flags = update.value(2).toInt();

    emit chatTyping(uid, flags);
}

void LongPoll::onGroupChatTyping(const QVariantList &update)
{
    int uid = update.value(1).toInt();
    int chatId = update.value(2).toInt();

    emit groupChatTyping(uid, chatId);
}

void LongPoll::onUserCall(const QVariantList &update)
{
    int uid = update.value(1).toInt();
    QString callId = update.value(2).toString();

    emit userCall(uid, callId);
}
