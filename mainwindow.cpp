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

    setDefaultFont();
    engine()->setNetworkAccessManagerFactory(new NetworkAccessManagerFactory);

    connect(Notificator::instance(), &Notificator::notificationClicked, this, &MainWindow::notificationClicked);
    connect(Notificator::instance(), &Notificator::notificationReplied, this, &MainWindow::notificationReplied);

    QString emoticonsTheme = Settings::instance()->dataDir() + "/smilies/default/"; //todo
    QString emoticonsRecent = Settings::instance()->configDir() + "/recentEmojis";
    Emoticons::instance()->setCurrentTheme(emoticonsTheme);
    Emoticons::instance()->setRecentFile(emoticonsRecent);

    authorization = new Authorization();

    connect(authorization, &Authorization::showAuthPage, this, &MainWindow::showAuthPage);
    connect(authorization, &Authorization::showMainPage, this, &MainWindow::showMainPage);

    connect(this, &MainWindow::activeChanged, this, &MainWindow::onActiveChanged);

    connect(Client::instance()->connection(), &Connection::authorized, this, &MainWindow::onAuthorized);
    connect(Client::instance()->connection(), &Connection::logout, this, &MainWindow::onLogout);
    connect(Client::instance()->connection(), &Connection::error, this, &MainWindow::onError);

    connect(Client::instance()->longPoll(), &LongPoll::started, this, &MainWindow::onLongPollStarted);
    connect(Client::instance()->longPoll(), &LongPoll::stopped, this, &MainWindow::onLongPollStopped);
    connect(Client::instance()->longPoll(), &LongPoll::rebuild, this, &MainWindow::onRebuild);

    dialogsHandler = new DialogsHandler();
    rosterHandler = new RosterHandler();

    rootContext()->setContextProperty("authorization", authorization);
    rootContext()->setContextProperty("main", this);
    rootContext()->setContextProperty("engine", Client::instance()->engine());
    rootContext()->setContextProperty("settings", Settings::instance());
    rootContext()->setContextProperty("dialogsHandler", dialogsHandler);
    rootContext()->setContextProperty("rosterHandler", rosterHandler);
    rootContext()->setContextProperty("chats", Chats::instance());
    rootContext()->setContextProperty("emoticons", Emoticons::instance());
    rootContext()->setContextProperty("audio", Audio::instance());

    qmlRegisterSingletonType(QUrl("qrc:/qml/AudioPlayerObject.qml"), "TitanIM.Multimedia", 1, 0, "AudioPlayer");
    qmlRegisterSingletonType(QUrl("qrc:/qml/PhotoViewer.qml"), "TitanIM.Viewer", 1, 0, "PhotoViewer");
    qmlRegisterSingletonType(QUrl("qrc:/qml/VideoViewer.qml"), "TitanIM.Viewer", 1, 0, "VideoViewer");
    qmlRegisterSingletonType(QUrl("qrc:/qml/EmoticonsBox.qml"), "TitanIM.Tool", 1, 0, "EmoticonsBox");

    setTitle(QCoreApplication::applicationName());
    setWidth(770);
    setHeight(560);
    authorization->connectToVk();
}

MainWindow::~MainWindow()
{
    delete rosterHandler;
    delete dialogsHandler;
    delete authorization;
    Client::instance()->destroy();
    Chats::instance()->destroy();
    Emoticons::instance()->destroy();
    Notificator::instance()->destroy();
    Settings::instance()->destroy();
}

void MainWindow::showAuthPage()
{
    if (_initialItem.isEmpty())
    {
        _initialItem = "Authorization.qml";
        setSource(QUrl("qrc:/qml/main.qml"));
        showExpanded();
    }
}

