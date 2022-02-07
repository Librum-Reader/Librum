import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../components"

Page
{
    id: root
    background: Rectangle
    {
        anchors.fill: parent
        color: properties.loginWindowBackground
    }
    
    
    property int baseHeight : 550
    property int baseWidth : 542
    
    ColumnLayout
    {
        anchors.centerIn: parent
        width: root.baseWidth
        
        Rectangle
        {
            id: backgroundRect
            Layout.preferredHeight: root.baseHeight
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignTop
            radius: 4
            color: properties.loginContentBackground
            
            ColumnLayout
            {
                id: inContainerLayout
                anchors
                {
                    top:   parent.top
                    left:  parent.left
                    right: parent.right
                }
                
                MLogo
                {
                    id: logo
                    Layout.topMargin: 48
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
                }
                
                Label
                {
                    id: welcomeText
                    Layout.topMargin: 24
                    Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
                    text: "Welcome back!"
                    color: properties.colorBaseText
                    font.bold: true
                    font.pointSize: 22
                    font.family: properties.defaultFontFamily
                }
                
                Label
                {
                    id: loginText
                    Layout.topMargin: 4
                    Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
                    text: "Log into your account"
                    color: properties.colorLightText2
                    font.pointSize: 12
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
                    }
                    
                    MLabeledInputBox
                    {
                        id: passwordInput
                        Layout.preferredWidth: parent.width
                        Layout.topMargin: 22
                        placeholderContent: ""
                        headerText: "Password"
                        addImageToRight: true
                        image: "/resources/images/eye.svg"
                        toggleImage: "/resources/images/eye-off.svg"
                    }
                    
                    RowLayout
                    {
                        id: rememberMeRow
                        Layout.topMargin: 28
                        
                        MCheckBox
                        {
                            id: rememberMeCheckBox
                            checkboxHeight: 20
                            checkboxWidth: 20
                            imageSource: "/resources/images/check.svg"
                            borderColor: properties.colorDarkBorder
                            borderRadius: 4
                        }
                        
                        Label
                        {
                            id: rememberMeText
                            text: "Remember me"
                            Layout.alignment: Qt.AlignVCenter
                            Layout.leftMargin: 6
                            font.pointSize: 11
                            color: properties.colorMediumText
                        }
                        
                        Item {
                            Layout.preferredWidth: 131
                            Layout.preferredHeight: 1
                        }
                        
                        Label
                        {
                            id: forgotPasswordLabel
                            text: "Forgot password?"
                            Layout.alignment: Qt.AlignVCenter
                            Layout.leftMargin: 3
                            font.pointSize: 10
                            color: properties.colorBasePurple
                        }
                    }
                    
                    MButton 
                    {
                        id: loginButton
                        buttonHeight: 40
                        buttonWidth: parent.width
                        Layout.topMargin: 32
                        backgroundColor: properties.colorBasePurple
                        fontColor: properties.colorBrightText
                        fontBold: true
                        textContent: "Login"
                        
                        onClicked:
                        {
                            loadPage("LibraryPage");
                        }
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