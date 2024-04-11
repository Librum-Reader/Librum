import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Librum.elements
import Librum.style
import Librum.icons
import Librum.controllers
import Librum.fonts

Item {
    id: root
    property int sidebarWidth
    property int arrowRotation
    signal clicked

    implicitWidth: 72
    implicitHeight: 60

    Pane {
        id: container
        padding: 0
        anchors.fill: parent
        background: Rectangle {
            color: "transparent"
        }

        RowLayout {
            width: root.sidebarWidth
            height: parent.height
            spacing: 0

            Rectangle {
                id: profilePicture
                Layout.preferredWidth: 36
                Layout.preferredHeight: 36
                Layout.alignment: Qt.AlignVCenter
                Layout.leftMargin: 18
                clip: true
                radius: width
                antialiasing: true
                color: UserController.profilePicture.length
                       === 0 ? Style.colorDefaultProfilePicture : "transparent"

                Label {
                    id: initials
                    anchors.centerIn: parent
                    visible: UserController.profilePicture.length === 0
                    text: UserController.name[0].toUpperCase()
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
        }
    }

    function giveFocus() {
        root.forceActiveFocus()
    }
}
