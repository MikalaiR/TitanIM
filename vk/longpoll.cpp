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
#include "messageparser.h"
#include "profileparser.h"

LongPoll::LongPoll(Connection *connection, Engine *engine)
{
    _connection = connection;
    _engine = engine;

    _longPollVars.wait = 25;
    _status = Offline;

    _httpLongPoll = new QNetworkAccessManager(this);
    connect(_httpLongPoll, SIGNAL(finished(QNetworkReply*)), this, SLOT(longPollResponse(QNetworkReply*)));

    connect(this, SIGNAL(rebuild()), this, SLOT(onRebuild()));
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
    return _status == Online;
}

void LongPoll::start()
{
    setStatus(Connecting);
    getLongPollServer();
}

void LongPoll::stop()
{
    setStatus(Offline);
}

void LongPoll::resume()
{
    setStatus(Connecting);
    longPoll();
}

void LongPoll::getLongPollServer()
{
    if (_status == Offline)
    {
        return;
    }

    bool isHttps = _connection->isHttps();

    Packet *packet = new Packet("messages.getLongPollServer");
    packet->setPerishable(true);
    packet->addParam("use_ssl", QString::number(isHttps));
    packet->addParam("need_pts", 1);
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
    _longPollVars.pts = response.value("pts").toString();

    delete sender;

    if (_longPollVars.ts.isEmpty())
    {
        getLongPollServer();
        return;
    }

    if (_status != Offline)
    {
        setStatus(Online);
        longPoll();
    }
}

void LongPoll::longPoll()
{
    if (_status == Offline)
    {
        return;
    }

    if (_longPollVars.ts.isEmpty())
    {
        emit rebuild();
        return;
    }

    QString requestUrl = QString("%1?act=a_check&key=%2&ts=%3&wait=%4&mode=2")
            .arg(_longPollVars.server)
            .arg(_longPollVars.key)
            .arg(_longPollVars.ts)
            .arg(_status == Connecting ? 3 : _longPollVars.wait);

    QNetworkRequest request;
    request.setUrl(requestUrl);
    QNetworkReply *replyLongPoll = _httpLongPoll->get(request);
    connect(this, SIGNAL(stopped()), replyLongPoll, SLOT(abort()));
}

void LongPoll::longPollResponse(QNetworkReply *networkReply)
{
    if (_status == Offline)
    {
        networkReply->deleteLater();
        return;
    }

    if (networkReply->error() != QNetworkReply::NoError || networkReply->bytesAvailable() == 0)
    {
        networkReply->deleteLater();
        getLongPollServer();
        return;
    }

    QVariantMap response = Utils::parseJSON(networkReply->readAll()).toMap();
    networkReply->deleteLater();

    if (response.contains("failed"))
    {
        if (response.value("failed").toInt() == 2 && _status == Online)
        {
            getLongPollServer();
        }
        else
        {
            getLongPollHistory();
        }

        return;
    }

    _longPollVars.ts = response.value("ts").toString();
    QVariantList updates = response.value("updates").toList();

    setStatus(Online);

    handler(updates);
    longPoll();
}

void LongPoll::getLongPollHistory()
{
    Packet *packet = new Packet("execute.messagesGetLongPollHistory");
    packet->setPerishable(true);
    packet->addParam("ts", _longPollVars.ts);
    packet->addParam("pts", _longPollVars.pts);
    packet->addParam("max_msg_id", _engine->maxMsgId());
    packet->addParam("events_limit", 35000);
    packet->addParam("msgs_limit", 7000);
    packet->addParam("fields", "photo_100,online,last_seen,sex");

    connect(packet, SIGNAL(finished(const Packet*,QVariantMap)), this, SLOT(getLongPollHistoryFinished(const Packet*,QVariantMap)));
    connect(packet, SIGNAL(error(const Packet*,const ErrorResponse*)), this, SLOT(getLongPollHistoryError(const Packet*,const ErrorResponse*)));
    _connection->appendQuery(packet);
}

