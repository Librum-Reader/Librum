import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Librum.style
import Librum.fonts
import Librum.icons
import CustomComponents

Page {
    id: page
    padding: 0
    background: Rectangle {
        color: Style.colorPageBackground
    }

    ColumnLayout {
        id: layout
        width: parent.width
        spacing: 0

        SwipeView {
            id: view
            Layout.fillWidth: true
            Layout.preferredHeight: 460
            currentIndex: 0

            Page {
                id: firstPage
                horizontalPadding: 22
                background: Rectangle {
                    opacity: 0
                }

                ColumnLayout {
                    width: parent.width

                    Image {
                        Layout.fillWidth: true
                        Layout.topMargin: 72
                        Layout.alignment: Qt.AlignHCenter
                        sourceSize.width: 290
                        fillMode: Image.PreserveAspectFit
                        source: Icons.readingTimeIllustration
                    }

                    Label {
                        id: title
                        Layout.alignment: Qt.AlignHCenter
                        Layout.fillWidth: true
                        Layout.topMargin: 36
                        Layout.leftMargin: 5
                        Layout.rightMargin: 5
                        text: qsTr("Reading books was never this easy")
                        color: Style.colorText
                        wrapMode: Text.WordWrap
                        horizontalAlignment: Text.AlignHCenter
                        font.bold: true
                        font.pointSize: Fonts.size25
                    }

                    Label {
                        id: description
                        Layout.alignment: Qt.AlignHCenter
                        Layout.fillWidth: true
                        Layout.topMargin: 12
                        text: qsTr("Increase your productivity through a simple and modern interface combined with powerful tools.")
                        lineHeight: 1.1
                        color: "#A6A6A6"
                        wrapMode: Text.WordWrap
                        horizontalAlignment: Text.AlignHCenter
                        font.pointSize: Fonts.size15
                    }
                }
            }

            Page {
                id: secondPage
                horizontalPadding: 22
                background: Rectangle {
                    opacity: 0
                }

                ColumnLayout {
                    width: parent.width

                    Image {
                        Layout.fillWidth: true
                        Layout.topMargin: 72
                        Layout.alignment: Qt.AlignHCenter
                        sourceSize.width: 290
                        fillMode: Image.PreserveAspectFit
                        source: Icons.readingTimeIllustration
                    }

                    Label {
                        Layout.alignment: Qt.AlignHCenter
                        Layout.fillWidth: true
                        Layout.topMargin: 36
                        Layout.leftMargin: 5
                        Layout.rightMargin: 5
                        text: qsTr("Build your own online library")
                        color: Style.colorText
                        wrapMode: Text.WordWrap
                        horizontalAlignment: Text.AlignHCenter
                        font.bold: true
                        font.pointSize: Fonts.size25
                    }

                    Label {
                        Layout.alignment: Qt.AlignHCenter
                        Layout.fillWidth: true
                        Layout.topMargin: 12
                        text: qsTr("Add books to build your own online library that you can access at anytime from anywhere.")
                        lineHeight: 1.1
                        color: "#A6A6A6"
                        wrapMode: Text.WordWrap
                        horizontalAlignment: Text.AlignHCenter
                        font.pointSize: Fonts.size15
                    }
                }
            }

            Page {
                id: thirdPage
                horizontalPadding: 22
                background: Rectangle {
                    opacity: 0
                }

                ColumnLayout {
                    width: parent.width

                    Image {
                        Layout.fillWidth: true
                        Layout.topMargin: 72
                        Layout.alignment: Qt.AlignHCenter
                        sourceSize.width: 290
                        fillMode: Image.PreserveAspectFit
                        source: Icons.readingTimeIllustration
                    }

                    Label {
                        Layout.alignment: Qt.AlignHCenter
                        Layout.fillWidth: true
                        Layout.topMargin: 36
                        Layout.leftMargin: 5
                        Layout.rightMargin: 5
                        text: qsTr("Organize your library your way")
                        color: Style.colorText
                        wrapMode: Text.WordWrap
                        horizontalAlignment: Text.AlignHCenter
                        font.bold: true
                        font.pointSize: Fonts.size25
                    }

                    Label {
                        Layout.alignment: Qt.AlignHCenter
                        Layout.fillWidth: true
                        Layout.topMargin: 12
                        text: qsTr("Add your books to collections, tag them and sort them in any way you want.")
                        lineHeight: 1.1
                        color: "#A6A6A6"
                        wrapMode: Text.WordWrap
                        horizontalAlignment: Text.AlignHCenter
                        font.pointSize: Fonts.size15
                    }
                }
            }
        }

        PageIndicator {
            id: indicator
            Layout.alignment: Qt.AlignHCenter
            Layout.topMargin: 22
            count: view.count
            currentIndex: view.currentIndex
        }

        MButton {
            id: mainButton
            Layout.fillWidth: true
            Layout.preferredHeight: 54
            Layout.topMargin: 32
            Layout.leftMargin: 22
            Layout.rightMargin: 22
            borderWidth: 0
            radius: 8
            backgroundColor: Style.colorBasePurple
            fontSize: Fonts.size16
            opacityOnPressed: 0.85
            textColor: Style.colorFocusedButtonText
            fontWeight: Font.Bold
            text: view.currentIndex < 2 ? qsTr("Continue") : qsTr("Get started")

            onClicked: {
                if (view.currentIndex < 2) {
                    view.currentIndex++
                } else {
                    loadPage(registerPage)
                }
            }
        }

        MButton {
            id: signInButton
            Layout.fillWidth: true
            Layout.preferredHeight: 54
            Layout.topMargin: 6
            Layout.leftMargin: 22
            Layout.rightMargin: 22
            borderWidth: 0
            radius: 8
            backgroundColor: "#F2F2F2"
            fontSize: Fonts.size16
            opacityOnPressed: 0.6
            textColor: Style.colorBasePurple
            fontWeight: Font.Bold
            text: qsTr("Sign In")

            onClicked: loadPage(loginPage)
        }
    }
}
