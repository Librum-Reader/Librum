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

    Connections {
        id: proccessLoginResult
        target: AuthController
        function onLoginFinished(errorCode, message) {
            internal.processLoginResult(errorCode, message)
        }
    }

    Connections {
        id: proccessLoadingUserResult
        target: UserController
        function onFinishedLoadingUser(success) {
            if (success)
                loadPage(homePage, navbar.homeItem)
            else
                loginFailedPopup.open()
        }
    }

    ColumnLayout {
        id: layout
        anchors.fill: parent
        spacing: 0

        Item {
            Layout.fillHeight: true
            Layout.minimumHeight: 32
            Layout.maximumHeight: 68
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
            Layout.topMargin: 52
            text: qsTr("Welcome back!")
            color: Style.colorTitle
            font.weight: Font.Bold
            font.pointSize: Fonts.size26
        }

        Label {
            id: loginText
            Layout.topMargin: 4
            Layout.alignment: Qt.AlignHCenter
            text: qsTr("Log into your account")
            color: Style.colorSubtitle
            font.pointSize: Fonts.size13dot25
        }

        MLabeledInputBox {
            id: emailInput
            Layout.fillWidth: true
            Layout.preferredHeight: 54
            Layout.topMargin: 40
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
            id: signInButton
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
            text: qsTr("Sign in")

            onClicked: internal.login()
        }

        Label {
            Layout.topMargin: 16
            Layout.alignment: Qt.AlignHCenter
            text: qsTr("Forgot password?")
            font.pointSize: Fonts.size13dot25
            font.weight: Font.Medium
            opacity: forgotPasswordArea.pressed ? 0.8 : 1
            color: Style.colorBasePurple

            TapHandler {
                id: forgotPasswordArea
                onTapped: loadPage(forgotPasswordPage)
            }
        }

        Item {
            Layout.fillHeight: true
        }

        Item {
            id: registerLinkContainer
            Layout.alignment: Qt.AlignHCenter
            Layout.bottomMargin: 12
            implicitHeight: registerLinkRow.implicitHeight + 8
            implicitWidth: registerLinkRow.implicitWidth + 8

            RowLayout {
                id: registerLinkRow
                anchors.centerIn: parent
                spacing: 0

                Label {
                    text: qsTr("Don't have an account? ")
                    font.pointSize: Fonts.size13
                    opacity: registerLinkArea.pressed ? 0.8 : 1
                    font.weight: Font.Medium
                    color: Style.colorText
                }

                Label {
                    text: qsTr("Register!")
                    font.pointSize: Fonts.size13
                    opacity: registerLinkArea.pressed ? 0.8 : 1
                    font.weight: Font.Medium
                    color: Style.colorBasePurple
                }
            }

            TapHandler {
                id: registerLinkArea
                onTapped: loadPage(registerPage)
            }
        }
    }

    QtObject {
        id: internal
        property color previousBorderColor: emailInput.borderColor

        function login() {
            AuthController.loginUser(emailInput.text, passwordInput.text, true)
        }

        function processLoginResult(errorCode, message) {
            if (errorCode === ErrorCode.NoError) {
                UserController.loadUser(true)
            } else {
                internal.setLoginError(errorCode, message)
            }
        }

        function setLoginError(errorCode, message) {
            switch (errorCode) {
            case ErrorCode.EmailOrPasswordIsWrong:
                emailInput.setError()
                passwordInput.errorText = message
                passwordInput.setError()
                break
            case ErrorCode.EmailAddressTooLong:
                // Fall through
            case ErrorCode.EmailAddressTooShort:
                // Fall through
            case ErrorCode.InvalidEmailAddressFormat:
                emailInput.errorText = message
                emailInput.setError()
                break
            case ErrorCode.PasswordTooLong:
                // Fall through
            case ErrorCode.PasswordTooShort:

                passwordInput.errorText = message
                passwordInput.setError()
                break
            default:
                generalErrorText.text = message
                generalErrorText.visible = true
            }
        }

        function clearLoginError() {
            emailInput.errorText = ""
            emailInput.clearError()
            passwordInput.errorText = ""
            passwordInput.clearError()

            generalErrorText.visible = false
            generalErrorText.text = ""
        }
    }
}
