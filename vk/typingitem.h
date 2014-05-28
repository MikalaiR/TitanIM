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

#ifndef TYPINGITEM_H
#define TYPINGITEM_H

#include <QObject>
#include <QSharedPointer>
#include <QTimer>
#include "messagebase.h"

class TypingItemPrivate : public MessageBase
{
    Q_OBJECT

public:
    TypingItemPrivate();
    int uid() const;
    void setUid(const int uid);
    inline bool isActive() const { return !_deleted; }

private:
    int _uid;
    QTimer *_timer;

public slots:
    void restart();
    void stop();

protected slots:
    void setIsActive(const bool isActive);
    void onTimerTimeout();

signals:
    void activeChanged(const int id, const int uid, const bool isActive);
};

typedef QSharedPointer<TypingItemPrivate> TypingItem;

#endif // TYPINGITEM_H
