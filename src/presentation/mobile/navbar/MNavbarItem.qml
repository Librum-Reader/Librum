import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Librum.style
import Librum.fonts
import CustomComponents

Item {
    id: root
    property alias text: label.text
    property alias icon: icon.source
    property bool selected: false
    property int topMargin: 0
    property int iconHeight: 20
    signal clicked

    implicitHeight: 66
    implicitWidth: 56

    ColumnLayout {
        anchors.centerIn: parent
        spacing: 4

        IconImage {
            id: icon
            Layout.alignment: Qt.AlignHCenter
            Layout.topMargin: root.topMargin
            opacity: tapHandler.pressed ? 0.75 : 1
            sourceSize.height: root.iconHeight
            fillMode: Image.PreserveAspectFit
            color: root.selected ? Style.colorBasePurple : "#9F9F9F"
        }

        Label {
            id: label
            Layout.alignment: Qt.AlignHCenter
            color: root.selected ? Style.colorBasePurple : "#9F9F9F"
            font.pointSize: Fonts.size12
        }
    }

    TapHandler {
        id: tapHandler
        onTapped: root.clicked()
    }
}
