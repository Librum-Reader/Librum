import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Librum.style
import Librum.icons
import Librum.fonts

Popup {
    id: root
    property var focusedItem
    property alias content: text.text
    property int triangleOffset: 4
    property int xOffset: 0

    implicitWidth: text.implicitWidth + 2 * container.horizontalPadding
    padding: 0
    parent: Overlay.overlay
    background: Rectangle {
        color: "transparent"
    }

    onOpenedChanged: {
        let mappedPoint = focusedItem.mapToItem(baseRoot.contentItem, 0, 0)
        y = mappedPoint.y - implicitHeight - 6

        // Make sure to position the popup to the left or right of the focused item
        // depending on where it is on the screen and if there is enough space
        if ((mappedPoint.x + implicitWidth + xOffset) < baseRoot.width) {
            internal.leftAligned = false
            x = mappedPoint.x + xOffset
        } else {
            internal.leftAligned = true
            x = mappedPoint.x - implicitWidth + focusedItem.width - xOffset
        }
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        Pane {
            id: container
            Layout.fillHeight: true
            Layout.fillWidth: true
            horizontalPadding: 10
            verticalPadding: 10
            background: Rectangle {
                color: Style.colorPopupBackground
                border.width: 1
                border.color: Style.colorContainerBorder
                radius: 4
                antialiasing: true
            }

            Label {
                id: text
                width: parent.width
                color: Style.colorText
                font.pointSize: Fonts.bigSize
            }
        }

        Image {
            id: triangleDecorator
            Layout.topMargin: -1
            Layout.leftMargin: internal.leftAligned ? 0 : root.triangleOffset
            Layout.rightMargin: internal.leftAligned ? root.triangleOffset : 0
            Layout.alignment: internal.leftAligned ? Qt.AlignRight : Qt.AlignLeft
            source: Icons.popupDroplet
            rotation: 180
        }
    }

    QtObject {
        id: internal
        property bool leftAligned: false
    }
}
