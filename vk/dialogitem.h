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

#ifndef DIALOGITEM_H
#define DIALOGITEM_H

#include <QObject>
#include <QSharedPointer>
#include "notifypropertybase.h"
#include "profileitem.h"
#include "messageitem.h"
#include "groupchathandler.h"
#include "historypacket.h"
#include "typinghandler.h"

class DialogItemPrivate : public NotifyPropertyBase
{
    Q_OBJECT

    Q_PROPERTY(QString displayName READ displayName NOTIFY displayNameChanged)
    Q_PROPERTY(QStringList decoration READ decoration NOTIFY decorationChanged)
    Q_PROPERTY(QString description READ description NOTIFY descriptionChanged)

public:
    DialogItemPrivate();
    ~DialogItemPrivate();
    QString displayName() const;
    QStringList decoration() const;
    QString description() const;
    bool isGroupChat() const;
    ProfileItem profile() const;
    void setProfile(const ProfileItem profile);
    MessageItem message() const;
    void setMessage(const MessageItem message);
    GroupChatHandler* groupChatHandler() const;
    void setGroupChatHandler(GroupChatHandler *groupChatHandler);
    TypingHandler* typingHandler() const;
    bool isTyping() const;
    int unreadCount() const;
    void setUnreadCount(const int unreadCount);
    void incUnreadDialogs();
    void decUnreadDialogs();
    bool isCurrent() const;
    void setCurrent(const bool current);

private:
    ProfileItem _profile;
    MessageItem _message;
    GroupChatHandler *_groupChatHandler;
    TypingHandler *_typingHandler;
    int _unreadCount;
    bool _isCurrent;

public slots:
    void getMessage(Connection *connection);
    void typing(const int uid);

protected slots:
    void onProfilePropertyChanged(const int uid, const QString &propertyName);
    void onMessagePropertyChanged(const int mid, const QString &propertyName);
    void onGroupChatPropertyChanged(const int chatId, const QString &propertyName);
    void onTypingActiveChanged(const bool isActive);
    void onGetMessageFinished(const HistoryPacket *sender, const int id, const MessageList &messages);

signals:
    void displayNameChanged();
    void decorationChanged();
    void descriptionChanged();
    void newTyping(const TypingItem typing);
};

typedef QSharedPointer<DialogItemPrivate> DialogItem;

#endif // DIALOGITEM_H
