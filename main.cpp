#include <QtGui/QGuiApplication>
#include <QTextCodec>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);

    QCoreApplication::setApplicationName("TitanIM");
    QCoreApplication::setApplicationVersion("2.0.0");
    QCoreApplication::setOrganizationName("Ruslan Nazarov");
    QCoreApplication::setOrganizationDomain("http://titanim.ru");

    MainWindow w;
    return app.exec();
}
