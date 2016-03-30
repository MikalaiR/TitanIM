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

#ifndef TYPINGHANDLER_H
#define TYPINGHANDLER_H

#include <QObject>
#include <QMap>
#include <QSet>
#include "typingitem.h"

class TypingHandler : public QObject
{
    Q_OBJECT

public:
    TypingHandler(QObject *parent = 0);
    bool contains(const int uid);
    void restart(const int uid);
    void stop(const int uid);
    void setTyping(const TypingItem typing);
    void setTyping(const int uid);
    bool isActive() const;
    bool isActive(const int uid) const;
    inline QList<TypingItem> toList() const { return _typingUserList.values(); }

private:
    QMap<int, TypingItem> _typingUserList;
    QSet<int> _activeTypingList;
    bool _isActive;

protected:
    void updateActive(const int uid, const bool isActive);
    void setIsActive(const bool isActive);

protected slots:
    void onTypingItemActiveChanged(const int id, const int uid, const bool isActive);

signals:
    void activeChanged(const bool isActive);
    void newTyping(const TypingItem typing);
};

#endif // TYPINGHANDLER_H
