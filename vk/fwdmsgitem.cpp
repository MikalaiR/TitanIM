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

#include "fwdmsgitem.h"

ProfileList fwdMsgItemPrivate::_users;

fwdMsgItemPrivate::fwdMsgItemPrivate()
{
    setAttachmentType(Fwd_messages);

    if (!_users)
    {
        _users = ProfileList::create(); //todo clear memory
    }
}

QVariantList fwdMsgItemPrivate::messages() const
{
    QVariantList list;

    for (int i = 0; i < _messages->count(); i++)
    {
        list.append(QVariant::fromValue(_messages->at(i).data()));
    }

    return list;
}

void fwdMsgItemPrivate::setMessages(const MessageList messages)
{
    _messages = messages;
}

QVariant fwdMsgItemPrivate::getProfile(const int id) const
{
    int index = _users->indexOf(id);

    if (index != -1)
    {
        return QVariant::fromValue(_users->at(index).data());
    }
    else
    {
        ProfileItem profile = ProfileItem::create();
        profile->setId(id);
        QQmlEngine::setObjectOwnership(profile.data(), QQmlEngine::CppOwnership);
        _users->add(profile);
        profile->getAllFields(Client::instance()->connection());
        return QVariant::fromValue(profile.data());
    }
}
