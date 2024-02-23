import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts
import Librum.style
import Librum.fonts
import Librum.controllers

Rectangle {
    id: root
    implicitWidth: 44
    implicitHeight: 44
    radius: width
    clip: true
    antialiasing: true
    color: UserController.profilePicture.length
           === 0 ? Style.colorDefaultProfilePicture : "transparent"

    Label {
        id: initials
        anchors.centerIn: parent
        visible: UserController.profilePicture.length === 0
        text: UserController.name[0].toUpperCase()
        font.pointSize: Fonts.size21
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

    TapHandler {
        onTapped: root.clicked()
    }
}
