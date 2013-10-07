#include "mainwindow.h"

MainWindow::MainWindow(QWindow *parent) :
    QtQuick2ApplicationViewer(parent)
{
    rootContext()->setContextProperty("main", this);

    setSource(QUrl("qrc:/qml/main.qml"));

    showExpanded();
}
