import QtQuick 2.1

FocusScope {
    id: root

    property int currentIndex: 0
    property int count: 0
    property ListModel __tabs: ListModel {}

    function removeTab(index) {
        var tab = __tabs.get(index).tab
        __tabs.remove(index, 1)
        tab.destroy()

        if (currentIndex > 0)
            currentIndex--

        __setOpacities()
    }

    function __setOpacities() {
        for (var i = 0; i < __tabs.count; ++i) {
            var child = __tabs.get(i).tab
            child.visible = (i == currentIndex ? true : false)
        }

        count = __tabs.count
    }

    Rectangle {
        id: t
        implicitWidth: 44
        implicitHeight: parent.height
        anchors.left: parent.left
        color: "#434547"

        ListView {
            id: tabrow
            anchors.fill: parent

            Accessible.role: Accessible.PageTabList
            orientation: Qt.Vertical
            interactive: false
            focus: true

            currentIndex: root.currentIndex
            onCurrentIndexChanged: tabrow.positionViewAtIndex(currentIndex, ListView.Contain)

            model: root.__tabs

            delegate: Item {
                implicitWidth: ListView.view.width
                implicitHeight: ListView.view.width

                Image {
                    anchors.fill: parent
                    source: "images/tab.png"

                    Image {
                        anchors.centerIn: parent
                        source: modelData.icon
                    }
                }

                Rectangle {
                    width: parent.width
                    height: 1
                    y: parent.height - 1
                    color: "black"
                }

                MouseArea {
                    anchors.fill: parent
                    onPressed: {
                        root.currentIndex = index;
                    }
                }
            }
        }
    }

    Loader {
        id: frameLoader
        z: t.z - 1
        anchors.left: t.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right

        Item {
            id: stack
            anchors.fill: parent
            Component.onCompleted: addTabs(stack.children)

            function addTabs(tabs) {
                var tabAdded = false

                for (var i = 0 ; i < tabs.length ; ++i) {
                    var tab = tabs[i]
                    if (!tab.__inserted && tab.Accessible.role === Accessible.LayeredPane) {
                        tab.__inserted = true

                        if (tab.parent === root) {
                            tab.parent = stack
                            tab.Component.onDestruction.connect(stack.onDynamicTabDestroyed.bind(tab))
                        }

                        __tabs.append({tab: tab})
                        tabAdded = true
                    }
                }

                if (tabAdded)
                    __setOpacities()
            }

            function onDynamicTabDestroyed() {
                for (var i = 0; i < stack.children.length; ++i) {
                    if (this === stack.children[i]) {
                        root.removeTab(i)
                        break
                    }
                }
            }
        }
    }

    onCurrentIndexChanged: __setOpacities()
    onChildrenChanged: stack.addTabs(root.children)
}