import QtQuick 2.0

Item {
    id: avatarItem

    property var source

    Component {
        id: avatarImage

        AvatarImage {
            source: url
        }
    }

    Row {
        width: parent.width
        height: parent.height
        spacing: 2

        Column {
            width: i2.active ? parent.width / 2 - 1 : parent.width
            height: parent.height
            spacing: 2

            AvatarImage {
                id: i1
                width: parent.width
                height: i3.active ? parent.height / 2 - 1 : parent.height
                source: avatarItem.source[0]
            }

            Loader {
                id: i3
                property string url: active ? avatarItem.source[2] : ""
                width: parent.width
                height: parent.height / 2 - 1
                active: avatarItem.source.length > 2
                sourceComponent: avatarImage
            }
        }

        Column {
            width: parent.width / 2 - 1
            height: parent.height
            spacing: 2

            Loader {
                id: i2
                property string url: active ? avatarItem.source[1] : ""
                width: parent.width
                height: i4.active ? parent.height / 2 - 1 : parent.height
                active: avatarItem.source.length > 1
                sourceComponent: avatarImage
            }

            Loader {
                id: i4
                property string url: active ? avatarItem.source[3] : ""
                width: parent.width
                height:  parent.height / 2 - 1
                active: avatarItem.source.length > 3
                sourceComponent: avatarImage
            }
        }
    }
}
