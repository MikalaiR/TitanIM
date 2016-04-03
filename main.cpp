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

#include <QtGui/QGuiApplication>
#include <QTextCodec>
#include <QtWebEngine>
#include <QFont>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);

    QtWebEngine::initialize();

    QCoreApplication::setApplicationName("TitanIM");
    QCoreApplication::setApplicationVersion("2.0.3");
    QCoreApplication::setOrganizationName("Ruslan Nazarov");
    QCoreApplication::setOrganizationDomain("http://titanim.ru");

    QFont defaultFont;
    defaultFont.setFamily("Geneva");//todo win lin
    app.setFont(defaultFont);

    MainWindow w;
    return app.exec();
}
