import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts
import Librum.style
import Librum.fonts
import Librum.controllers
import CustomComponents

Page {
    id: root
    horizontalPadding: 18
    background: Rectangle {
        color: Style.colorPageBackground
    }

    Label {
        id: welcomeText
        anchors.centerIn: parent
        text: qsTr("Home Page")
        color: Style.colorTitle
        font.weight: Font.Bold
        font.pointSize: Fonts.size28
    }
}
