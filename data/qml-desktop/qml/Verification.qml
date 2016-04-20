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
    id: verificationWindow
    readonly property string name: "verificationWindow"
    property string sid: ""
    property string phone: ""
    property bool isSms: false
    property string uri: ""

    function verificationCode() {
        if (code.text.length == 0){
            code.forceActiveFocus()
            return
        }

        verificationWindow.focus = true
        nextLabel.enabled = false
        code.enabled = false
        authorization.setVerificationCode(verificationWindow.sid, code.text)
    }

    function reset() {
        code.text = ""
        nextLabel.enabled = true
        code.enabled = true
        code.forceActiveFocus()
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

            Text {
                height: 30
                width: rowCode.width
                anchors.right: parent.right
                text: isSms ? qsTr("Enter the confirmation code we've sent to you via SMS")
                            : qsTr("Enter the code provided by the code generating app")
                verticalAlignment: Text.AlignTop
                font.pointSize: main.fontPointSize - 1
                color: "grey"
            }

            Row {
                id: rowCode
                anchors.right: parent.right
                spacing: 10

                Text {
                    anchors.verticalCenter: code.verticalCenter
                    text: isSms ? qsTr("sms") : qsTr("code")
                    font.pointSize: main.fontPointSize - 1
                    color: "grey"
                }

                TextField {
                    id: code
                    width: 250
                    placeholderText: qsTr("enter your code")
                    inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhNoPredictiveText
                    font.pointSize: main.fontPointSize
                    style: TextFieldStyle {
                            textColor: control.enabled ? "black" : "grey"
                            background: Item {}
                    }

                    Keys.onReturnPressed: {
                        verificationCode()
                    }
                }
            }

            Rectangle {
                height: 1
                width: code.width
                anchors.right: parent.right
                color: "#e8e8e8"
            }

            Text {
                id: problemLabel
                width: rowCode.width
                anchors.right: parent.right
                text: qsTr("Any trouble receiving codes?")
                horizontalAlignment: Text.AlignRight
                font.pointSize: main.fontPointSize - 2
                color: "#2C83D6"

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        authorization.showValidation(uri)
                    }
                }
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
                    verificationCode()
                }
            }
        }
    }

    Connections {
        target: authorization

        onIncorrectCode: {
            reset()
        }

        onCaptcha: {
            Qt.createComponent("Captcha.qml").createObject(verificationWindow, {sid: captchaSid, img: captchaImg})
        }
    }

    Component.onCompleted: {
        code.forceActiveFocus()
    }
}
