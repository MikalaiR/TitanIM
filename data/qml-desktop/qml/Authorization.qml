import QtQuick 2.0
import QtQuick.Controls 1.0

Rectangle {
    Rectangle {
        anchors.fill: parent;
        color: "#4e729a"
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#42658c" }
            GradientStop { position: 1.0; color: "#5a7da5" }
        }
    }

    Column {
        id: column;
        width: parent.width - 40;
        anchors.centerIn: parent;
        spacing: 20;

        Image {
            id: logo;
            height: 35
            anchors.horizontalCenter: parent.horizontalCenter;
            source: "images/logo_full.png";
            fillMode: Image.PreserveAspectFit
        }

        Column {
            width: parent.width
            spacing: 3;

            TextField {
                id: login;
                width: parent.width;
                placeholderText: qsTr("Email");
                inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhEmailCharactersOnly | Qt.ImhNoPredictiveText;

                Keys.onReturnPressed: {
                    pass.forceActiveFocus();
                }
            }

            TextField {
                id: pass;
                width: parent.width;
                placeholderText: qsTr("Password");
                echoMode: TextInput.Password;
                inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhNoPredictiveText;

                Keys.onReturnPressed: {
                    if (login.text.length == 0){
                        login.forceActiveFocus();
                        return;
                    }

                    if (pass.text.length == 0){
                        pass.forceActiveFocus();
                        return;
                    }

                    parent.focus = true;
                    authorization.connectToVk(login.text, pass.text);
                }
            }

            Text {
                anchors.horizontalCenter: parent.horizontalCenter;
                color: "white";
                text: qsTr("Sign up for VKontakte");

                MouseArea {
                    anchors.fill: parent;
                    onClicked: {
                        //                joinDialog.open();
                    }
                }
            }
        }
    }
}
