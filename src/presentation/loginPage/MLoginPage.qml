import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import CustomComponents
import Librum.style
import Librum.icons

Page
{
    id: root
    background: Rectangle
    {
        anchors.fill: parent
        color: Style.loginWindowBackground
    }
    
    Shortcut
    {
        sequence: "Ctrl+Return"
        onActivated: loginButton.buttonTriggeredAction()
    }
    
    ColumnLayout
    {
        id: layout
        width: 544
        anchors.centerIn: parent
        
        
        Pane
        {
            id: backgroundRect
            Layout.fillWidth: true
            topPadding: 48
            horizontalPadding: 71
            bottomPadding: 42
            background: Rectangle
            {
                color: Style.loginContentBackground
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
                    font.family: Style.defaultFontFamily
                }
                
                Label
                {
                    id: loginText
                    Layout.topMargin: 4
                    Layout.alignment: Qt.AlignHCenter
                    text: "Log into your account"
                    color: Style.colorLightText2
                    font.pointSize: 13
                    font.family: Style.defaultFontFamily
                }
                
                
                MLabeledInputBox
                {
                    id: emailInput
                    Layout.fillWidth: true
                    Layout.topMargin: 32
                    placeholderContent: "kaidoe@gmail.com"
                    placeholderColor: Style.colorLightText
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
                    imagePath: Icons.eyeOn
                    toggledImagePath: Icons.eyeOff
                    
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
                                    actionOnClicked();
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
                        Layout.fillWidth: true
                    }
                    
                    Label
                    {
                        id: forgotPasswordLabel
                        text: "Forgot password?"
                        Layout.alignment: Qt.AlignVCenter
                        Layout.leftMargin: 3
                        font.pointSize: 10
                        color: Style.colorBasePurple
                        
                        MouseArea
                        {
                            anchors.fill: parent
                            
                            onClicked:
                            {
                                loadPage(forgotPasswordPage);
                            }
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
                    fontColor: Style.colorBrightText
                    fontWeight: Font.Bold
                    text: "Login"
                    
                    onClicked: loadPage(homePage, sidebar.homeItem, false);
                    
                    Keys.onPressed: 
                        (event) =>
                        {
                            if(event.key === Qt.Key_Up) rememberMeCheckBox.giveFocus();
                            else if(event.key === Qt.Key_Return) buttonTriggeredAction();
                        }
                    
                    
                    function buttonTriggeredAction()
                    {
                        loadPage(homePage, sidebar.homeItem, false);
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
            font.pointSize: 10
            color: Style.colorBasePurple
            
            MouseArea
            {
                anchors.fill: parent
                onClicked: loadPage(registerPage);
            }
        }
    }
    
    Component.onCompleted: emailInput.giveFocus();
}