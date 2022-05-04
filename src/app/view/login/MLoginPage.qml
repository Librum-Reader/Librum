import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import CustomComponents

Page
{
    id: root
    background: Rectangle
    {
        anchors.fill: parent
        color: properties.loginWindowBackground
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
                color: properties.loginContentBackground
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
                    color: properties.colorBaseText
                    font.bold: true
                    font.pointSize: 26
                    font.family: properties.defaultFontFamily
                }
                
                Label
                {
                    id: loginText
                    Layout.topMargin: 4
                    Layout.alignment: Qt.AlignHCenter
                    text: "Log into your account"
                    color: properties.colorLightText2
                    font.pointSize: 13
                    font.family: properties.defaultFontFamily
                }
                
                
                MLabeledInputBox
                {
                    id: emailInput
                    Layout.fillWidth: true
                    Layout.topMargin: 32
                    placeholderContent: "kaidoe@gmail.com"
                    placeholderColor: properties.colorLightText
                    headerText: "Email"
                    autoFocus: true
                }
                
                MLabeledInputBox
                {
                    id: passwordInput
                    Layout.fillWidth: true
                    Layout.topMargin: 22
                    headerText: "Password"
                    addImageToRight: true
                    imagePath: properties.iconEyeOn
                    toggledImagePath: properties.iconEyeOff
                }
                
                MLoginOptions
                {
                    id: loginOption
                    Layout.fillWidth: true
                    Layout.topMargin: 24
                }
                
                MButton 
                {
                    id: loginButton
                    Layout.fillWidth: true
                    Layout.preferredHeight: 40
                    Layout.topMargin: 42
                    backgroundColor: properties.colorBasePurple
                    fontSize: 12
                    fontColor: properties.colorBrightText
                    fontBold: true
                    textContent: "Login"
                    
                    onClicked:
                    {
                        
                        loadPage("HomePage");
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
            color: properties.colorBasePurple
            
            MouseArea
            {
                anchors.fill: parent
                onClicked: loadPage("RegisterPage");
            }
        }
    }
}
