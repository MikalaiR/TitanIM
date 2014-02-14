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
    id: photoAttachments

    property int maximumWidth
    property var photoList
    readonly property int count: photoList.length

    function __calcHeight(count, size, spacing, width) {
        var col = Math.floor((width + spacing) / (size + spacing))
        var row = Math.ceil(count / col)
        return ((size + spacing) * row) - spacing
    }

    function __calcItemSize(count) {
        switch (count) {
        case 1:
            return 250
        case 2:
            return 200
        case 3:
        case 4:
        case 5:
        case 6:
            return 120
        default:
            return 80
        }
    }

    implicitWidth: flowPhotos.implicitWidth
    implicitHeight: __calcHeight(photoList.length, flowPhotos.itemSize, flowPhotos.spacing, maximumWidth)

    Flow {
        id: flowPhotos

        property int itemSize: photoAttachments.maximumWidth < __calcItemSize(count) ? photoAttachments.maximumWidth
                                                                                     : __calcItemSize(count)

        width: photoAttachments.maximumWidth
        spacing: 4

        Repeater {
            model: photoList

            Image {
                id: img
                width: flowPhotos.itemSize
                height: flowPhotos.itemSize
                smooth: true
                fillMode: Image.PreserveAspectCrop
                source: flowPhotos.itemSize > 110 ? modelData.srcBig : modelData.src
            }
        }
    }

    onWidthChanged: {
        implicitHeight = __calcHeight(photoList.length, flowPhotos.itemSize, flowPhotos.spacing, maximumWidth)
    }
}
