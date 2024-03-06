import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Librum.style
import Librum.fonts
import Librum.icons

Pane {
    id: root
    implicitWidth: 520
    implicitHeight: 46
    padding: 0
    horizontalPadding: 16
    background: Rectangle {
        color: Style.colorControlBackground
        border.color: Style.colorContainerBorder
        radius: 5
        border.width: 2
    }

    RowLayout {
        width: parent.width
        anchors.verticalCenter: parent.verticalCenter
        spacing: 1

        Image {
            id: searchBarIcon
            source: Icons.search
            fillMode: Image.PreserveAspectFit
            sourceSize.height: 18
        }

        TextField {
            id: inputField
            Layout.fillWidth: true
            selectByMouse: true
            color: Style.colorBaseInputText
            font.pointSize: Fonts.size12dot5
            placeholderText: "Search Books..."
            placeholderTextColor: Style.colorPlaceholderText
            selectionColor: Style.colorTextSelection
            background: Rectangle {
                anchors.fill: parent
                radius: 5
                color: "transparent"
            }

            onTextEdited: {
                if (!popup.opened)
                    popup.open()
            }
        }
    }

    MBookSelectorPopup {
        id: popup
        width: root.width
        y: root.height + 8
        x: -root.horizontalPadding
    }

    function giveFocus() {
        inputField.forceActiveFocus()
    }
}
