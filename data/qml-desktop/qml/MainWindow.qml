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

import QtQuick 2.1
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.0

Rectangle {
    id: mainWindow
    readonly property string name: "mainWindow"
    property bool visiblePageStackView: false
    property int widthTabBar: 280

    function popPage(page) {
        pageStackView.pop(page)
    }

    function pushPage(page, name) {
        if (pageStackView.currentItem.name !== name)
        {
            pageStackView.push(page)
        }
    }

    function __showPageStackView() {
        tabBar.Layout.minimumWidth = 200
        tabBar.Layout.maximumWidth = 500
        splitView.addItem(pageStackView)
        tabBar.width = widthTabBar
        pageStackView.visible = true
    }

    function __hidePageStackView() {
        tabBar.Layout.minimumWidth = -1
        tabBar.Layout.maximumWidth = -1
        splitView.removeItem(pageStackView)
        pageStackView.visible = false
    }

    SplitView {
        id: splitView
        anchors.fill: parent
        orientation: Qt.Horizontal

        TabBarView {
            id: tabBar
            height: parent.height

            TabBarItem {
                id: dialogsTab
                title: "dialogs"
                icon: "images/messages_tab.png"
                badge: dialogsHandler.unreadDialogs;
                Dialogs {id: dialogs}

                onClicked: {
                    popPage({item: Qt.resolvedUrl("Chat.qml"), immediate: true})

                    if (dialogsTab.visible || badge > 0)
                        dialogs.scrollToTop()
                }
            }

            TabBarItem {
                id: contactsTab
                title: "contacts"
                icon: "images/contacts_tab.png"
                Roster {id: roster}

                onClicked: {
                    if (chats.currentChatId !== 0 && !chats.isForward) {
                        pushPage({item: Qt.resolvedUrl("Profile.qml"), immediate: true}, "profile")
                    }

                    if (contactsTab.visible)
                        roster.scrollToTop()

                    roster.clearFilter()
                    roster.forceActiveFocusFilter()
                }
            }

            TabBarItem {
                id: favoritesTab
                title: "favorites"
                icon: "images/favorites_tab.png"
                Text {text: "favorites"}
            }

            TabBarItem {
                id: settingsTab
                title: "settings"
                icon: "images/settings_tab.png"
                Settings {id: settings}

                onClicked: {
                    engine.getSelfProfile()
                }
            }

            onWidthChanged: { //qml bug
                if (width < 200) {
                    splitView.enabled = false
                    splitView.enabled = true
                }
            }

            onCurrentIndexChanged: {
                if (currentIndex == 3)
                    visiblePageStackView = false
                else
                    visiblePageStackView = true
            }
        }

        Component.onCompleted: {
            visiblePageStackView = true
        }
    }

    StackView {
        id: pageStackView
        Layout.fillWidth: true
        visible: false
        initialItem: Qt.resolvedUrl("Chat.qml")
        clip: true

        delegate: StackViewDelegate {
            function transitionFinished(properties)
            {
                properties.exitItem.opacity = 1
            }

            pushTransition: StackViewTransition {
                PropertyAnimation {
                    target: enterItem
                    property: "opacity"
                    from: 0
                    to: 1
                }
                PropertyAnimation {
                    target: exitItem
                    property: "opacity"
                    from: 1
                    to: 0
                }
            }
        }
    }

    Connections {
        target: authorization

        onCaptcha: {
            Qt.createComponent("Captcha.qml").createObject(mainWindow, {sid: captchaSid, img: captchaImg});
        }
    }

    onVisiblePageStackViewChanged: {
        if (visiblePageStackView)
            __showPageStackView()
        else
            __hidePageStackView()
    }
}