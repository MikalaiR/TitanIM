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
import QtQuick.Window 2.1
import QtQuick.Controls 1.2

Item {
    id: notificationBox
    anchors.fill: parent

    property int widthDialog: 100
    property int heightDialog: 100
    property string title: ""
    property string ok: ""
    property string cancel: ""
    property Component component

    signal accepted
    signal rejected

    function close() {
        window.close()
        closeAnimation.running = true
    }

    PropertyAnimation {
        id: openAnimation
        target: notificationBox
        easing.type: Easing.InOutQuad
        property: "opacity"
        duration: 400
        from: 0
        to: 1
        running: true
    }

    PropertyAnimation {
        id: closeAnimation
        target: notificationBox
        easing.type: Easing.InOutQuad
        property: "opacity"
        duration: 400
        from: 1
        to: 0

        onRunningChanged: {
            if (!running) {
                notificationBox.destroy()
            }
        }
    }

    Rectangle {
        id: overlay
        anchors.fill: parent
        color: "black"
        opacity: 0.6

        MouseArea {
            acceptedButtons: Qt.AllButtons
            anchors.fill: parent
        }
    }

    Window {
        id: window
        width: notificationBox.widthDialog
        height: notificationBox.heightDialog
        modality: Qt.ApplicationModal
        flags: Qt.Widget
        visible: true

        Rectangle {
            id: dialogWindow
            anchors.fill: parent
            anchors.centerIn: parent
            color: "white"

            Rectangle {
                id: header
                height: 45
                width: parent.width
                anchors.top: parent.top
                color: "#5A7CA3"

                Text {
                    id: titleLabel
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 15
                    text: notificationBox.title
                    font.bold: true
                    color: "white"
                }
            }

            Loader {
                id: content
                anchors.top: header.bottom
                anchors.topMargin: 12
                anchors.horizontalCenter: dialogWindow.horizontalCenter
                sourceComponent: notificationBox.component
                active: true
            }

            Rectangle {
                id: footer
                height: 50
                width: parent.width
                anchors.bottom: parent.bottom
                color: "#EEF0F2"

                Row {
                    anchors.right: parent.right
                    anchors.rightMargin: 15
                    anchors.verticalCenter: parent.verticalCenter
                    spacing: 7

                    Button {
                        id: btnOk
                        width: implicitWidth + 10
                        text: notificationBox.ok
                        visible: notificationBox.ok.length

                        onClicked: {
                            notificationBox.accepted()
                        }
                    }

                    Button {
                        id: btnCancel
                        width: implicitWidth + 10
                        text: notificationBox.cancel
                        visible: notificationBox.cancel.length

                        onClicked: {
                            notificationBox.rejected()
                        }
                    }
                }
            }

            Keys.onEscapePressed: {
                notificationBox.rejected()
            }
        }
    }

    onAccepted: {
        close()
    }

    onRejected: {
        close()
    }
}