void MainWindow::showMainPage()
{
    if (_initialItem.isEmpty())
    {
        _initialItem = "MainWindow.qml";
        setSource(QUrl("qrc:/qml/main.qml"));
        showExpanded();
    }
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

void MainWindow::openChat(const int uid, const bool setCurrent)
{
    int index = dialogsHandler->indexOf(uid, false);

    if (index > -1)
    {
        DialogItem dialog = dialogsHandler->dialog(index, false);
        Chats::instance()->openChat(dialog, setCurrent);
    }
    else
    {
        DialogItem dialog = DialogItem::create();
        dialog->setId(uid);
        dialog->setProfile(Client::instance()->engine()->getProfile(uid));
        dialog->createStructure();
        dialog->getAllFields(Client::instance()->connection(), true);
//        dialogsHandler->model()->append(dialog); //todo
        Chats::instance()->openChat(dialog, setCurrent);
    }
}

void MainWindow::showExpanded()
{
    QtQuick2ApplicationViewer::showExpanded();
    setPosition(positionGlobalCenter(width(), height()));
}

QPoint MainWindow::positionGlobalCenter(const int width, const int height) const
{
    QScreen *screen = this->screen();
    int x = (screen->virtualGeometry().width() / 2) - (width / 2);
    int y = (screen->virtualGeometry().height() / 2) - (height / 2);

    return QPoint(x, y);
}

QPoint MainWindow::positionGlobalCursor() const
{
    return QCursor::pos();
}

QPoint MainWindow::mapToGlobal(const int x, const int y) const
{
    return QWindow::mapToGlobal(QPoint(x, y));
}

void MainWindow::setDefaultFont()
{
    QFont defaultFont;

#if defined(Q_OS_MAC)
    defaultFont.setFamily("Geneva");
    _fontPointSize = 13;
#elif defined(Q_OS_WIN)
    //todo win
    _fontPointSize = 13;
#else
    int id = QFontDatabase::addApplicationFont(":/qml/fonts/Ubuntu-L.ttf");

    if (id != -1)
    {
        QString family = QFontDatabase::applicationFontFamilies(id).first();
        defaultFont.setFamily(family);
        defaultFont.setWeight(QFont::Light);
    }

    _fontPointSize = 11;
#endif

    qApp->setFont(defaultFont);
}

int MainWindow::fontPointSize() const
{
    return _fontPointSize;
}

void MainWindow::setFontPointSize(const int fontPointSize)
{
    _fontPointSize = fontPointSize;
    emit fontPointSizeChanged();
}

QString MainWindow::initialItem() const
{
    return _initialItem;
}

void MainWindow::notificationClicked(const int peer, const int mid)
{
    openChat(peer);
    show();
}

void MainWindow::notificationReplied(const int peer, const int mid, const QString &response)
{
    openChat(peer, false);
    Chats::instance()->chat(peer)->markAsRead();
    Chats::instance()->chat(peer)->sendMessage(response);
}

void MainWindow::onActiveChanged()
{
    if (isActive() && Chats::instance()->currentChatId() != 0 && Chats::instance()->currentChat()->countUnread() > 0
            && Settings::instance()->loadProfile("chat/autoRead", true).toBool())
    {
        Chats::instance()->currentChat()->markAsRead();
    }
}

void MainWindow::onAuthorized(const int uid, const QString &token, const QString &secret)
{
    dialogsHandler->model()->load();
    rosterHandler->model()->load();
}

void MainWindow::onLogout(const int uid)
{
    setTitle(QCoreApplication::applicationName());

    dialogsHandler->clear();
    rosterHandler->clear();
    Chats::instance()->clear();
    Audio::instance()->clear();
}

void MainWindow::onError(const ErrorResponse::Error &error, const QString &text, const bool global, const bool fatal)
{
    qDebug() << "ERROR:" << error << text; //todo
}

void MainWindow::onLongPollStarted()
{
    setTitle(QCoreApplication::applicationName());
}

void MainWindow::onLongPollStopped()
{
    if (Client::instance()->connection()->isAuthorized())
    {
        setTitle(QCoreApplication::applicationName() + " - " + tr("Connecting..."));
    }
}

void MainWindow::onRebuild()
{
    dialogsHandler->model()->load();
    rosterHandler->model()->load();
}
