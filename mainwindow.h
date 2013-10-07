#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QQmlContext>
#include "qtquick2applicationviewer.h"

class MainWindow : public QtQuick2ApplicationViewer
{
    Q_OBJECT

public:
    explicit MainWindow(QWindow *parent = 0);

signals:

public slots:

};

#endif // MAINWINDOW_H
