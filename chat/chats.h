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

#ifndef CHATS_H
#define CHATS_H

#include <QObject>
#include <QQmlComponent>
#include "chatsortfilterproxymodel.h"
#include "chatshandler.h"
#include "attachmentsmodel.h"
#include "vk/dialogitem.h"

class Chats : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int currentChatId READ currentChatId NOTIFY currentChatChanged)
    Q_PROPERTY(Chat* currentChat READ currentChat NOTIFY currentChatChanged)
    Q_PROPERTY(DialogItemPrivate* currentChatDialog READ currentChatDialog NOTIFY currentChatChanged)
    Q_PROPERTY(ChatSortFilterProxyModel* currentChatModel READ currentChatModel CONSTANT)
    Q_PROPERTY(AttachmentsModel* currentChatAttachments READ currentChatAttachments NOTIFY currentChatChanged)
    Q_PROPERTY(bool isForward READ isForward WRITE markAsForward NOTIFY isForwardChanged)
    Q_PROPERTY(bool isSelectUser READ isSelectUser WRITE markAsSelectUser NOTIFY isSelectUserChanged)

public:
    static Chats *instance();
    static void destroy();
    int currentChatId() const;
    Chat* currentChat() const;
    DialogItemPrivate* currentChatDialog() const;
    ChatSortFilterProxyModel* currentChatModel() const;
    AttachmentsModel* currentChatAttachments() const;
    Chat* chat(const int id) const;
    void clear();
    bool isForward() const;
    void markAsForward(const bool isMark);
    bool isSelectUser() const;
    void markAsSelectUser(const bool isMark);

private:
    explicit Chats();
    ~Chats();

private:
    static Chats *aInstance;
    ChatsHandler *_chatsHandler;
    int _currentChatId;
    ChatSortFilterProxyModel *_proxy;
    DialogItemPrivate *_currentDialog;
    AttachmentsModel *_currentChatAttachments;
    bool _markAsForward;
    bool _isSelectUser;
    QTimer *_timerUpdater;

protected:
    void setCurrentChat(const int id);

public slots:
    void openChat(const DialogItem dialog, const bool setCurrent=true);
    bool isMuteUser(const int id) const;
    void setMuteUser(const int id, const bool isMute);

protected slots:
    void onTimerUpdaterTimeout();

signals:
    void currentChatChanged(const int id);
    void muteUserChanged(const int id, const bool isMute);
    void isForwardChanged(const bool isForward);
    void isSelectUserChanged(const bool isSelectUser);
};

#endif // CHATS_H
