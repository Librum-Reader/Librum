import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../components"

Page
{
    id: root
    height: parent.height
    width: parent.width
    background: Rectangle
    {
        anchors.fill: parent
        color: properties.loginWindowBackground
    }
    
    property int baseHeight : 550
    property int baseWidth : 542
        
    ColumnLayout
    {
        id: layout
        anchors.centerIn: parent
        width: root.baseWidth
        
        Rectangle
        {
            id: containerRect
            height: root.baseHeight
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignTop
            radius: 4
            color: properties.loginContentBackground
            
            ColumnLayout
            {
                id: inRectLayout
                anchors
                {
                    top:   parent.top
                    left:  parent.left
                    right: parent.right
                }
                
                anchors.margins: 15
                anchors.topMargin: 48
                
                Rectangle
                {
                    id: logo
                    height: 72
                    width: 72
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
                    color: properties.colorBasePurple
                    radius: 8
                    antialiasing: true
                    
                    Rectangle
                    {
                        anchors.centerIn: parent
                        width: parent.width / 2
                        height: parent.height / 2
                        radius: width / 4
                        color: properties.loginContentBackground
                    }
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
                
                Item
                {
                    id: inputGroup
                    width: 400
                    Layout.topMargin: 28
                    Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
                    
                    ColumnLayout
                    {
                        width: parent.width
                        spacing: 0
                        
                        MLabeledInputBox 
                        {
                            id: emailInput
                            placeholderContent: "kaidoe@gmail.com"
                            placeholderColor: properties.colorLightText
                            headerText: "Email"
                        }
                        
                        MLabeledInputBox
                        {
                            id: passwordInput
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
                                borderColor: "gray"
                                borderRadius: 4
                            }
                            
                            Label
                            {
                                text: "Remember me"
                                Layout.alignment: Qt.AlignVCenter
                                Layout.leftMargin: 3
                                font.pointSize: 11
                                color: "#505057"
                            }
                            
                            Item {
                                width: 131
                                height: 1
                            }
                            
                            Label
                            {
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
                            fontColor: "white"
                            fontBold: true
                            textContent: "Login"
                        }
                    }
                }
            }
        }
        
        Label
        {
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
