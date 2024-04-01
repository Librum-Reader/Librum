import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.platform
import Librum.style
import Librum.fonts
import Librum.icons
import Librum.controllers
import CustomComponents
import "../profilePage"

Page {
    id: root
    padding: 0
    background: Rectangle {
        color: Style.colorPageBackground
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        Rectangle {
            id: topPart
            Layout.fillWidth: true
            Layout.preferredHeight: 100
            color: Style.colorControlBackground

            RowLayout {
                width: parent.width
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 2

                MBackButton {
                    id: backButton
                    Layout.leftMargin: 12
                    Layout.bottomMargin: -3

                    onClicked: loadPage(profilePage, navbar.profileItem)
                }

                Label {
                    id: pageTitle
                    Layout.alignment: Qt.AlignHCenter
                    text: qsTr("My Account")
                    color: Style.colorTitle
                    font.weight: Font.Bold
                    font.pointSize: Fonts.size20
                }

                // An element of the size of the back button to be able to correctly align the title
                Item {
                    id: counterElem
                    Layout.preferredWidth: backButton.width
                    Layout.preferredHeight: backButton.height
                    Layout.rightMargin: 12
                }
            }
        }

        Rectangle {
            id: middlePart
            Layout.fillWidth: true
            Layout.preferredHeight: 72
            color: Style.colorPageBackground
        }

        Pane {
            id: mainPart
            Layout.fillWidth: true
            Layout.fillHeight: true
            verticalPadding: 0
            horizontalPadding: 16
            background: Rectangle {
                color: Style.colorControlBackground
            }

            ColumnLayout {
                width: parent.width
                spacing: 0

                MProfilePicture {
                    Layout.preferredWidth: 92
                    Layout.preferredHeight: 92
                    Layout.alignment: Qt.AlignHCenter
                    Layout.topMargin: -(Layout.preferredWidth / 2)
                    fontSize: Fonts.size34
                }

                Item {
                    id: changePictureItem
                    Layout.alignment: Qt.AlignHCenter
                    Layout.preferredHeight: changePictureText.implicitHeight * 2.2
                    Layout.preferredWidth: 140
                    Layout.topMargin: 2

                    Label {
                        id: changePictureText
                        anchors.centerIn: parent
                        text: "Change Picture"
                        color: Style.colorBasePurple
                        font.weight: Font.DemiBold
                        font.pointSize: Fonts.size15dot5
                        opacity: changePictureTapHandler.pressed ? 0.7 : 1
                    }

                    TapHandler {
                        id: changePictureTapHandler

                        onTapped: fileDialog.open()
                    }
                }

                MLabeledInputBox {
                    id: nameInput
                    Layout.fillWidth: true
                    Layout.preferredHeight: 54
                    Layout.topMargin: 52
                    text: UserController.name
                    placeHolderText: qsTr("Name")
                }

                MLabeledInputBox {
                    id: emailInput
                    Layout.fillWidth: true
                    Layout.preferredHeight: 54
                    Layout.topMargin: 18
                    text: UserController.name
                    placeHolderText: qsTr("Email")
                    readOnly: true
                }

                MButton {
                    id: signInButton
                    Layout.fillWidth: true
                    Layout.preferredHeight: 54
                    Layout.topMargin: 36
                    borderWidth: 0
                    radius: 40
                    backgroundColor: Style.colorBasePurple
                    fontSize: Fonts.size16
                    opacityOnPressed: 0.85
                    textColor: Style.colorFocusedButtonText
                    text: qsTr("Save Changes")

                    onClicked: ;
                }
            }
        }
    }

    FileDialog {
        id: fileDialog
        fileMode: FileDialog.OpenFiles
        folder: StandardPaths.writableLocation(StandardPaths.PicturesLocation)

        onAccepted: root.currentImage = file
    }
}
