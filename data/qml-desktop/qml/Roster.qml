import QtQuick 2.1

Item {
    anchors.fill: parent

    Rectangle {
        anchors.fill: parent
        color: "#f9f9f9"
    }

    ListView {
        id: rosterView;
        anchors.fill: parent
        model: rosterModel;
        highlight: HighlightListView { }
        delegate: RosterDelegate { }
        spacing: 2
        section.property: "alphabet"
        section.labelPositioning: ViewSection.CurrentLabelAtStart | ViewSection.InlineLabels
        section.delegate: SectionHeading { }
    }
}