void LongPoll::getLongPollHistoryFinished(const Packet *sender, const QVariantMap &result)
{
    QVariantMap response = result.value("response").toMap();

    if (response.contains("more"))
    {
        emit rebuild();
        delete sender;
        return;
    }

    QVariantMap messagesItem = response.value("messages").toMap();
    MessageList messageList = MessageParser::parser(messagesItem.value("items").toList());

    ProfileList profiles = ProfileParser::parser(response.value("profiles").toList());
    int maxMid = 0;

    for (int i = 0; i < messageList->count(); i++)
    {
        MessageItem message = qobject_cast<MessageItem>(messageList->at(i));
        int mid = message->id();
        int fromId = message->fromId();
        int uid = message->uid();

        if (message->deleted() || message->uid() == 0)
        {
            continue;
        }

        if (mid > maxMid)
        {
            maxMid = mid;
        }

        if (message->isOut())
        {
            emit messageOutAdded(fromId, message, profiles->item(uid));
        }
        else
        {
            emit messageInAdded(fromId, message, profiles->item(uid));
        }
    }

    _engine->setMaxMsgId(maxMid);

    QVariantList updates = response.value("history").toList();
    handler(updates);

    setStatus(Online);
    getLongPollServer();

    emit obsoleteFriendsOnline();
    delete sender;
}

void LongPoll::getLongPollHistoryError(const Packet *sender, const ErrorResponse *errorResponse)
{
    if (errorResponse->code() != ErrorResponse::ServerIsNotAvailable)
    {
        emit rebuild();
    }

    delete sender;
}

void LongPoll::onRebuild()
{
    getLongPollServer();
}

void LongPoll::setStatus(const Status status)
{
    if (status != _status)
    {
        _status = status;

        switch (_status) {
        case Online:
            emit started();
            break;

        case Offline:
            emit stopped();
            break;
        }
    }
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
            if (update.count() > 4)
            {
                onMessageAdded(update);
            }

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

void LongPoll::messageAdd(const MessageItem message, const ProfileItem profile)
{
    _engine->setMaxMsgId(message->id());

    if (message->isOut())
    {
        emit messageOutAdded(message->fromId(), message, profile);
    }
    else
    {
        emit messageInAdded(message->fromId(), message, profile);
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

    QString action;
    int actionMid = 0;
    QString actionText;

    if (additional.contains("source_act"))
    {
        action = additional.value("source_act").toString();
        actionMid = additional.contains("source_mid") ? additional.value("source_mid").toInt() : 0;
        actionText = additional.contains("source_text") ? additional.value("source_text").toString() : "";
        actionText = additional.contains("source_email") ? additional.value("source_email").toString() : actionText;
    }

    message->beginChangeGroupProperties();

    message->setId(mid);
    message->setFromId(fromId);
    message->setDate(date);
    message->setIsUnread(flags & Unread);
    message->setIsOut(flags & Outbox);
    message->setBody(body, emoji, false);
    message->setTitle(title);

    if (additional.contains("from"))
        message->setUid(additional.value("from").toInt());

    message->setActionMid(actionMid);
    message->setActionText(actionText);
    message->setAction(action);

    message->endChangeGroupProperties();

    if (additional.contains("attach1") || additional.contains("geo") || additional.contains("fwd"))
    {
        message->getAllFields(_connection);
    }

    ProfileItem profile = _engine->getProfile(message->uid());

    if (!profile->isEmpty())
    {
        messageAdd(message, profile);
    }
    else
    {
        auto pConnection = QSharedPointer<QMetaObject::Connection>(new QMetaObject::Connection);
        QMetaObject::Connection &connection = *pConnection;

        auto lambda  = [this, message, profile, pConnection, &connection]()
        {
            QObject::disconnect(connection);
            messageAdd(message, profile);
        };

        connection = QObject::connect(profile.data(), &ProfileItemPrivate::fullNameChanged, lambda);
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

    emit userOnline(uid);
}

void LongPoll::onUserOffline(const QVariantList &update)
{
    int uid = qAbs(update.value(1).toInt());
    bool isAway = update.value(2).toBool();

    emit userOffline(uid, isAway);
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
