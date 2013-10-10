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
#include "qtquick2applicationviewer.h"
#include "vk/client.h"

class MainWindow : public QtQuick2ApplicationViewer
{
    Q_OBJECT

public:
    explicit MainWindow(QWindow *parent = 0);
};

#endif // MAINWINDOW_H
