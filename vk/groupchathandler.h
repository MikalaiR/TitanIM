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
#include <QVariantList>
#include "notifypropertybase.h"
#include "global.h"
#include "client.h"
#include "profileparser.h"

class GroupChatHandler : public NotifyPropertyBase
{
    Q_OBJECT

    Q_PROPERTY(QVariantList users READ users NOTIFY usersChanged)
    Q_PROPERTY(int usersCount READ usersCount NOTIFY usersChanged)
    Q_PROPERTY(int adminId READ adminId CONSTANT)
    Q_PROPERTY(bool left READ left NOTIFY leftChanged)
    Q_PROPERTY(bool kicked READ kicked NOTIFY kickedChanged)

public:
    GroupChatHandler(const int chatId);
    int chatId() const;
    void setUsers(ProfileList profiles);
    ProfileItem profile(const int uid);
    QVariant user(const int uid);
    ProfileList profiles();
    QVariantList users();
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
    void setKicked(const bool kicked);
    bool left() const;
    void setLeft(const bool left);

private:
    int _chatId;
    ProfileItem _selfProfile;
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
    void addChatUser(const int uid);
    void removeChatUser(const int uid=0);

protected slots:
    void onUserChanged(const int i);
    void loadFinished(const Packet *sender, const QVariantMap &result);

signals:
    void usersChanged();
    void leftChanged();
    void kickedChanged();
};

#endif // GROUPCHATHANDLER_H
