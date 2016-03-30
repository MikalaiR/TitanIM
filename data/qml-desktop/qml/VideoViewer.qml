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

pragma Singleton

import QtQuick 2.0

Item {
    id: videoViewer
    parent: main.contentItem

    property var __item

    function setItem(item) {
        window.visible = false

        img.source = ""
        __item = item
        img.source = __item.url

        window.visible = true
        window.requestActivate()
    }

    ToolWindow {
        id: window
        width: Math.max(400, img.implicitWidth)
        height: Math.max(250, img.implicitHeight)
        minimumWidth: 200
        minimumHeight: 200
        modality: Qt.NonModal
        flags: Qt.Dialog
        color: "black"
        title: __item.title

        Item {
            id: content
            focus: true
            anchors.fill: parent

            AnimatedImage {
                id: img
                anchors.fill: parent
                fillMode: AnimatedImage.PreserveAspectFit
            }

            Keys.onPressed: {
                if (event.key === Qt.Key_Escape) {
                    window.close()
                }

                event.accepted = true;
            }
        }

        SpinnerItem {
            id: busyIndicator
            width: window.width
            height: window.height
            color: "white"
            running: img.status !== AnimatedImage.Ready
            progress: img.progress
        }

        onClicked: {
            if (img.status == AnimatedImage.Ready) {
                close()
            }
        }
    }
}
