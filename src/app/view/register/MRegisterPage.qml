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
        width: 543
        anchors.centerIn: parent
        
        
        Pane
        {
            id: backgroundRect
            Layout.fillWidth: true
            topPadding: 48
            bottomPadding: 36
            horizontalPadding: 52
            background: Rectangle
            {
                color: properties.loginContentBackground
                radius: 4
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
                    Layout.topMargin: 24
                    Layout.alignment: Qt.AlignHCenter
                    color: properties.colorBaseText
                    text: "Welcome!"
                    font.bold: true
                    font.pointSize: 26
                    font.family: properties.defaultFontFamily
                }
                
                Label
                {
                    id: accountStorageText
                    Layout.fillWidth: true
                    Layout.topMargin: 8
                    Layout.alignment: Qt.AlignHCenter
                    horizontalAlignment: Text.AlignHCenter
                    text: "Your credentials are only used to authenticate yourself. " + 
                          "Everything will be stored in a secure database."
                    font.pointSize: 13
                    font.family: properties.defaultFontFamily
                    color: properties.colorLightText2
                    wrapMode: "WordWrap"
                }
                
                Pane
                {
                    id: inputGroup
                    Layout.fillWidth: true
                    Layout.topMargin: 38
                    Layout.alignment: Qt.AlignHCenter
                    verticalPadding: 0
                    horizontalPadding: 20
                    background: Rectangle
                    {
                        color: "transparent"
                    }
                    
                    
                    ColumnLayout
                    {
                        id: inputLayout
                        width: parent.width
                        spacing: 0
                        
                        
                        MNameInput
                        {
                            id: nameInput
                            Layout.fillWidth: true
                        }
                        
                        MLabeledInputBox 
                        {
                            id: emailInput
                            Layout.fillWidth: true
                            Layout.topMargin: 26
                            headerText: 'Email'
                            placeholderContent: "kaidoe@gmail.com"
                            placeholderColor: properties.colorLightText
                        }
                        
                        MLabeledInputBox 
                        {
                            id: passwordInput
                            Layout.fillWidth: true
                            Layout.topMargin: 16
                            headerText: 'Password'
                            placeholderColor: properties.colorLightText
                            addImageToRight: true
                            imagePath: properties.iconEyeOn
                            toggledImagePath: properties.iconEyeOff
                        }
                        
                        MLabeledInputBox 
                        {
                            id: passwordConfirmationInput
                            Layout.fillWidth: true
                            Layout.topMargin: 16
                            headerText: 'Confirmation password'
                            placeholderColor: properties.colorLightText
                            addImageToRight: true
                            imagePath: properties.iconEyeOn
                            toggledImagePath: properties.iconEyeOff
                        }
                        
                        MKeepMeUpdated
                        {
                            id: keepMeUpdated
                            Layout.topMargin: 28
                        }
                        
                        MAcceptPolicy
                        {
                            id: acceptPolicy
                            Layout.topMargin: 32                            
                        }
                        
                        MButton 
                        {
                            id: loginButton
                            Layout.fillWidth: true
                            Layout.preferredHeight: 40
                            Layout.topMargin: 44
                            backgroundColor: properties.colorBasePurple
                            fontSize: 12                            
                            fontColor: properties.colorBrightText
                            fontBold: true
                            textContent: "Let's start"
                            
                            onClicked: loadPage("LoginPage");
                        }
                    }
                }
                
            }
        }
        
        Label
        {
            id: loginRedirectionLabel
            Layout.alignment: Qt.AlignHCenter
            Layout.topMargin: 14
            text: "Already have an account? Login"
            font.pointSize: 10
            color: properties.colorBasePurple
            
            
            MouseArea
            {
                anchors.fill: parent
                onClicked: loadPage("LoginPage");
            }
        }
    }
}