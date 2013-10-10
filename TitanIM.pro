# -------------------------------------------------
# Project created by rnazarov 2013-10-07T23:37:07
# -------------------------------------------------

QT += network

TARGET = titanim
TEMPLATE = app

qml.source = data/qml-desktop/qml
qml.target = share

DEPLOYMENTFOLDERS = qml

SOURCES += main.cpp \
    mainwindow.cpp \
    vk/client.cpp \
    vk/connection.cpp \
    vk/longpoll.cpp \
    vk/signup.cpp \
    vk/packet.cpp \
    vk/k8json.cpp \
    vk/errorresponse.cpp \
    vk/utils.cpp

HEADERS += \
    mainwindow.h \
    vk/client.h \
    vk/connection.h \
    vk/longpoll.h \
    vk/signup.h \
    vk/packet.h \
    vk/global.h \
    vk/k8json.h \
    vk/errorresponse.h \
    vk/utils.h

RESOURCES += \
    data/qml-desktop/qml/qml.qrc

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

# If your application uses the Qt Mobility libraries, uncomment the following
# lines and add the respective components to the MOBILITY variable.
# CONFIG += mobility
# MOBILITY +=

# Installation path
# target.path =

# Please do not modify the following two lines. Required for deployment.
include(qtquick2applicationviewer/qtquick2applicationviewer.pri)
qtcAddDeployment()
