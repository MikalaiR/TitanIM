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

import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.4

Rectangle {
    id: authWindow
    readonly property string name: "authWindow"

    function connectToVk() {
        if (login.text.length == 0){
            login.forceActiveFocus()
            return
        }

        if (pass.text.length == 0){
            pass.forceActiveFocus()
            return
        }

        authWindow.focus = true
        nextLabel.enabled = false
        login.enabled = false
        pass.enabled = false
        authorization.connectToVk(login.text, pass.text)
    }

    Rectangle {
        anchors.fill: parent
        color: "white"
    }

    Column {
        id: column
        width: parent.width - 100
        height: 300
        anchors.centerIn: parent
        anchors.verticalCenterOffset: -30
        spacing: 50

        LogoHeader {
            id: headerRow
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.horizontalCenterOffset: -30
        }

        Column {
            id: formCol
            anchors.left: headerRow.left
            spacing: 8

            Row {
                anchors.right: parent.right
                spacing: 10

                Text {
                    anchors.verticalCenter: login.verticalCenter
                    text: qsTr("login")
                    font.pointSize: main.fontPointSize - 1
                    color: "grey"
                }

                TextField {
                    id: login
                    width: 250
                    placeholderText: qsTr("phone or email")
                    inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhEmailCharactersOnly | Qt.ImhNoPredictiveText
                    font.pointSize: main.fontPointSize
                    style: TextFieldStyle {
                            textColor: control.enabled ? "black" : "grey"
                            background: Item {}
                    }

                    Keys.onReturnPressed: {
                        pass.forceActiveFocus()
                    }
                }
            }

            Rectangle {
                height: 1
                width: login.width
                anchors.right: parent.right
                color: "#e8e8e8"
            }

            Row {
                anchors.right: parent.right
                spacing: 10

                Text {
                    anchors.verticalCenter: pass.verticalCenter
                    text: qsTr("password")
                    font.pointSize: main.fontPointSize - 1
                    color: "grey"
                }

                TextField {
                    id: pass
                    width: 250
                    placeholderText: qsTr("password")
                    echoMode: TextInput.Password
                    inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhNoPredictiveText
                    font.pointSize: main.fontPointSize
                    style: TextFieldStyle {
                            textColor: control.enabled ? "black" : "grey"
                            background: Item {}
                    }

                    Keys.onReturnPressed: {
                        connectToVk()
                    }
                }
            }

            Rectangle {
                height: 1
                width: pass.width
                anchors.right: parent.right
                color: "#e8e8e8"
            }
        }

        Text {
            id:nextLabel
            anchors.right: formCol.right
            anchors.rightMargin: 250 - width - 10
            text: qsTr("Next")
            color: "#2C83D6"
            font.pointSize: main.fontPointSize

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    connectToVk()
                }
            }
        }
    }

    Connections {
        target: authorization

        onCaptcha: {
            Qt.createComponent("Captcha.qml").createObject(authWindow, {sid: captchaSid, img: captchaImg})
        }
    }

    Component.onCompleted: {
        login.forceActiveFocus()
    }
}
