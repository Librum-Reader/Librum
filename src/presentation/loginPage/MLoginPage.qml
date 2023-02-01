import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15
import CustomComponents 1.0
import Librum.style 1.0
import Librum.icons 1.0
import Librum.controllers 1.0


MFlickWrapper
{
    id: root
    contentHeight: Window.height < layout.implicitHeight ? 
                       layout.implicitHeight : Window.height
    
    // Passing the focus to emailInput on Component.onCompleted() causes it
    // to pass controll back to root for some reason, this fixes the focus problem
    onActiveFocusChanged: if(activeFocus) emailInput.giveFocus()
    
    // Focus the emailInput when page has loaded
    Component.onCompleted: emailInput.giveFocus()
    
    Shortcut
    {
        sequence: "Ctrl+Return"
        onActivated: internal.login()
    }
    
    Connections
    {
        id: proccessLoginResult
        target: AuthController
        function onLoginFinished(success)
        {
            internal.processLoginResult(success);
        }
    }
    
    Connections
    {
        id: proccessLoadingUserResult
        target: UserController
        function onFinishedLoadingUser(success)
        {
            if(success)
                loadPage(homePage, sidebar.homeItem, false);
            else
                loginFailedDialog.open();
        }
    }
    
    
    Page
    {
        id: page
        anchors.fill: parent
        bottomPadding: 22
        background: Rectangle
        {
            anchors.fill: parent
            color: Style.pagesBackground
        }
        
        
        ColumnLayout
        {
            id: layout
            anchors.centerIn: parent
            width: 544
            
            
            Pane
            {
                id: backgroundRect
                Layout.fillWidth: true
                topPadding: 48
                horizontalPadding: 71
                bottomPadding: 42
                background: Rectangle
                {
                    color: Style.containerBackground
                    radius: 5
                }
                
                
                ColumnLayout
                {
                    width: parent.width
                    spacing: 0
                    
                    
                    MLogo
                    {
                        id: logo
                        Layout.alignment: Qt.AlignHCenter
                    }
                    
                    Label
                    {
                        id: welcomeText
                        Layout.alignment: Qt.AlignHCenter
                        Layout.topMargin: 24
                        text: "Welcome back!"
                        color: Style.colorBaseText
                        font.bold: true
                        font.pointSize: 26
                    }
                    
                    Label
                    {
                        id: loginText
                        Layout.topMargin: 4
                        Layout.alignment: Qt.AlignHCenter
                        text: "Log into your account"
                        color: Style.colorSubtitle
                        font.pointSize: 13
                    }
                    
                    
                    MLabeledInputBox
                    {
                        id: emailInput
                        Layout.fillWidth: true
                        Layout.topMargin: 32
                        placeholderContent: "kaidoe@gmail.com"
                        placeholderColor: Style.colorPlaceholderText
                        headerText: "Email"
                        
                        Keys.onPressed: 
                            (event) =>
                            {
                                if(event.key === Qt.Key_Down || event.key === Qt.Key_Return)
                                {
                                    passwordInput.giveFocus();
                                }
                            }
                    }
                    
                    MLabeledInputBox
                    {
                        id: passwordInput
                        Layout.fillWidth: true
                        Layout.topMargin: 22
                        headerText: "Password"
                        image: Icons.eyeOn
                        toggledImage: Icons.eyeOff
                        
                        Keys.onPressed: 
                            (event) =>
                            {
                                if(event.key === Qt.Key_Up)
                                {
                                    emailInput.giveFocus();
                                }
                                else if(event.key === Qt.Key_Down || event.key === Qt.Key_Return)
                                {
                                    rememberMeCheckBox.giveFocus();
                                }
                            }
                    }
                    
                    RowLayout
                    {
                        id: optionsLayout
                        Layout.preferredWidth: parent.width
                        Layout.fillWidth: true
                        Layout.topMargin: 24
                        
                        MCheckBox
                        {
                            id: rememberMeCheckBox
                            Layout.preferredWidth: 20
                            Layout.preferredHeight: 20
                            
                            Keys.onPressed: 
                                (event) =>
                                {
                                    if(event.key === Qt.Key_Return)
                                    {
                                        toggle();
                                    }
                                    else if(event.key === Qt.Key_Up)
                                    {
                                        passwordInput.giveFocus();
                                    }
                                    else if(event.key === Qt.Key_Down)
                                    {
                                        loginButton.giveFocus();
                                    }
                                }
                        }
                        
                        Label
                        {
                            id: rememberMeText
                            text: "Remember me"
                            Layout.alignment: Qt.AlignVCenter
                            Layout.leftMargin: 4
                            font.pointSize: 11
                            color: Style.colorMediumText
                        }
                        
                        Item
                        {
                            id: widthFiller
                            Layout.fillWidth: true
                        }
                        
                        Label
                        {
                            id: forgotPasswordLabel
                            text: "Forgot password?"
                            Layout.alignment: Qt.AlignVCenter
                            Layout.leftMargin: 3
                            font.pointSize: 10.5
                            color: Style.colorBasePurple
                            
                            MouseArea
                            {
                                id: forgotPasswordPageRedirection
                                anchors.fill: parent
                                
                                onClicked: loadPage(forgotPasswordPage)
                            }
                        }
                    }
                    
                    MButton 
                    {
                        id: loginButton
                        Layout.fillWidth: true
                        Layout.preferredHeight: 40
                        Layout.topMargin: 42
                        borderWidth: 0
                        backgroundColor: Style.colorBasePurple
                        fontSize: 12
                        textColor: Style.colorFocusedButtonText
                        fontWeight: Font.Bold
                        text: "Login"
                        
                        onClicked: internal.login()
                        
                        Keys.onPressed: 
                            (event) =>
                            {
                                if(event.key === Qt.Key_Up) rememberMeCheckBox.giveFocus();
                                else if(event.key === Qt.Key_Return) internal.login();
                            }
                    }
                }
            }
            
            Label
            {
                id: registerLinkLabel
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 14
                text: "Don't have an account? Register"
                font.pointSize: 10.5
                color: Style.colorBasePurple
                
                MouseArea
                {
                    anchors.fill: parent
                    onClicked: loadPage(registerPage);
                }
            }
            
        }
        
    }
    
    MLoginFailedPopup
    {
        id: loginFailedDialog
        x: Math.round(root.width / 2 - implicitWidth / 2)
        y: Math.round(root.height / 2 - implicitHeight / 2) - 75
        
        onOpenedChanged: if(opened) loginFailedDialog.giveFocus()
    }
    
    QtObject
    {
        id: internal
        
        function login()
        {
            // No-credential login skip - For development only
            if(emailInput.text === "" && passwordInput.text === "")
            {
                loadPage(homePage, sidebar.homeItem, false);
                return;
            }
            
            AuthController.loginUser(emailInput.text, passwordInput.text);   
        }
        
        function processLoginResult(success)
        {
            if(success)
            {
                UserController.loadUser();
            }
            else
            {
                // TODO: Login failed
            }
        }
    }
}