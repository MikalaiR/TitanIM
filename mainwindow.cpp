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

#include "mainwindow.h"

MainWindow::MainWindow(QWindow *parent) :
    QtQuick2ApplicationViewer(parent)
{
    connect(Client::instance()->connection(), SIGNAL(connected(int,QString,QString)), this, SLOT(onConnected(int,QString,QString)));
    connect(Client::instance()->connection(), SIGNAL(disconnected()), this, SLOT(onDisconnected()));
//    connect(Client::instance()->connection(), SIGNAL(error(Error,QString,bool,bool)), this, SLOT(onError(Error,QString,bool,bool)));

    dialogsHandler = new DialogsHandler();
    rosterModel = new RosterModel(this);

    rootContext()->setContextProperty("main", this);
    rootContext()->setContextProperty("dialogsModel", dialogsHandler->proxy());
    rootContext()->setContextProperty("rosterModel", rosterModel);
    rootContext()->setContextProperty("chats", Chats::instance());

    setTitle("TitanIM");
    setSource(QUrl("qrc:/qml/main.qml"));
    showExpanded();

    Client::instance()->connection()->connectToVk("", "");
}

MainWindow::~MainWindow()
{
    delete rosterModel;
    delete dialogsHandler;
    Chats::instance()->destroy();
    Client::instance()->destroy();
}

void MainWindow::dialogCurrentIndexChanged(const int i)
{
    DialogItem dialog = dialogsHandler->dialog(i);
    Chats::instance()->openChat(dialog);
}

void MainWindow::rosterCurrentIndexChanged(const int i)
{
    DialogItem dialog = DialogItem::create();
    ProfileItem profile = rosterModel->at(i);
    dialog->setProfile(profile);
//    dialogsHandler->model()->append(dialog); //todo
    Chats::instance()->openChat(dialog);
}

void MainWindow::onConnected(const int uid, const QString &token, const QString &secret)
{
    qDebug() << "connecting" << " " << token << " " << secret;

    dialogsHandler->model()->load();
    rosterModel->load();
}

void MainWindow::onDisconnected()
{
    qDebug() << "disconnecting";
}

//void MainWindow::onError(const QQuickView::Error &error, const QString &text, const bool global, const bool fatal)
//{
//    qDebug() << error << text;
//}
