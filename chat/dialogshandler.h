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
#include "settings.h"

class DialogsHandler : public QObject
{
    Q_OBJECT

    Q_PROPERTY(DialogsModel* model READ model CONSTANT)
    Q_PROPERTY(QSortFilterProxyModel* proxy READ proxy CONSTANT)
    Q_PROPERTY(int unreadDialogs READ unreadDialogs NOTIFY unreadDialogsChanged)

public:
    DialogsHandler();
    ~DialogsHandler();
    DialogsModel* model() const;
    QSortFilterProxyModel* proxy() const;
    int unreadDialogs() const;
    DialogItem dialog(const int index, const bool isProxyIndex=true) const;
    Q_INVOKABLE int indexOf(const int id, const bool proxyIndex=true) const;

private:
    DialogsModel *_model;
    QSortFilterProxyModel *_proxy;
    int _unreadDialogs;

protected:
    void incUnreadDialogs();
    void decUnreadDialogs();

protected slots:
    void setUnreadDialogs(const int unreadDialogs);
    void onLongPollMessageInAdded(const DialogItem dialog);
    void onLongPollMessageOutAdded(const DialogItem dialog);
    void onUserStatusChanged(const int uid, const bool online);
    void onInMessagesRead(const int id, const int mid);
    void onMessageFlagsSet(const int mid, const int mask, const int id);
    void onMessageFlagsReseted(const int mid, const int mask, const int id, const uint date);
    void onRecoveryDialog(const DialogsPacket *sender, const DialogList &dialogs);

signals:
    void unreadDialogsChanged(const int unreadDialogs);
};

#endif // DIALOGSHANDLER_H
