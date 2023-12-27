import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Librum.style
import Librum.icons
import Librum.fonts

Item {
    id: root
    signal tagsSelected
    signal tagsRemoved

    implicitWidth: container.width
    implicitHeight: 36

    Pane {
        id: container
        property int hPadding: 12
        property int minWidth: 104

        height: parent.height
        width: (layout.width + hPadding * 2) < minWidth ? minWidth : layout.width + hPadding * 2
        padding: 0
        background: Rectangle {
            color: Style.colorControlBackground
            border.width: 1
            border.color: Style.colorContainerBorder
            radius: 5
        }

        RowLayout {
            id: layout
            anchors.centerIn: parent
            spacing: 6

            Image {
                id: tagIcon
                sourceSize.height: 18
                source: Icons.tag
                fillMode: Image.PreserveAspectFit
            }

            Label {
                id: tagLabel
                color: Style.colorText
                text: qsTr("Tags")
                font.pointSize: Fonts.size12
                font.weight: Font.Bold
            }
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true

        onEntered: mouseArea.cursorShape = Qt.PointingHandCursor

        onClicked: selectionPopup.opened ? selectionPopup.close(
                                               ) : selectionPopup.open()
    }

    MTagSelectorPopup {
        id: selectionPopup
        y: root.height + 6

        onClosed: {
            if (selectionPopup.hasAtLeastOneTagSelected())
                root.tagsSelected()
            else
                root.tagsRemoved()
        }
    }

    function clearSelections() {
        selectionPopup.clearSelections()
    }

    function giveFocus() {
        root.forceActiveFocus()
    }
}
