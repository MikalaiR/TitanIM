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

GroupChatHandler::GroupChatHandler(const int chatId)
{
    _chatId = chatId;
    _id = _chatId + GROUP_CHAT_OFFSET;

    _users = ProfileList::create();
    _users->add(Client::instance()->engine()->getProfile());
    connect(_users.data(), SIGNAL(itemChanged(int)), this, SLOT(onUserChanged(int)));

    _cover.clear();
    _usersCount = 0;
    _adminId = 0;
    _kicked = false;
    _left = false;
}

int GroupChatHandler::id() const
{
    return _id;
}

int GroupChatHandler::chatId() const
{
    return _chatId;
}

void GroupChatHandler::addUser(ProfileItem profile)
{
    if (profile->id() != Client::instance()->uid())
    {
        _users->add(profile);
        setUsersCount(_usersCount + 1);
        refreshAvatars();
        emit propertyChanged(_chatId, "users");
    }
    else
    {
        getAllFields();
    }
}

void GroupChatHandler::removeUser(const int uid)
{
    if (uid != Client::instance()->uid())
    {
        int index = _users->indexOf(uid);
        if (index != -1)
        {
            _users->removeAt(index);
        }

        setUsersCount(_usersCount - 1);
        refreshAvatars();
        emit propertyChanged(_chatId, "users");
    }
    else
    {
        getAllFields();
    }
}

ProfileItem GroupChatHandler::user(const int uid)
{
    return _users->item(uid);
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

        if (profile->id() != Client::instance()->uid() || _users->count() < 3)
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

        emit propertyChanged(_chatId, "usersCount");
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
        emit propertyChanged(_chatId, "cover");
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
        _title = title;
        emit propertyChanged(_chatId, "title");
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
        emit propertyChanged(_chatId, "adminId");
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

bool GroupChatHandler::left() const
{
    return _left;
}

void GroupChatHandler::setActionMsg(const MessageItem msg)
{
    switch (msg->action()) {
    case MessageItemPrivate::Chat_photo_update:
    {
        getAllFields(); //todo
        break;
    }

    case MessageItemPrivate::Chat_photo_remove:
    {
        setCover("");
        break;
    }

    case MessageItemPrivate::Chat_title_update:
    {
        setTitle(msg->actionText());
        break;
    }

    case MessageItemPrivate::Chat_invite_user:
    case MessageItemPrivate::Chat_invite_self:
    {
        ProfileItem p = Client::instance()->engine()->getProfile(msg->actionMid());
        addUser(p);
        break;
    }

    case MessageItemPrivate::Chat_kick_user:
    case MessageItemPrivate::Chat_kick_self:
    {
        removeUser(msg->actionMid());
        break;
    }
    }
}

void GroupChatHandler::getAllFields()
{
    Packet *packet = new Packet("messages.getChat");
    packet->addParam("chat_id", _chatId);
    packet->addParam("fields", "photo_100");

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
    emit propertyChanged(_chatId, "users");
}

void GroupChatHandler::loadFinished(const Packet *sender, const QVariantMap &result)
{
    QVariantMap response = result.value("response").toMap();

    _title = response.value("title").toString();
    _adminId = response.value("adminId").toInt();
    _cover = response.value("photo_100").toString();
    _kicked = (response.contains("kicked") && response.value("kicked").toInt());
    _left = (response.contains("left") && response.value("left").toInt());
    ProfileList profiles = ProfileParser::parser(response.value("users").toList());

    _users->clear();
    _users->add(profiles->toList());
    setUsersCount(_users->count());
    refreshAvatars();

    emit propertyChanged(_chatId, "all");
    delete sender;
}
