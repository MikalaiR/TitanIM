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
#include <QDebug>
#include "qtquick2applicationviewer.h"
#include "vk/client.h"
#include "chat/dialogshandler.h"
#include "roster/rostermodel.h"
#include "chat/chats.h"
#include "settings.h"

class MainWindow : public QtQuick2ApplicationViewer
{
    Q_OBJECT

public:
    explicit MainWindow(QWindow *parent = 0);
    ~MainWindow();

private:
    DialogsHandler *dialogsHandler;
    RosterModel *rosterModel;

public slots:
    void dialogCurrentIndexChanged(const int i);
    void rosterCurrentIndexChanged(const int i);

private slots:
    void onConnected(const int uid, const QString &token, const QString &secret);
    void onDisconnected();
//    void onError(const Error &error, const QString &text, const bool global, const bool fatal);
};

#endif // MAINWINDOW_H
