import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0

Rectangle {
    height: 520;
    width: 720;

    SplitView {
        anchors.fill: parent
        orientation: Qt.Horizontal

        TabBarView {
            id: tabBar
            width: 280
            height: parent.height
            Layout.minimumWidth: 200
            Layout.maximumWidth: 500

            TabBarItem {
                title: "1"
                icon: "images/messages_tab.png"
                Dialogs { }
            }

            TabBarItem {
                title: "2"
                icon: "images/contacts_tab.png"
                Text{text: "2"}
            }

            TabBarItem {
                title: "3"
                icon: "images/favorites_tab.png"
                Text{text: "3"}
            }

            TabBarItem {
                title: "4"
                icon: "images/settings_tab.png"
                Text{text: "4"}
            }
        }

        Rectangle {
            Layout.fillWidth: true
            color: "darkgray"
        }
    }
}
