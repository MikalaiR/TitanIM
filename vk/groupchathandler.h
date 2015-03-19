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

#ifndef GROUPCHATHANDLER_H
#define GROUPCHATHANDLER_H

#include <QObject>
#include <QStringList>
#include "global.h"
#include "client.h"
#include "profileparser.h"

class GroupChatHandler : public QObject
{
    Q_OBJECT

public:
    GroupChatHandler(const int chatId);
    int id() const;
    int chatId() const;
    void addUser(ProfileItem profile);
    void removeUser(const int uid);
    ProfileItem user(const int uid);
    QStringList avatars() const;
    void refreshAvatars();
    int usersCount() const;
    void setUsersCount(const int usersCount);
    QString peopleConversationText() const;
    void setCover(const QString &cover);
    QString title() const;
    void setTitle(const QString &title);
    int adminId() const;
    void setAdminId(const int adminId);
    bool isCover() const;
    bool kicked() const;
    bool left() const;

private:
    int _id;
    int _chatId;
    ProfileList _users;
    QStringList _avatars;
    int _usersCount;
    QString _cover;
    QString _title;
    int _adminId;
    bool _kicked;
    bool _left;

public slots:
    void setActionMsg(const MessageItem msg);
    void getAllFields();
    void updatePeopleConversationText();

protected slots:
    void onUserChanged(const int i);
    void loadFinished(const Packet *sender, const QVariantMap &result);

signals:
    void propertyChanged(const int chatId, const QString &propertyName);
};

#endif // GROUPCHATHANDLER_H
