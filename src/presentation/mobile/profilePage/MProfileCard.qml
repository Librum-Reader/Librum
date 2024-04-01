import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts
import Librum.style
import Librum.fonts
import Librum.controllers
import CustomComponents

Rectangle {
    color: "transparent"
    implicitHeight: 44

    RowLayout {
        id: layout

        MProfilePicture {
            Layout.alignment: Qt.AlignVCenter
        }

        ColumnLayout {
            Layout.leftMargin: 10
            Layout.topMargin: -2
            Layout.alignment: Qt.AlignVCenter
            spacing: -1

            Label {
                Layout.fillWidth: true
                text: UserController.name
                color: Style.colorText
                font.weight: Font.DemiBold
                font.pointSize: Fonts.size18
            }

            Label {
                Layout.fillWidth: true
                text: UserController.email
                color: "#B8B3B3"
                font.weight: Font.Thin
                font.pointSize: Fonts.size15
            }
        }
    }
}
