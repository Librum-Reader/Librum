import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Librum.fonts
import Librum.style

Item {
    id: root
    property string uuid
    property string icon
    property string title
    property bool selected: false

    signal clicked

    implicitHeight: 32

    Rectangle {
        anchors.fill: parent
        color: unsortedArea.containsMouse
               || root.selected ? Style.colorTransparentHighlight : "transparent"
        opacity: 0.08
        radius: 4
    }

    RowLayout {
        height: parent.height
        width: parent.width
        spacing: 8

        LayoutMirroring.enabled: baseRoot.rightAlign
        LayoutMirroring.childrenInherit: true

        Image {
            Layout.leftMargin: 15
            sourceSize.width: 18
            Layout.alignment: Qt.AlignVCenter
            source: root.icon
            fillMode: Image.PreserveAspectFit
        }

        Label {
            Layout.preferredWidth: implicitWidth
            Layout.alignment: Qt.AlignVCenter
            text: root.title
            color: Style.colorText
            font.pointSize: Fonts.size10dot25
            font.weight: Font.Medium
        }

        Item {
            Layout.fillWidth: true
        }
    }

    MouseArea {
        id: unsortedArea
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor

        onClicked: root.clicked()
    }
}
