import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Librum.style
import Librum.fonts
import Librum.controllers
import CustomComponents

Page {
    id: page
    horizontalPadding: 18
    background: Rectangle {
        color: Style.colorPageBackground
    }

    ColumnLayout {
        id: layout
        anchors.fill: parent
        spacing: 0

        Item {
            Layout.fillHeight: true
            Layout.minimumHeight: 32
            Layout.maximumHeight: 52
        }

        MLogo {
            id: logo
            Layout.alignment: Qt.AlignHCenter
            Layout.preferredWidth: 92
            Layout.preferredHeight: 92
        }

        Label {
            id: welcomeText
            Layout.alignment: Qt.AlignHCenter
            Layout.topMargin: 40
            text: qsTr("Welcome!")
            color: Style.colorTitle
            font.weight: Font.Bold
            font.pointSize: Fonts.size26
        }

        Label {
            id: registerText
            Layout.topMargin: 4
            Layout.alignment: Qt.AlignHCenter
            text: qsTr("Create an account and start reading!")
            color: Style.colorSubtitle
            font.pointSize: Fonts.size13dot25
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.topMargin: 20
            spacing: 8

            MLabeledInputBox {
                id: firstnameInput
                Layout.fillWidth: true
                Layout.preferredHeight: 54
                placeHolderText: qsTr("First name")
            }

            MLabeledInputBox {
                id: lastnameInput
                Layout.fillWidth: true
                Layout.preferredHeight: 54
                placeHolderText: qsTr("Last name")
            }
        }

        MLabeledInputBox {
            id: emailInput
            Layout.fillWidth: true
            Layout.preferredHeight: 54
            Layout.topMargin: 8
            placeHolderText: qsTr("Email")
        }

        MLabeledInputBox {
            id: passwordInput
            Layout.fillWidth: true
            Layout.preferredHeight: 54
            Layout.topMargin: 8
            isPassword: true
            placeHolderText: qsTr("Password")
            textHidden: true
        }

        MButton {
            id: registerButton
            Layout.fillWidth: true
            Layout.preferredHeight: 54
            Layout.topMargin: 20
            borderWidth: 0
            radius: 8
            backgroundColor: Style.colorBasePurple
            fontSize: Fonts.size16
            opacityOnPressed: 0.85
            textColor: Style.colorFocusedButtonText
            fontWeight: Font.Bold
            text: qsTr("Register")

            onClicked: {

            }
        }

        Item {
            id: loginLinkContainer
            Layout.alignment: Qt.AlignHCenter
            Layout.topMargin: 10
            implicitHeight: loginLinkRow.implicitHeight + 8
            implicitWidth: loginLinkRow.implicitWidth + 8

            RowLayout {
                id: loginLinkRow
                anchors.centerIn: parent
                spacing: 0

                Label {
                    text: qsTr("Already have an account? ")
                    font.pointSize: Fonts.size13
                    opacity: loginLinkArea.pressed ? 0.8 : 1
                    font.weight: Font.Medium
                    color: Style.colorText
                }

                Label {
                    text: qsTr("Sign in!")
                    font.pointSize: Fonts.size13
                    opacity: loginLinkArea.pressed ? 0.8 : 1
                    font.weight: Font.Medium
                    color: Style.colorBasePurple
                }
            }

            TapHandler {
                id: loginLinkArea
                onTapped: loadPage(loginPage)
            }
        }

        Item {
            Layout.fillHeight: true
        }

        Label {
            textFormat: Text.RichText
            Layout.alignment: Qt.AlignHCenter
            Layout.fillWidth: true
            Layout.bottomMargin: 8
            wrapMode: Text.WordWrap
            //: Make sure to translate the following words together to make a logical sentence
            text: qsTr('By clicking Register, you agree to our') + '<br><a href="'
                  + AppInfoController.website + '/TermsOfService" style="text-decoration: none; color: '
                  + Style.colorBasePurple + ';"> ' + qsTr('Terms') + '</a>' + ' '
                  + qsTr('and') + ' <a href="' + AppInfoController.website
                  + '/privacypolicy" style="text-decoration: none; color: ' + Style.colorBasePurple
                  + ';">' + qsTr('Privacy Policy') + '</a>'
            horizontalAlignment: Text.AlignHCenter
            font.pointSize: Fonts.size12
            font.weight: Font.Medium
            color: Style.colorPlaceholderText

            onLinkActivated: link => Qt.openUrlExternally(link)
        }
    }
}
