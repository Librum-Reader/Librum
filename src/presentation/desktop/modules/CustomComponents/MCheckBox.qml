import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Librum.style
import Librum.icons


/**
 A checkbox which toggles between two pictures, depending on its state
 */
Item {
    id: root
    property color borderColor: Style.colorCheckboxBorder
    property color checkedBorderColor: Style.colorContainerBorder
    property int borderRadius: 4
    property int borderWidth: 1
    property int checkedBorderWidth: 0
    property color uncheckedBackgroundColor: "transparent"
    property color checkedBackgroundColor: Style.colorBasePurple
    property string image: Icons.checkWhite
    property int imageSize: container.width - 10
    property bool checked: false
    property bool enabled: true
    signal clicked

    implicitWidth: 22
    implicitHeight: 22

    Rectangle {
        id: container
        anchors.fill: parent
        radius: root.borderRadius
        border.width: root.checked ? root.checkedBorderWidth : root.borderWidth
        border.color: root.activeFocus ? root.checkedBorderColor : root.borderColor
        color: root.checked ? root.checkedBackgroundColor : root.uncheckedBackgroundColor
        antialiasing: true

        Image {
            id: image
            anchors.centerIn: parent
            visible: root.checked ? true : false
            sourceSize.width: root.imageSize
            source: root.image
            fillMode: Image.PreserveAspectFit
        }

        MouseArea {
            id: mouseArea
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: Qt.PointingHandCursor

            onClicked: root.toggle()
        }
    }

    function toggle() {
        if (!root.enabled)
            return

        root.clicked()
        root.checked = !root.checked
    }

    function giveFocus() {
        root.forceActiveFocus()
    }
}
