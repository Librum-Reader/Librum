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
                        
                        
                        RowLayout
                        {
                            id: nameInputLayout
                            Layout.preferredWidth: parent.width
                            Layout.preferredHeight: firstNameInput.implicitHeight
                            spacing: 28
                            
                            
                            MLabeledInputBox
                            {
                                id: firstNameInput
                                Layout.preferredWidth: parent.width / 2 - nameInputLayout.spacing / 2
                                Layout.preferredHeight: parent.height
                                headerText: 'First name'
                                placeholderContent: "Kai"
                                placeholderColor: properties.colorLightText
                                
                                Keys.onPressed: 
                                    (event) =>
                                    {
                                        if(event.key === Qt.Key_Return || event.key === Qt.Key_Down)
                                        {
                                            lastNameInput.giveFocus();
                                        }
                                    }
                            }
                            
                            MLabeledInputBox
                            {
                                id: lastNameInput
                                Layout.preferredWidth: parent.width / 2 - nameInputLayout.spacing / 2
                                Layout.preferredHeight: parent.height
                                headerText: "Last name"
                                placeholderContent: "Doe"
                                placeholderColor: properties.colorLightText
                                
                                Keys.onPressed: 
                                    (event) =>
                                    {
                                        if(event.key === Qt.Key_Return || event.key === Qt.Key_Down)
                                        {
                                            emailInput.giveFocus();
                                        }
                                        else if(event.key === Qt.Key_Up)
                                        {
                                            firstNameInput.giveFocus();
                                        }
                                    }
                            }
                        }
                        
                        MLabeledInputBox 
                        {
                            id: emailInput
                            Layout.fillWidth: true
                            Layout.topMargin: 19
                            headerText: 'Email'
                            placeholderContent: "kaidoe@gmail.com"
                            placeholderColor: properties.colorLightText
                            
                            Keys.onPressed: 
                                (event) =>
                                {
                                    if(event.key === Qt.Key_Return || event.key === Qt.Key_Down)
                                    {
                                        passwordInput.giveFocus();
                                    }
                                    else if(event.key === Qt.Key_Up)
                                    {
                                        lastNameInput.giveFocus();
                                    }
                                }
                        }
                        
                        MLabeledInputBox 
                        {
                            id: passwordInput
                            Layout.fillWidth: true
                            Layout.topMargin: 16
                            headerText: 'Password'
                            placeholderColor: properties.colorLightText
                            imagePath: properties.iconEyeOn
                            toggledImagePath: properties.iconEyeOff
                            
                            Keys.onPressed: 
                                (event) =>
                                {
                                    if(event.key === Qt.Key_Return || event.key === Qt.Key_Down)
                                    {
                                        passwordConfirmationInput.giveFocus();
                                    }
                                    else if(event.key === Qt.Key_Up)
                                    {
                                        emailInput.giveFocus();
                                    }
                                }
                        }
                        
                        MLabeledInputBox 
                        {
                            id: passwordConfirmationInput
                            Layout.fillWidth: true
                            Layout.topMargin: 16
                            headerText: 'Confirmation password'
                            placeholderColor: properties.colorLightText
                            imagePath: properties.iconEyeOn
                            toggledImagePath: properties.iconEyeOff
                            
                            Keys.onPressed: 
                                (event) =>
                                {
                                    if(event.key === Qt.Key_Return || event.key === Qt.Key_Down)
                                    {
                                        keepMeUpdated.giveFocus();
                                    }
                                    else if(event.key === Qt.Key_Up)
                                    {
                                        passwordInput.giveFocus();
                                    }
                                }
                        }
                        
                        MKeepMeUpdated
                        {
                            id: keepMeUpdated
                            Layout.fillWidth: true
                            Layout.topMargin: 28
                            
                            onKeyUp: passwordConfirmationInput.giveFocus();
                            onKeyDown: acceptPolicy.giveFocus();
                        }
                        
                        MAcceptPolicy
                        {
                            id: acceptPolicy
                            Layout.fillWidth: true
                            Layout.topMargin: 32
                            
                            onKeyUp: keepMeUpdated.giveFocus();
                            onKeyDown: loginButton.giveFocus();
                        }
                        
                        MButton 
                        {
                            id: loginButton
                            Layout.fillWidth: true
                            Layout.preferredHeight: 40
                            Layout.topMargin: 44
                            borderWidth: 0
                            backgroundColor: properties.colorBasePurple
                            fontSize: 12                            
                            fontColor: properties.colorBrightText
                            fontWeight: Font.Bold
                            text: "Let's start"
                            
                            onClicked: loadPage("LoginPage");
                            
                            Keys.onPressed: 
                                (event) =>
                                {
                                    if(event.key === Qt.Key_Return)
                                    {
                                        loadPage("LoginPage");
                                    }
                                    else if(event.key === Qt.Key_Up)
                                    {
                                        acceptPolicy.giveFocus();
                                    }
                                }
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
    
    Component.onCompleted: firstNameInput.giveFocus();
}