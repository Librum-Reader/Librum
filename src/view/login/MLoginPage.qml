import QtQuick
import QtQuick.Controls
import QtQuick.Layouts


Page
{
    id: root
    width: 542
    height: 550
        
    ColumnLayout
    {
        id: layout
        width: parent.width
        spacing: 0
        
        Rectangle
        {
            id: containerRect
            height: root.height
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignTop
            radius: 4
            color: "white"
            
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
                    width:  72
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
                        color: "#F6F6F6"
                    }
                }
                
                Label
                {
                    id: welcomeText
                    Layout.topMargin: 24
                    Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
                    text: "Welcome back!"
                    color: "#32324D"
                    font.bold: true
                    font.pointSize: 22
                    font.family: "Droid Sans Fallback"
                }
                
                Label
                {
                    id: loginText
                    Layout.topMargin: 6
                    Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
                    text: "Log into your account"
                    color: "#A5A5BA"
                    font.pointSize: 12
                    font.family: "Droid Sans Fallback"
                }
                
                Item
                {
                    id: inputGroup
                    width: 400
                    Layout.topMargin: 30
                    Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
                    
                    ColumnLayout
                    {
                        width: parent.width
                        spacing: 0
                        
                        MLabeledInputBox 
                        {
                            id: emailInput
                            placeholderContent: "kaidoe@gmail.com"
                            headerText: "Email"
                        }
                        
                        MLabeledInputBox
                        {
                            id: passwordInput
                            Layout.topMargin: 15
                            placeholderContent: ""
                            headerText: "Password"
                            addImageToRight: true
                            image: "/resources/images/eye.svg"
                            toggleImage: "/resources/images/eye-off.svg"
                        }
                        
                        RowLayout
                        {
                            Layout.topMargin: 28
                            
                            MCheckBox
                            {
                                width:  18
                                height: 18
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
                                width: 175
                                height: 1
                            }
                            
                            Label
                            {
                                text: "Forgot password?"
                                Layout.alignment: Qt.AlignVCenter
                                Layout.leftMargin: 3
                                font.pointSize: 9
                                color: "#57575E"
                            }
                        }
                        
                        MLoginButton 
                        {
                            id: loginButton
                            inputField1: emailInput
                            inputField2: passwordInput
                            color: properties.colorBasePurple
                            
                            onClearInputs:
                            {
                                passwordInput.clearInput();
                                emailInput.clearInput();
                            }
                            
                            onClicked:
                            {
                                loadPage("RegisterPage");
                            }
                        }
                    }
                }
            }
        }
        
        Label
        {
            Layout.alignment: Qt.AlignHCenter
            Layout.topMargin: 18
            text: "Don't have an account? Register"
            font.pointSize: 9
            color: properties.colorBasePurple
        }
    }
}
