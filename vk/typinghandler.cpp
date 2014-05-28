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

#include "typinghandler.h"

TypingHandler::TypingHandler(QObject *parent) :
    QObject(parent)
{
    _isActive = false;
}

bool TypingHandler::contains(const int uid)
{
    return _typingUserList.contains(uid);
}

void TypingHandler::restart(const int uid)
{
    _typingUserList.value(uid)->restart();
}

void TypingHandler::stop(const int uid)
{
    _typingUserList.value(uid)->stop();
}

void TypingHandler::setTyping(const TypingItem typing)
{
    _typingUserList[typing->uid()] = typing;
    connect(typing.data(), SIGNAL(activeChanged(int,int,bool)), this, SLOT(onTypingItemActiveChanged(int,int,bool)));
    updateActive(typing->uid(), typing->isActive());

    emit newTyping(typing);
}

void TypingHandler::setTyping(const int uid)
{
    if (contains(uid))
    {
        restart(uid);
    }
    else
    {
        TypingItem typing = TypingItem::create();
        typing->setUid(uid);

        setTyping(typing);
    }
}

bool TypingHandler::isActive() const
{
    return _isActive;
}

bool TypingHandler::isActive(const int uid) const
{
    return _activeTypingList.contains(uid);
}

void TypingHandler::updateActive(const int uid, const bool isActive)
{
    if (isActive)
    {
        _activeTypingList.insert(uid);
    }
    else
    {
        _activeTypingList.remove(uid);
    }

    setIsActive(_activeTypingList.count());
}

void TypingHandler::setIsActive(const bool isActive)
{
    if (_isActive != isActive)
    {
        _isActive = isActive;
        emit activeChanged(_isActive);
    }
}

void TypingHandler::onTypingItemActiveChanged(const int id, const int uid, const bool isActive)
{
    updateActive(uid, isActive);
}
