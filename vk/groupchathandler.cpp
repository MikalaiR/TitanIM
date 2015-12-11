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

#include "groupchathandler.h"
#include "client.h"

GroupChatHandler::GroupChatHandler(const int chatId) : NotifyPropertyBase()
{
    _chatId = chatId;
    _id = _chatId + GROUP_CHAT_OFFSET;

    _selfProfile = Client::instance()->engine()->getProfile();

    _users = ProfileList::create();
    connect(_users.data(), SIGNAL(itemChanged(int)), this, SLOT(onUserChanged(int)));

    _cover.clear();
    _usersCount = 0;
    _adminId = 0;
    _kicked = false;
    _left = false;
}

int GroupChatHandler::chatId() const
{
    return _chatId;
}

void GroupChatHandler::setUsers(ProfileList profiles)
{
    _users->clear();
    _users->add(profiles->toList());
    setUsersCount(_users->count());
    refreshAvatars();

    emitPropertyChanged("users");
    emit usersChanged();
}

ProfileItem GroupChatHandler::profile(const int uid)
{
    return _users->item(uid);
}

QVariant GroupChatHandler::user(const int uid)
{
    ProfileItem profile = _users->item(uid);

    QQmlEngine::setObjectOwnership(profile.data(), QQmlEngine::CppOwnership);
    return QVariant::fromValue(profile.data());
}

QVariantList GroupChatHandler::users()
{
    QVariantList list;

    for (int i = 0; i < _users->count(); i++)
    {
        ProfileItem profile = _users->at(i);
        if (profile->isEmpty())
        {
            profile->getAllFields(Client::instance()->connection());
        }

        QQmlEngine::setObjectOwnership(profile.data(), QQmlEngine::CppOwnership);
        list.append(QVariant::fromValue(profile.data()));
    }

    return list;
}

ProfileList GroupChatHandler::profiles()
{
    return _users;
}

QStringList GroupChatHandler::avatars() const
{
    if (_usersCount > 0)
    {
        return isCover() ? QStringList(_cover) : _avatars;
    }
    else
    {
        return QStringList("https://vk.com/images/icons/multichat_50.png");
    }
}

void GroupChatHandler::refreshAvatars()
{
    _avatars.clear();

    for (int i = 0; i < _users->count(); i++)
    {
        ProfileItem profile = _users->at(i);

        if (profile->id() != _selfProfile->id() || _users->count() < 3)
        {
            _avatars.append(profile->photoMediumRect());
        }
    }
}

int GroupChatHandler::usersCount() const
{
    return _usersCount;
}

void GroupChatHandler::setUsersCount(const int usersCount)
{
    if (_usersCount != usersCount)
    {
        _usersCount = usersCount;

        if (_usersCount == 0)
        {
            _users->clear();
            refreshAvatars();
        }

        emitPropertyChanged("usersCount");
    }
}

QString GroupChatHandler::peopleConversationText() const
{
    return Utils::peopleConversation(_usersCount);
}

void GroupChatHandler::setCover(const QString &cover)
{
    if (_cover != cover)
    {
        _cover = cover;
        emitPropertyChanged("cover");
    }
}

QString GroupChatHandler::title() const
{
    return _title;
}

void GroupChatHandler::setTitle(const QString &title)
{
    if (_title != title)
    {
        _title = Emoticons::instance()->fromEmoji(title);
        emitPropertyChanged("title");
    }
}

int GroupChatHandler::adminId() const
{
    return _adminId;
}

void GroupChatHandler::setAdminId(const int adminId)
{
    if (_adminId != adminId)
    {
        _adminId = adminId;
        emitPropertyChanged("adminId");
    }
}

bool GroupChatHandler::isCover() const
{
    return !(_cover.isEmpty());
}

bool GroupChatHandler::kicked() const
{
    return _kicked;
}

void GroupChatHandler::setKicked(const bool kicked)
{
    if (_kicked != kicked)
    {
        _kicked = kicked;
        emitPropertyChanged("kicked");
        emit kickedChanged();
    }
}

bool GroupChatHandler::left() const
{
    return _left;
}

void GroupChatHandler::setLeft(const bool left)
{
    if (_left != left)
    {
        _left = left;
        emitPropertyChanged("left");
        emit leftChanged();
    }
}

void GroupChatHandler::setActionMsg(const MessageItem msg)
{
    switch (msg->action()) {
    case MessageItemPrivate::Chat_photo_update:
    case MessageItemPrivate::Chat_photo_remove:
    {
        getAllFields();
        break;
    }
    }
}

void GroupChatHandler::addChatUser(const int uid)
{
    Packet *packet = new Packet("messages.addChatUser");
    packet->addParam("chat_id", _chatId);
    packet->addParam("user_id", uid);
    Client::instance()->connection()->appendQuery(packet);
}

void GroupChatHandler::removeChatUser(const int uid)
{
    Packet *packet = new Packet("messages.removeChatUser");
    packet->addParam("chat_id", _chatId);
    packet->addParam("user_id", uid == 0 ? _selfProfile->id() : uid);
    Client::instance()->connection()->appendQuery(packet);
}

void GroupChatHandler::getAllFields()
{
    Packet *packet = new Packet("execute.messagesGetChat");
    packet->addParam("chat_id", _chatId);
    packet->addParam("fields", "photo_100,online,last_seen,sex,domain");

    connect(packet, SIGNAL(finished(const Packet*,QVariantMap)), this, SLOT(loadFinished(const Packet*,QVariantMap)));
    Client::instance()->connection()->appendQuery(packet);
}

void GroupChatHandler::updatePeopleConversationText()
{
    //todo
}

void GroupChatHandler::onUserChanged(const int i)
{
    refreshAvatars();
    emitPropertyChanged("users");
}

void GroupChatHandler::loadFinished(const Packet *sender, const QVariantMap &result)
{
    QVariantMap response = result.value("response").toMap();

    beginChangeGroupProperties();

    setTitle(Emoticons::instance()->fromEmoji(response.value("title").toString()));
    setAdminId(response.value("admin_id").toInt());
    setCover(response.value("photo_100").toString());
    setKicked(response.contains("kicked") && response.value("kicked").toInt());
    setLeft(response.contains("left") && response.value("left").toInt());
    setUsers(ProfileParser::parser(response.value("users").toList()));

    endChangeGroupProperties();

    delete sender;
}
