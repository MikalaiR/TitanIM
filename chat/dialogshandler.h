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

#ifndef DIALOGSHANDLER_H
#define DIALOGSHANDLER_H

#include <QObject>
#include <QSortFilterProxyModel>
#include "dialogsmodel.h"
#include "vk/client.h"
#include "notification/notificator.h"
#include "chats.h"

class DialogsHandler : public QObject
{
    Q_OBJECT

    Q_PROPERTY(DialogsModel* model READ model CONSTANT)
    Q_PROPERTY(QSortFilterProxyModel* proxy READ proxy CONSTANT)
    Q_PROPERTY(int unreadDialogs READ unreadDialogs NOTIFY unreadDialogsChanged)
    Q_PROPERTY(QString typingText READ typingText NOTIFY typingTextChanged)

public:
    DialogsHandler();
    ~DialogsHandler();
    DialogsModel* model() const;
    QSortFilterProxyModel* proxy() const;
    int unreadDialogs() const;
    QString typingText() const;
    DialogItem dialog(const int index, const bool isProxyIndex=true) const;
    Q_INVOKABLE int indexOf(const int id, const bool proxyIndex=true) const;
    void clear();

private:
    DialogsModel *_model;
    QSortFilterProxyModel *_proxy;
    int _unreadDialogs;
    QString _typingText;
    bool _flagMarkAsRead;

protected:
    void incUnreadDialogs();
    void decUnreadDialogs();

public slots:
    void getCounterUnreadDialogs();

protected slots:
    void setUnreadDialogs(const int unreadDialogs);
    void onLongPollMessageInAdded(const int id, const MessageItem message, const ProfileItem profile);
    void onLongPollMessageOutAdded(const int id, const MessageItem message, const ProfileItem profile);
    void onLongPollChatTyping(const int id, const int uid, const int chatId);
    void onInMessagesRead(const int id, const int mid);
    void onOutMessagesRead(const int id, const int mid);
    void onMessageFlagsSet(const int mid, const int mask, const int id);
    void onMessageFlagsReseted(const int mid, const int mask, const int id, const uint date);
    void onGroupChatUpdated(const int chatId, const bool self);
    void onCounterUnreadDialogs(const Packet *sender, const QVariantMap &result);
    void timerEvent(QTimerEvent *event);

signals:
    void unreadDialogsChanged(const int unreadDialogs);
    void typingTextChanged(const QString &typingText);
    void newMessage(const bool isOut, const int id);
};

#endif // DIALOGSHANDLER_H
