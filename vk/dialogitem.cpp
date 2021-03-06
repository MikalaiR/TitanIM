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

#include "dialogitem.h"
#include "dialogparser.h"

DialogItemPrivate::DialogItemPrivate()
{
    _groupChatHandler = 0;
    _typingHandler = 0;
    _unreadCount = 0;
    _isCurrent = false;
    _isLoading = false;

    connect(this, &DialogItemPrivate::propertyChanged, this, &DialogItemPrivate::displayNameChanged); //todo
    connect(this, &DialogItemPrivate::propertyChanged, this, &DialogItemPrivate::decorationChanged); //todo
    connect(this, &DialogItemPrivate::propertyChanged, this, &DialogItemPrivate::descriptionChanged); //todo
}

DialogItemPrivate::~DialogItemPrivate()
{
    if (_groupChatHandler)
    {
        _groupChatHandler->deleteLater();
    }
}

QString DialogItemPrivate::displayName() const
{
    return isGroupChat() ? _groupChatHandler->title() : _profile->fullName();
}

QStringList DialogItemPrivate::decoration() const
{
    return isGroupChat() ? _groupChatHandler->avatars() : QStringList(_profile->photoMediumRect());
}

QString DialogItemPrivate::description() const
{
    if (isGroupChat())
    {
        return _groupChatHandler->peopleConversationText();
    }
    else
    {
        return _profile->lastSeenText();
    }
}

bool DialogItemPrivate::isGroupChat() const
{
    return _groupChatHandler;
}

ProfileItem DialogItemPrivate::profile() const
{
    return _profile;
}

void DialogItemPrivate::setProfile(const ProfileItem profile)
{
    if (_profile.data() != profile.data())
    {
        if (_profile)
        {
            disconnect(_profile.data(), &ProfileItemPrivate::propertyChanged, this, &DialogItemPrivate::onProfilePropertyChanged);
        }

        _profile = profile;

        if (_id == 0)
        {
            _id = _profile->id();
        }

        connect(profile.data(), &ProfileItemPrivate::propertyChanged, this, &DialogItemPrivate::onProfilePropertyChanged);
        emitPropertyChanged("profile");
    }
}

MessageItem DialogItemPrivate::message() const
{
    return _message;
}

void DialogItemPrivate::setMessage(const MessageItem message)
{
    if (_message.data() != message.data())
    {
        if (_message)
        {
            disconnect(_message.data(), &MessageItemPrivate::propertyChanged, this, &DialogItemPrivate::onMessagePropertyChanged);
        }

        _message = message;

        if (!_groupChatHandler)
        {
            _id = _message->fromId();
        }
        else if (_message->action() != MessageItemPrivate::None)
        {
            _groupChatHandler->setActionMsg(_message);
        }

        if (!message->isOut() && _typingHandler && _typingHandler->isActive(message->uid()))
        {
            _typingHandler->stop(message->uid());
        }

        connect(message.data(), &MessageItemPrivate::propertyChanged, this, &DialogItemPrivate::onMessagePropertyChanged);
        emitPropertyChanged("message");
    }
}

GroupChatHandler *DialogItemPrivate::groupChatHandler() const
{
    QQmlEngine::setObjectOwnership(_groupChatHandler, QQmlEngine::CppOwnership);
    return _groupChatHandler;
}

void DialogItemPrivate::setGroupChatHandler(GroupChatHandler *groupChatHandler)
{
    if (_groupChatHandler != groupChatHandler)
    {
        if (_groupChatHandler)
        {
            disconnect(_groupChatHandler, &GroupChatHandler::propertyChanged, this, &DialogItemPrivate::onGroupChatPropertyChanged);
            delete _groupChatHandler;
        }

        _groupChatHandler = groupChatHandler;

        _id = groupChatHandler->id();

        connect(_groupChatHandler, &GroupChatHandler::propertyChanged, this, &DialogItemPrivate::onGroupChatPropertyChanged);
        emitPropertyChanged("groupChat");
    }
}

TypingHandler *DialogItemPrivate::typingHandler() const
{
    return _typingHandler;
}

bool DialogItemPrivate::isTyping() const
{
    return _typingHandler && _typingHandler->isActive();
}

int DialogItemPrivate::unreadCount() const
{
    return _unreadCount;
}

void DialogItemPrivate::setUnreadCount(const int unreadCount)
{
    if (_unreadCount != unreadCount)
    {
        _unreadCount = unreadCount;
        emitPropertyChanged("unreadCount");
    }
}

void DialogItemPrivate::incUnreadDialogs()
{
    setUnreadCount(_unreadCount + 1);
}

void DialogItemPrivate::decUnreadDialogs()
{
    setUnreadCount(_unreadCount - 1);
}

bool DialogItemPrivate::isCurrent() const
{
    return _isCurrent;
}

