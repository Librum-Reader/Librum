import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Librum.style
import Librum.fonts

Item {
    id: root
    property string image
    property int imageWidth: 15
    property string text
    property int textSpacing: 5
    signal clicked

    implicitWidth: 125
    implicitHeight: layout.implicitHeight

    RowLayout {
        id: layout
        anchors.fill: parent
        spacing: root.textSpacing
        opacity: mouseArea.pressed ? 0.8 : 1

        Image {
            id: icon
            source: root.image
            fillMode: Image.PreserveAspectFit
            sourceSize.width: root.imageWidth
        }

        Label {
            id: labels
            text: root.text
            color: Style.colorLightText
            font.pointSize: Fonts.size10dot5
            font.weight: Font.Medium
        }

        Item {
            Layout.fillWidth: true
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent

        onClicked: root.clicked()
    }
}
