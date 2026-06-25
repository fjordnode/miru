import QtQuick
import QtQuick.Layouts
import StremioLinux

ColumnLayout {
    id: root

    property string title: "Search Results"
    property string subtitle: ""
    property var model: []
    signal openRequested(var item)

    readonly property int cardWidth: 168
    readonly property int cardHeight: cardWidth * 1.5 + 52
    readonly property int gridSpacing: Theme.s16
    readonly property int gridColumns: Math.max(1, Math.floor((width + gridSpacing) / (cardWidth + gridSpacing)))

    Layout.fillWidth: true
    spacing: Theme.s16

    RowLayout {
        Layout.fillWidth: true
        spacing: Theme.s12

        Rectangle { Layout.preferredWidth: 4; Layout.preferredHeight: 22; radius: 2; color: Theme.accent }
        Text {
            text: root.title
            color: Theme.text
            font.pixelSize: Theme.fH3
            font.bold: true
        }
        Text {
            visible: root.subtitle.length > 0
            text: root.subtitle
            color: Theme.textMute
            font.pixelSize: Theme.fSmall
            Layout.alignment: Qt.AlignBottom
            bottomPadding: 4
        }
        Item { Layout.fillWidth: true }
    }

    GridLayout {
        visible: root.model && root.model.length > 0
        Layout.fillWidth: true
        columns: root.gridColumns
        columnSpacing: root.gridSpacing
        rowSpacing: Theme.s24

        Repeater {
            model: root.model || []
            delegate: PosterCard {
                required property var modelData
                Layout.preferredWidth: root.cardWidth
                Layout.preferredHeight: root.cardHeight
                item: modelData
                onClicked: clickedItem => root.openRequested(clickedItem)
            }
        }
    }
}
