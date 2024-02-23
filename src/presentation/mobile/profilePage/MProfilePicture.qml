import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts
import Librum.style
import Librum.fonts
import Librum.controllers

Rectangle {
    id: root
    implicitWidth: 36
    implicitHeight: 36
    radius: width
    clip: true
    antialiasing: true
    color: UserController.profilePicture.length
           === 0 ? Style.colorDefaultProfilePicture : "transparent"

    Label {
        id: initials
        anchors.centerIn: parent
        visible: UserController.profilePicture.length === 0
        text: UserController.firstName[0].toUpperCase(
                  ) + UserController.lastName[0].toUpperCase()
        font.pointSize: Fonts.size12
        font.bold: true
        color: Style.colorFocusedButtonText
    }

    Image {
        id: profileImage
        visible: UserController.profilePicture.length > 0
        anchors.centerIn: parent
        Layout.leftMargin: 18
        source: UserController.profilePicture
        sourceSize.height: parent.height
        fillMode: Image.PreserveAspectFit
        cache: false
    }

    MouseArea {
        anchors.fill: parent
        cursorShape: Qt.PointingHandCursor

        onClicked: root.clicked()
    }
}
