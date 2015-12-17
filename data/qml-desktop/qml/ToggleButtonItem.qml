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

Item {
    id: toggleswitch
    width: background.width; height: background.height

    property bool on: false
    property int __duration: 0

    signal clicked

    function toggle() {
        toggleswitch.on = !toggleswitch.on
        clicked()
    }

    function releaseSwitch() {
        if (knob.x == 0) {
            if (toggleswitch.state == "off") return;
        }
        if (knob.x == 13) {
            if (toggleswitch.state == "on") return;
        }

        toggle();
    }

    Image {
        id: background
        source: !toggleswitch.on ? "images/toggle_background.png" : "images/toggle_on_background.png"
        MouseArea { anchors.fill: parent; onClicked: toggle() }
    }

    Image {
        id: knob
        y: -1
        source: "images/toggle_item.png"

        MouseArea {
            anchors.fill: parent
            drag.target: knob; drag.axis: Drag.XAxis; drag.minimumX: 0; drag.maximumX: 13
            onClicked: toggle()
            onReleased: releaseSwitch()
        }
    }

    states: [
        State {
            name: "on"
            when: toggleswitch.on
            PropertyChanges { target: knob; x: 13 }
            PropertyChanges { target: toggleswitch }
        },
        State {
            name: "off"
            when: !toggleswitch.on
            PropertyChanges { target: knob; x: 0 }
            PropertyChanges { target: toggleswitch }
        }
    ]

    transitions: Transition {
        NumberAnimation { properties: "x"; easing.type: Easing.InOutQuad; duration: __duration }
    }

    Component.onCompleted: {
        __duration = 200
    }
}
