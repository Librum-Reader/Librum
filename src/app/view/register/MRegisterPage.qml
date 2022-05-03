import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import CustomComponents

Page
{
    id: root
    readonly property int containerWidth : 542
    readonly property int containerHeight : 780

    background: Rectangle
    {
        anchors.fill: parent
        color: properties.loginWindowBackground
    }
    
    ColumnLayout
    {
        anchors.centerIn: parent
        width: root.containerWidth
        
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
                Layout.preferredWidth: parent.width
                anchors.topMargin: 48
                anchors
                {
                    top:   parent.top
                    left:  parent.left
                    right: parent.right
                }
                
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
                    Layout.preferredWidth: 500
                    Layout.alignment: Qt.AlignHCenter
                    horizontalAlignment: Text.AlignHCenter
                    Layout.topMargin: 6
                    text: "Your credentials are only used to authenticate yourself. " + 
                          "Everything will be stored in a secure database."
                    font.pointSize: 13
                    font.family: properties.defaultFontFamily
                    color: properties.colorLightText2
                    wrapMode: "WordWrap"
                }
                
                Item
                {
                    id: inputGroup
                    Layout.preferredWidth: 400
                    Layout.alignment: Qt.AlignHCenter
                    Layout.topMargin: 36
                    
                    
                    ColumnLayout
                    {
                        id: inputLayout
                        width: parent.width
                        spacing: 0
                        
                        MNameInput
                        {
                            id: nameInput
                            Layout.alignment: Qt.AlignHCenter
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
                            Layout.fillWidth: true
                            Layout.topMargin: 28
                        }
                        
                        MAcceptPolicy
                        {
                            id: acceptPolicy
                            Layout.fillWidth: true
                            Layout.topMargin: 32
                        }
                        
                        MButton 
                        {
                            id: loginButton
                            Layout.preferredHeight: 40
                            Layout.preferredWidth: parent.width
                            Layout.topMargin: 32
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
