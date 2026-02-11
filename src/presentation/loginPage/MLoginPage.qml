import QtQuick
import QtCore
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window
import CustomComponents
import Librum.style
import Librum.icons
import Librum.controllers
import Librum.models
import Librum.fonts

MFlickWrapper {
    opacity: 0
    id: root
    contentHeight: Window.height < layout.implicitHeight ? layout.implicitHeight : Window.height

    // Passing the focus to emailInput on Component.onCompleted() causes it
    // to pass controll back to root for some reason, this fixes the focus problem.
    onActiveFocusChanged: if (activeFocus)
                              emailInput.giveFocus()

    Component.onCompleted: {
        console.log("LoginPage Component.onCompleted called")
        console.log("baseRoot:", baseRoot, "externalBookMode:", baseRoot.externalBookMode)
        // Focus the emailInput when page has loaded.
        emailInput.giveFocus()

        // For some reason this prevents a SEGV. Directly calling the auto login
        // directly causes the application to crash on startup.
        console.log("Starting autoLoginTimer...")
        autoLoginTimer.start()

        // Determine the index of the language used in the combobox
        let index = languageComboBox.getIndexByText(AppInfoController.language)
        languageComboBox.defaultIndex = index
    }

    Timer {
        id: autoLoginTimer
        interval: 0
        onTriggered: {
            console.log("Auto-login timer triggered, calling loginOffline")
            AuthController.loginOffline()
        }
    }

    Shortcut {
        sequence: "Ctrl+Return"
        onActivated: internal.login()
    }

    Connections {
        id: proccessLoginResult
        target: AuthController
        function onLoginFinished(errorCode, message) {
            console.log("AuthController.loginFinished signal received, errorCode:", errorCode, "message:", message)
            internal.processLoginResult(errorCode, message)
        }
    }

    Connections {
        id: proccessLoadingUserResult
        target: UserController
        function onFinishedLoadingUser(success) {
            console.log("UserController.finishedLoadingUser signal received, success:", success)
            console.log("baseRoot.externalBookMode:", baseRoot.externalBookMode)
            if (success) {
                console.log("User loaded successfully, loading home page...")
                if (baseRoot.externalBookMode)
                    loadPage(externalReadingPage)
                else {
                    console.log("Calling loadPage for home page...")
                    loadPage(homePage, sidebar.homeItem, false)
                }
            } else {
                console.log("User loading failed, showing login failed popup")
                loginFailedPopup.open()
            }

            loginButton.loading = false
        }
    }

    Page {
        id: page
        anchors.fill: parent
        bottomPadding: 22
        background: Rectangle {
            color: Style.colorAuthenticationPageBackground
        }

        MComboBox {
            id: languageComboBox
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.leftMargin: 12
            width: 200
            height: 12
            dropDownIconLeft: true
            boxBackgroundColor: "transparent"
            checkBoxStyle: false
            fontSize: Fonts.size12
            selectedItemFontSize: Fonts.size12
            maxHeight: 200
            spacing: 14
            dropdownIconSize: 10
            popupSpacing: 14
            borderWidth: 0

            model: TranslationsModel

            onItemChanged: index => AppInfoController.switchToLanguage(
                               model.get(index).code)

            function getIndexByText(searchText) {
                for (var i = 0; i < model.count; ++i) {
                    if (model.get(i).text.toLocaleLowerCase(
                                ) === searchText.toLocaleLowerCase()) {
                        return i
                    }
                }

                return -1
            }
        }

        ColumnLayout {
            id: layout
            anchors.centerIn: parent
            width: 544

            Pane {
                id: backgroundRect
                Layout.fillWidth: true
                topPadding: 48
                horizontalPadding: 71
                bottomPadding: 42
                background: Rectangle {
                    color: Style.colorContainerBackground
                    radius: 5
                }

                ColumnLayout {
                    width: parent.width
                    spacing: 0

                    MLogo {
                        id: logo
                        Layout.alignment: Qt.AlignHCenter
                    }

                    Label {
                        id: welcomeText
                        Layout.alignment: Qt.AlignHCenter
                        Layout.topMargin: 24
                        text: qsTr("Welcome back!")
                        color: Style.colorText
                        font.bold: true
                        font.pointSize: Fonts.size26
                    }

                    Label {
                        id: loginText
                        Layout.topMargin: 4
                        Layout.alignment: Qt.AlignHCenter
                        text: qsTr("Log into your account")
                        color: Style.colorSubtitle
                        font.pointSize: Fonts.size13
                    }

                    MLabeledInputBox {
                        id: emailInput
                        Layout.fillWidth: true
                        Layout.topMargin: 32
                        placeholderContent: "kaidoe@gmail.com"
                        placeholderColor: Style.colorPlaceholderText
                        headerText: qsTr("Email")

                        onEdited: internal.clearLoginError()
                        Keys.onPressed: event => {
                                            if (event.key === Qt.Key_Down
                                                || event.key === Qt.Key_Return) {
                                                passwordInput.giveFocus()
                                            }
                                        }
                    }

                    MLabeledInputBox {
                        id: passwordInput
                        Layout.fillWidth: true
                        Layout.topMargin: 22
                        headerText: qsTr("Password")
                        image: Icons.eyeOn
                        toggledImage: Icons.eyeOff

                        onEdited: internal.clearLoginError()
                        Keys.onPressed: event => {
                                            if (event.key === Qt.Key_Up) {
                                                emailInput.giveFocus()
                                            } else if (event.key === Qt.Key_Down
                                                       || event.key === Qt.Key_Return) {
                                                rememberMeCheckBox.giveFocus()
                                            }
                                        }
                    }

                    Label {
                        id: generalErrorText
                        Layout.topMargin: 8
                        visible: false
                        color: Style.colorErrorText
                    }

                    RowLayout {
                        id: optionsLayout
                        Layout.preferredWidth: parent.width
                        Layout.fillWidth: true
                        Layout.topMargin: 24

                        MCheckBox {
                            id: rememberMeCheckBox
                            Layout.preferredWidth: 20
                            Layout.preferredHeight: 20

                            Keys.onPressed: event => {
                                                if (event.key === Qt.Key_Return) {
                                                    toggle()
                                                } else if (event.key === Qt.Key_Up) {
                                                    passwordInput.giveFocus()
                                                } else if (event.key === Qt.Key_Down) {
                                                    loginButton.giveFocus()
                                                }
                                            }
                        }

                        Label {
                            id: rememberMeText
                            text: qsTr("Remember me")
                            Layout.alignment: Qt.AlignVCenter
                            Layout.leftMargin: 4
                            font.pointSize: Fonts.size11
                            color: Style.colorText

                            MouseArea {
                                anchors.fill: parent
                                cursorShape: Qt.PointingHandCursor

                                onClicked: rememberMeCheckBox.toggle()
                            }
                        }

                        Item {
                            id: widthFiller
                            Layout.fillWidth: true
                        }

                        Label {
                            id: forgotPasswordLabel
                            text: qsTr("Forgot password?")
                            Layout.alignment: Qt.AlignVCenter
                            Layout.leftMargin: 3
                            font.pointSize: Fonts.size10dot5
                            opacity: forgotPasswordPageRedirection.pressed ? 0.8 : 1
                            color: Style.colorBasePurple

                            MouseArea {
                                id: forgotPasswordPageRedirection
                                anchors.fill: parent
                                cursorShape: Qt.PointingHandCursor

                                onClicked: loadPage(forgotPasswordPage)
                            }
                        }
                    }

                    MButton {
                        id: loginButton
                        Layout.fillWidth: true
                        Layout.preferredHeight: 40
                        Layout.topMargin: 42
                        borderWidth: 0
                        backgroundColor: Style.colorBasePurple
                        fontSize: Fonts.size12
                        opacityOnPressed: 0.85
                        textColor: Style.colorFocusedButtonText
                        fontWeight: Font.Bold
                        text: qsTr("Login")

                        onClicked: internal.login()

                        onFocusChanged: {
                            if (focus)
                                opacity = opacityOnPressed
                            else
                                opacity = 1
                        }

                        Keys.onPressed: event => {
                                            if (event.key === Qt.Key_Up)
                                            rememberMeCheckBox.giveFocus()
                                            else if (event.key === Qt.Key_Return)
                                            internal.login()
                                        }
                    }
                }
            }

            Label {
                id: registerLinkLabel
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 14
                text: qsTr("Don't have an account? Register")
                font.pointSize: Fonts.size10dot5
                opacity: registerLinkArea.pressed ? 0.8 : 1
                color: Style.colorBasePurple

                MouseArea {
                    id: registerLinkArea
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor

                    onClicked: loadPage(registerPage)
                }
            }
        }
    }

    MWarningPopup {
        id: loginFailedPopup
        x: Math.round(root.width / 2 - implicitWidth / 2)
        y: Math.round(root.height / 2 - implicitHeight / 2) - 75
        visible: false
        title: qsTr("We're Sorry")
        message: qsTr("Logging you in failed, please try again later.")
        leftButtonText: qsTr("Ok")
        rightButtonText: qsTr("Report")
        messageBottomSpacing: 8

        onDecisionMade: close()
        onOpenedChanged: if (opened)
                             loginFailedPopup.giveFocus()
    }

    QtObject {
        id: internal
        property color previousBorderColor: emailInput.borderColor

        function login() {
            loginButton.loading = true
            AuthController.loginUser(emailInput.text, passwordInput.text,
                                     rememberMeCheckBox.checked)
        }

        function processLoginResult(errorCode, message) {
            console.log("processLoginResult called with errorCode:", errorCode, "message:", message)
            if (errorCode === -1) {
                console.log("Login successful, loading user...")
                UserController.loadUser(false)
            } else if (errorCode !== 25) {
                console.log("Login failed with error:", errorCode)
                internal.setLoginError(errorCode, message)
                loginButton.loading = false
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
