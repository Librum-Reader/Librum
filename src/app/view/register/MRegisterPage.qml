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
    
    property int baseHeight : 770
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
                    Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
                    text: "Welcome!"
                    color: properties.colorBaseText
                    font.bold: true
                    font.pointSize: 26
                    font.family: properties.defaultFontFamily
                }
                
                Label
                {
                    id: accountStorageText
                    Layout.topMargin: 6
                    Layout.alignment: Qt.AlignHCenter
                    Layout.preferredWidth: 500
                    text: "Your credentials are only used to authenticate yourself. " + 
                          "Everything will be stored in a secure database."
                    horizontalAlignment: Text.AlignHCenter
                    wrapMode: "WordWrap"
                    color: properties.colorLightText2
                    font.pointSize: 13
                    font.family: properties.defaultFontFamily
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
                        
                        RowLayout
                        {
                            id: nameInputRow
                            spacing: 28
                            Layout.alignment: Qt.AlignHCenter
                            Layout.fillWidth: true
                            
                            MLabeledInputBox
                            {
                                id: firstNameInput
                                Layout.preferredWidth: 186
                                Layout.preferredHeight: 50
                                Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
                                headerText: 'First name'
                                placeholderContent: "Kai"
                                placeholderColor: properties.colorLightText
                            }
                            
                            MLabeledInputBox
                            {
                                id: lastNameInput
                                Layout.preferredWidth: 186
                                Layout.preferredHeight: 50
                                Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
                                headerText: "Last name"
                                placeholderContent: "Doe"
                                placeholderColor: properties.colorLightText
                            }
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
                            placeholderContent: ""
                            placeholderColor: properties.colorLightText
                            addImageToRight: true
                            image: "/resources/images/eye.svg"
                            toggleImage: "/resources/images/eye-off.svg"
                        }
                        
                        MLabeledInputBox 
                        {
                            id: passwordConfirmationInput
                            Layout.fillWidth: true
                            Layout.topMargin: 16
                            headerText: 'Confirmation password'
                            placeholderContent: ""
                            placeholderColor: properties.colorLightText
                            addImageToRight: true
                            image: "/resources/images/eye.svg"
                            toggleImage: "/resources/images/eye-off.svg"
                        }
                        
                        RowLayout
                        {
                            id: keepMeUpdatedRow
                            Layout.fillWidth: true
                            Layout.topMargin: 28
                            spacing: 4
                            
                            MCheckBox
                            {
                                id: updatesCheckBox
                                checkboxHeight: 20
                                checkboxWidth: 20
                                imageSource: "/resources/images/check.svg"
                                borderColor: properties.colorDarkBorder
                                borderRadius: 4
                            }
                            
                            Item
                            {
                                id: keepMeUpdatedText
                                Layout.preferredHeight: keepMeUpdatedTextFirst.implicitHeight
                                Layout.fillWidth: true
                                Layout.leftMargin: 6
                                
                                Column
                                {
                                    spacing: 2
                                    
                                    Label
                                    {
                                        id: keepMeUpdatedTextFirst
                                        text: "Keep me updated about the new features and"
                                        wrapMode: Text.WordWrap
                                        font.pointSize: 11
                                        color: properties.colorMediumText
                                    }
                                    
                                    Label
                                    {
                                        id: keepMeUpdatedTextSecond
                                        text: "upcoming improvements."
                                        wrapMode: Text.WordWrap
                                        font.pointSize: 11
                                        color: properties.colorMediumText
                                    }
                                }
                            }
                        }
                    
                        RowLayout
                        {
                            id: iAcceptRow
                            Layout.fillWidth: true
                            Layout.topMargin: 32
                            spacing: 4
                            
                            MCheckBox
                            {
                                id: iAcceptCheckBox
                                checkboxHeight: 20
                                checkboxWidth: 20
                                imageSource: "/resources/images/check.svg"
                                borderColor: properties.colorDarkBorder
                                borderRadius: 4
                            }
                            
                            Label
                            {
                                id: iAcceptTextFirst
                                Layout.fillWidth: true
                                Layout.leftMargin: 6
                                text: 'I accept the <font color=' + properties.colorBasePurple + '>terms</font> 
                                       and the <font color=' + properties.colorBasePurple + '>privacy policy.</font>'
                                textFormat: Text.RichText
                                wrapMode: Text.WordWrap
                                font.pointSize: 11
                                color: properties.colorMediumText
                            }
                        }
                        
                        MButton 
                        {
                            id: loginButton
                            Layout.preferredHeight: 40
                            Layout.preferredWidth: parent.width
                            Layout.topMargin: 40
                            backgroundColor: properties.colorBasePurple
                            fontColor: properties.colorBrightText
                            fontBold: true
                            textContent: "Let's start"
                            
                            onClicked:
                            {
                                loadPage("LoginPage");
                            }
                        }
                    }
                }
                
            }
        }
        
        Label
        {
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