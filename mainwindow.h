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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QQmlContext>
#include <QTranslator>
#include <QScreen>
#include <QDebug>
#include "qtquick2applicationviewer.h"
#include "cache/networkaccessmanagerfactory.h"
#include "vk/client.h"
#include "authorization.h"
#include "chat/dialogshandler.h"
#include "roster/rosterhandler.h"
#include "chat/chats.h"
#include "notification/notificator.h"

class MainWindow : public QtQuick2ApplicationViewer
{
    Q_OBJECT
    Q_PROPERTY(int fontPointSize READ fontPointSize CONSTANT)

public:
    explicit MainWindow(QWindow *parent = 0);
    ~MainWindow();
    void showExpanded();
    int fontPointSize() const;
    Q_INVOKABLE QPoint positionGlobalCenter(const int width, const int height) const;
    Q_INVOKABLE QPoint positionGlobalCursor() const;
    Q_INVOKABLE QPoint mapToGlobal(const int x, const int y) const;

private:
    QTranslator *translator;
    Authorization *authorization;
    DialogsHandler *dialogsHandler;
    RosterHandler *rosterHandler;

public slots:
    void showAuthPage();
    void showMainPage();
    void dialogCurrentIndexChanged(const int i);
    void rosterCurrentIndexChanged(const int i);
    void openChat(const int uid, const bool setCurrent=true);

private slots:
    void notificationClicked(const int peer, const int mid);
    void notificationReplied(const int peer, const int mid, const QString &response);
    void onConnected(const int uid, const QString &token, const QString &secret);
    void onDisconnected();
    void onError(const ErrorResponse::Error &error, const QString &text, const bool global, const bool fatal);
};

#endif // MAINWINDOW_H
