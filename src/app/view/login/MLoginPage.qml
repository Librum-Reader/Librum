import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import CustomComponents

Page
{
    id: root
    property int containerWidth : 542
    property int containerHeight : 550
    
    background: Rectangle
    {
        anchors.fill: parent
        color: properties.loginWindowBackground
    }
    
    
    ColumnLayout
    {
        id: layout
        width: root.containerWidth
        anchors.centerIn: parent
        
        Rectangle
        {
            id: backgroundRect
            Layout.fillWidth: true
            Layout.preferredHeight: root.containerHeight
            Layout.alignment: Qt.AlignTop
            color: properties.loginContentBackground
            radius: 4
            
            ColumnLayout
            {
                id: inRectLayout
                width: parent.width
                
                MLogo
                {
                    id: logo
                    Layout.alignment: Qt.AlignHCenter
                    Layout.topMargin: 48
                }
                
                Label
                {
                    id: welcomeText
                    Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
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
                    Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
                    text: "Log into your account"
                    color: properties.colorLightText2
                    font.pointSize: 13
                    font.family: properties.defaultFontFamily
                }
                
                ColumnLayout
                {
                    id: inputColumn
                    Layout.preferredWidth: 400
                    Layout.alignment: Qt.AlignHCenter
                    Layout.topMargin: 28
                    spacing: 0
                    
                    MLabeledInputBox
                    {
                        id: emailInput
                        Layout.preferredWidth: parent.width
                        placeholderContent: "kaidoe@gmail.com"
                        placeholderColor: properties.colorLightText
                        headerText: "Email"
                        autoFocus: true
                    }
                    
                    MLabeledInputBox
                    {
                        id: passwordInput
                        Layout.preferredWidth: parent.width
                        Layout.topMargin: 22
                        placeholderContent: ""
                        headerText: "Password"
                        addImageToRight: true
                        imagePath: "/resources/images/eye.svg"
                        toggledImagePath: "/resources/images/eye-off.svg"
                    }
                    
                    MLoginOptions
                    {
                        id: loginOption
                        Layout.topMargin: 28
                    }
                    
                    MButton 
                    {
                        id: loginButton
                        Layout.preferredWidth: parent.width
                        Layout.preferredHeight: 40
                        Layout.topMargin: 32
                        backgroundColor: properties.colorBasePurple
                        fontSize: 12
                        fontColor: properties.colorBrightText
                        fontBold: true
                        textContent: "Login"
                        
                        onClicked: loadPage("HomePage");
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