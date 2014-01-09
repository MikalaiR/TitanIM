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
#include <QSortFilterProxyModel>
#include <QQmlComponent>
#include "chatshandler.h"
#include "vk/dialogitem.h"

class Chats : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int currentChatId READ currentChatId NOTIFY currentChatChanged)
    Q_PROPERTY(Chat* currentChat READ currentChat NOTIFY currentChatChanged)
    Q_PROPERTY(DialogItemPrivate* currentChatDialog READ currentChatDialog NOTIFY currentChatChanged)
    Q_PROPERTY(QSortFilterProxyModel* currentChatModel READ currentChatModel CONSTANT)

public:
    static Chats *instance();
    static void destroy();
    int currentChatId() const;
    Chat* currentChat() const;
    DialogItemPrivate* currentChatDialog() const;
    QSortFilterProxyModel* currentChatModel() const;

private:
    explicit Chats();
    ~Chats();

private:
    static Chats *aInstance;
    ChatsHandler *_chatsHandler;
    int _currentChatId;
    QSortFilterProxyModel *_proxy;
    DialogItemPrivate *_dialog;

protected:
    void setCurrentChat(const int id);

public slots:
    void openChat(const DialogItem dialog);

signals:
    void currentChatChanged(const int id);
};

#endif // CHATS_H
