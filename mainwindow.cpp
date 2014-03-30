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
    translator = new QTranslator(this);
    translator->load("titanim_" + Settings::instance()->loadMain("main/language", QLocale::system().name()).toString(),
                     Settings::instance()->dataDir() + "/translations");
    qApp->installTranslator(translator);

    authorization = new Authorization();

    connect(authorization, SIGNAL(showAuthPage()), this, SLOT(showAuthPage()));
    connect(authorization, SIGNAL(showMainPage()), this, SLOT(showMainPage()));

    connect(Client::instance()->connection(), SIGNAL(connected(int,QString,QString)), this, SLOT(onConnected(int,QString,QString)));
    connect(Client::instance()->connection(), SIGNAL(disconnected()), this, SLOT(onDisconnected()));
//    connect(Client::instance()->connection(), SIGNAL(error(Error,QString,bool,bool)), this, SLOT(onError(Error,QString,bool,bool)));

    dialogsHandler = new DialogsHandler();
    rosterHandler = new RosterHandler();

    rootContext()->setContextProperty("authorization", authorization);
    rootContext()->setContextProperty("main", this);
    rootContext()->setContextProperty("dialogsHandler", dialogsHandler);
    rootContext()->setContextProperty("rosterHandler", rosterHandler);
    rootContext()->setContextProperty("chats", Chats::instance());

    setTitle("TitanIM");
    authorization->connectToVk();
}

MainWindow::~MainWindow()
{
    delete rosterHandler;
    delete dialogsHandler;
    delete authorization;
    Chats::instance()->destroy();
    Client::instance()->destroy();
    Settings::instance()->destroy();
}

void MainWindow::showAuthPage()
{
    setWidth(350);
    setHeight(160);
    setSource(QUrl("qrc:/qml/Authorization.qml"));
    showExpanded();
}

void MainWindow::showMainPage()
{
    setWidth(720);
    setHeight(520);
    setSource(QUrl("qrc:/qml/main.qml"));
    showExpanded();
}

void MainWindow::dialogCurrentIndexChanged(const int i)
{
    DialogItem dialog = dialogsHandler->dialog(i);
    Chats::instance()->openChat(dialog);
}

void MainWindow::rosterCurrentIndexChanged(const int i)
{
    ProfileItem profile = rosterHandler->profile(i);
    DialogItem dialog = DialogItem::create();
    dialog->setProfile(profile);
//    dialogsHandler->model()->append(dialog); //todo
    Chats::instance()->openChat(dialog);
}

void MainWindow::moveToCenter()
{
    QScreen *screen = this->screen();
    int x = (screen->virtualGeometry().width() / 2) - (this->width() / 2);
    int y = (screen->virtualGeometry().height() / 2) - (this->height() / 2);
    this->setPosition(x, y);
}

void MainWindow::showExpanded()
{
    QtQuick2ApplicationViewer::showExpanded();
    moveToCenter();
}

void MainWindow::onConnected(const int uid, const QString &token, const QString &secret)
{
    dialogsHandler->model()->load();
    rosterHandler->model()->load();
}

void MainWindow::onDisconnected()
{
}

//void MainWindow::onError(const QQuickView::Error &error, const QString &text, const bool global, const bool fatal)
//{
//    qDebug() << error << text;
//}