void DialogItemPrivate::setCurrent(const bool current)
{
    if (_isCurrent != current)
    {
        _isCurrent = current;
        emitPropertyChanged("isCurrent");
    }
}

bool DialogItemPrivate::isLoading() const
{
    return _isLoading;
}

bool DialogItemPrivate::isEmpty() const
{
    //todo _message.isEmpty()
    bool result = !(_message && _message->id() != -1) ||
                  !(_profile && (!_profile->isEmpty() || _profile->isLoading())) ||
                  (_message->isGroupChat() && !_groupChatHandler);

    return result;
}

void DialogItemPrivate::createStructure()
{
    if (!_profile && _message)
    {
        ProfileItem profile = ProfileItem::create();
        profile->setId(_message->uid());
        setProfile(profile);
    }
    else if (!_message && _profile)
    {
        MessageItem message = MessageItem::create();
        message->setId(-1);
        message->setUid(_profile->id());
        message->setDate(Utils::currentDateTime());
        setMessage(message);
    }
    else if (!_profile && !_message && (_id != 0))
    {
        ProfileItem profile = ProfileItem::create();
        profile->setId(_id < GROUP_CHAT_OFFSET ? _id : 0);

        MessageItem message = MessageItem::create();
        message->setId(-1);
        message->setUid(profile->id());
        message->setDate(Utils::currentDateTime());

        beginChangeGroupProperties();

        setProfile(profile);
        setMessage(message);

        endChangeGroupProperties();
    }
}

void DialogItemPrivate::setIsLoading(const bool isLoading)
{
    if (_isLoading != isLoading)
    {
        _isLoading = isLoading;
        emitPropertyChanged("isLoading");
    }
}

void DialogItemPrivate::getAllFields(Connection *connection, const bool want)
{
    if (_isLoading || _id == 0 || (want && !isEmpty()))
        return;

    setIsLoading(true);

    Packet *packet = new Packet("execute.messagesGetDialog");
    packet->addParam("user_id", _id);
    packet->addParam("preview_length", 50);
    packet->addParam("fields", "photo_100,online,last_seen,sex,domain,bdate,city,contacts,friend_status,blacklisted_by_me");

    connect(packet, &Packet::finished, this, &DialogItemPrivate::loadFinished);
    connection->appendQuery(packet);
}

void DialogItemPrivate::getLastMessage(Connection *connection)
{
    if (_isLoading)
        return;

    setIsLoading(true);

    HistoryPacket *historyPacket = new HistoryPacket(connection);
    connect(historyPacket, &HistoryPacket::history, this, &DialogItemPrivate::onGetMessageFinished);

    historyPacket->load(_id, 0, 1);
}

void DialogItemPrivate::typing(const int uid)
{
    if (!_typingHandler)
    {
        _typingHandler = new TypingHandler(this);
        connect(_typingHandler, &TypingHandler::activeChanged, this, &DialogItemPrivate::onTypingActiveChanged);
        connect(_typingHandler, &TypingHandler::newTyping, this, &DialogItemPrivate::newTyping);
    }

    _typingHandler->setTyping(uid);
}

void DialogItemPrivate::loadFinished(const Packet *sender, const QVariantMap &result)
{
    setIsLoading(false);

    QVariantMap response = result.value("response").toMap();
    QVariantMap dialogItem = response.value("dialog").toMap();
    ProfileList profiles = ProfileParser::parser(response.value("profiles").toList());
    DialogParser::parser(dialogItem, this, profiles);

    delete sender;
}

void DialogItemPrivate::onProfilePropertyChanged(const int uid, const QString &propertyName)
{
    QString property = QString("profile.%1").arg(propertyName);
    emitPropertyChanged(property);
}

void DialogItemPrivate::onMessagePropertyChanged(const int mid, const QString &propertyName)
{
    QString property = QString("message.%1").arg(propertyName);
    emitPropertyChanged(property);
}

void DialogItemPrivate::onGroupChatPropertyChanged(const int chatId, const QString &propertyName)
{
    QString property = QString("groupChat.%1").arg(propertyName);
    emitPropertyChanged(property);
}

void DialogItemPrivate::onTypingActiveChanged(const bool isActive)
{
    emitPropertyChanged("typing.isActive");
}

void DialogItemPrivate::onGetMessageFinished(const HistoryPacket *sender, const int id, const MessageList &messages)
{
    setIsLoading(false);

    setUnreadCount(sender->unreadCount());

    if (!messages->count())
    {
        MessageItem msg = MessageItem::create();
        msg->setFromId(_id);
        setMessage(msg);
    }
    else if (messages->at(0)->id() != message()->id())
    {
        setMessage(qobject_cast<MessageItem>(messages->at(0)));
    }

    delete sender;
}
