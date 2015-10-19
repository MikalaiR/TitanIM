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
import TitanIM 2.0

Item {
    id: photoViewer
    parent: main.contentItem

    property int currentIndex: 0
    property int count: 0
    property var __items
    property int __index: 0

    function setItems(items, index) {
        window.visible = false

        __items = items.slice()

        for (var i = 0; i < __items.length; i++) {
             if (__items[i].type !== Attachment.Photo) {
                 __items.splice(i, 1)

                 if (i < index) {
                     index--;
                 }

                 i--;
             }
        }

        setIndex(index)
        photoViewer.count = __items.length

        window.visible = true
        window.requestActivate()
    }

    function setUrl(url) {
        var item = {type: Attachment.Photo, srcBig: url}
        var items = [item]

        setItems(items, 0)
    }

    function setIndex(index) {
        __getNext(index)
        next()
    }

    function nextIndex() {
        var index = __index + 1

        if (index >= __items.length) {
            index = 0
        }

        return index
    }

    function prevIndex() {
        var index = currentIndex - 1

        if (index < 0) {
            index = __items.length - 1
        }

        return index
    }

    function next() {
        img1.visible = !img1.visible

        if (currentIndex === __index) {
            window.hide()
        }

        currentIndex = __index

        __getNext(nextIndex())
    }

    function prev() {
        setIndex(prevIndex())
    }

    function __getNext(index) {
        __index = index

        if (img1.visible) {
            img2.source = __items[__index].srcBig
        } else {
            img1.source = __items[__index].srcBig
        }
    }

    ToolWindow {
        id: window
        width: img.width
        height: img.height
        maximumWidth: img.width
        maximumHeight: img.height
        minimumWidth: img.width
        minimumHeight: img.height
        modality: Qt.NonModal
        flags: Qt.Dialog
        visible: true
        color: "#DFE3EA"
        title: qsTr("Photo %1 of %2").arg(currentIndex + 1).arg(count)

        Item {
            id: img
            focus: true
            width: img1.visible ? img1.implicitWidth : img2.implicitWidth
            height: img1.visible ? img1.implicitHeight : img2.implicitHeight

            Image {
                id: img1
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
                visible: false
            }

            Image {
                id: img2
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
                visible: !img1.visible
            }

            Keys.onPressed: {
                if (event.key === Qt.Key_Escape) {
                    window.hide()
                } else if (event.key === Qt.Key_Right) {
                    next()
                } else if (event.key === Qt.Key_Left) {
                    prev()
                }

                event.accepted = true;
            }
        }

        TextShadow {
            width: parent.width
            z: -10
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: Text.AlignHCenter
            font.pointSize: main.fontPointSize + 3
            font.bold: true
            font.family: "Helvetica"
            color: "#8B92A4"
            text: qsTr("Loading...")
            wrapMode: Text.Wrap
        }

        onClicked: {
            next()
        }

        onClosing: {
            hide()
            close.accepted = false
        }
    }
}
