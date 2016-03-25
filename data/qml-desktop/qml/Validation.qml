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
import QtWebKit 3.0

Rectangle {
    id: validationWindow
    readonly property string name: "validationWindow"
    property alias uri: webView.url

    WebView {
        id: webView
        z: 1
        anchors.fill: parent
    }

    Text {
        z: 0
        anchors.centerIn: parent
        font.pointSize: main.fontPointSize
        color: "grey"
        text: qsTr("Loading...")
        wrapMode: Text.Wrap
    }

    onUriChanged: {
        authorization.checkUrlValidation(uri)
    }
}
