# -------------------------------------------------
# Project created by rnazarov 2013-10-07T23:37:07
# -------------------------------------------------

QT += network \
      multimedia

TARGET = titanim
TEMPLATE = app
TRANSLATIONS = translations/titanim_ru.ts \
               translations/titanim_uk.ts \
               translations/titanim_be.ts

qml.source = data/qml-desktop/qml
qml.target = data
translations.source = data/translations
translations.target = data
sounds.source = data/sounds
sounds.target = data
smilies.source = data/smilies
smilies.target = data

DEPLOYMENTFOLDERS = translations \
                    sounds \
                    smilies

CONFIG(debug, debug|release):DEPLOYMENTFOLDERS += qml
CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT

SOURCES += main.cpp \
    mainwindow.cpp \
    vk/client.cpp \
    vk/connection.cpp \
    vk/longpoll.cpp \
    vk/signup.cpp \
    vk/packet.cpp \
    vk/errorresponse.cpp \
    vk/utils.cpp \
    vk/profileitem.cpp \
    vk/profileparser.cpp \
    vk/messageitem.cpp \
    vk/messageparser.cpp \
    vk/profilelist.cpp \
    vk/messagelist.cpp \
    vk/dialogspacket.cpp \
    chat/dialogsmodel.cpp \
    roster/rostermodel.cpp \
    vk/rosterpacket.cpp \
    vk/groupchathandler.cpp \
    chat/chatmodel.cpp \
    chat/chats.cpp \
    vk/historypacket.cpp \
    chat/chat.cpp \
    vk/dialogitem.cpp \
    vk/dialoglist.cpp \
    vk/dialogparser.cpp \
    chat/chatshandler.cpp \
    vk/sendmessagehandler.cpp \
    chat/dialogshandler.cpp \
    vk/observablecollection.cpp \
    vk/attachmentitem.cpp \
    vk/attachmentlist.cpp \
    vk/photoitem.cpp \
    vk/notifypropertybase.cpp \
    vk/attachmentsparser.cpp \
    vk/photoparser.cpp \
    settings.cpp \
    authorization.cpp \
    vk/uploadattachments.cpp \
    vk/uploadfile.cpp \
    roster/rosterhandler.cpp \
    vk/messagebase.cpp \
    vk/typingitem.cpp \
    vk/typinghandler.cpp \
    chat/chatsortfilterproxymodel.cpp \
    vk/emoticons.cpp

HEADERS += \
    mainwindow.h \
    vk/client.h \
    vk/connection.h \
    vk/longpoll.h \
    vk/signup.h \
    vk/packet.h \
    vk/global.h \
    vk/errorresponse.h \
    vk/utils.h \
    vk/profileitem.h \
    vk/profileparser.h \
    vk/messageitem.h \
    vk/messageparser.h \
    vk/profilelist.h \
    vk/messagelist.h \
    vk/dialogspacket.h \
    chat/dialogsmodel.h \
    roster/rostermodel.h \
    vk/rosterpacket.h \
    vk/groupchathandler.h \
    chat/chatmodel.h \
    chat/chats.h \
    vk/historypacket.h \
    chat/chat.h \
    vk/dialogitem.h \
    vk/dialoglist.h \
    vk/dialogparser.h \
    chat/chatshandler.h \
    vk/sendmessagehandler.h \
    chat/dialogshandler.h \
    vk/observablecollection.h \
    vk/attachmentitem.h \
    vk/attachmentlist.h \
    vk/photoitem.h \
    vk/notifypropertybase.h \
    vk/attachmentsparser.h \
    vk/photoparser.h \
    settings.h \
    authorization.h \
    vk/uploadattachments.h \
    vk/uploadfile.h \
    roster/rosterhandler.h \
    vk/messagebase.h \
    vk/typingitem.h \
    vk/typinghandler.h \
    chat/chatsortfilterproxymodel.h \
    vk/emoticons.h

RESOURCES += \
    data/qml-desktop/qml/qml.qrc \
    images/images.qrc

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
