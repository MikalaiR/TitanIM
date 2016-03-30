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

StackView {
    id: mainStackView
    width: 300
    height: 300
    initialItem: Qt.resolvedUrl(main.initialItem)

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

    Connections {
        target: authorization

        onShowAuthPage: {
                mainStackView.push({item: Qt.resolvedUrl("Authorization.qml"), replace: true})
        }

        onShowMainPage: {
            if (mainStackView.currentItem.name !== "mainWindow")
            {
                mainStackView.push({item: Qt.resolvedUrl("MainWindow.qml"), replace: true})
            }
        }

        onValidation: {
            mainStackView.push({item: Qt.resolvedUrl("Validation.qml"), replace: true, properties: {uri: validationUri}})
        }

        onVerification: {
            mainStackView.push({item: Qt.resolvedUrl("Verification.qml"), replace: true,
                                   properties: {sid: validationSid, phone: phoneMask, isSms: sms, uri: validationUri}})
        }
    }
}
