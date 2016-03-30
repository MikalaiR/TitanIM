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
import QtQuick.Controls 1.2

NotificationBox {
    id: dialogComponent

    property string sid: ""
    property string img: ""
    property string code: ""

    widthDialog: 274
    heightDialog: 202
    title: qsTr("Enter the code from the picture")
    ok: qsTr("Send")

    component: Column {
        height: 95
        spacing: 9

        Image {
            id: image
            source: img

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    image.source = img + "&rand=" + Math.floor(Math.random() * 100)
                }
            }
        }

        TextField {
            id: textField
            width: image.width
            font.pointSize: main.fontPointSize - 1
            placeholderText: qsTr("Enter code here")

            Keys.onReturnPressed: {
                if (textField.text.length) {
                    dialogComponent.accepted()
                }
            }

            Component.onCompleted: {
                forceActiveFocus()
            }

            onTextChanged: {
                dialogComponent.code = text
            }
        }
    }

    onAccepted: {
        authorization.setCaptcha(sid, code)
    }

    onRejected: {
        authorization.cancelCaptcha()
    }
}
