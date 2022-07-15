import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import CustomComponents
import Librum.style
import Librum.icons
import Librum.globals

MFlickWrapper
{
    id: root
    contentHeight: Window.height < layout.implicitHeight ? layout.implicitHeight : Window.height
    
    Page
    {
        id: page
        anchors.fill: parent
        bottomPadding: 16
        background: Rectangle
        {
            anchors.fill: parent
            color: Style.loginWindowBackground
        }
        
        
        Shortcut
        {
            sequence: "Ctrl+Return"
            onActivated: registerButton.buttonTriggeredAction()
        }
        
        
        ColumnLayout
        {
            id: layout
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
                        Layout.topMargin: 24
                        Layout.alignment: Qt.AlignHCenter
                        color: Style.colorBaseText
                        text: "Welcome!"
                        font.bold: true
                        font.pointSize: 26
                        font.family: Style.defaultFontFamily
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
                        font.family: Style.defaultFontFamily
                        color: Style.colorLightText2
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
                                spacing: 28
                                
                                
                                MLabeledInputBox
                                {
                                    id: firstNameInput
                                    Layout.fillWidth: true
                                    headerText: 'First name'
                                    placeholderContent: "Kai"
                                    placeholderColor: Style.colorLightText
                                    
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
                                    Layout.fillWidth: true
                                    headerText: "Last name"
                                    placeholderContent: "Doe"
                                    placeholderColor: Style.colorLightText
                                    
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
                                placeholderColor: Style.colorLightText
                                
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
                                placeholderColor: Style.colorLightText
                                imagePath: Icons.eyeOn
                                toggledImagePath: Icons.eyeOff
                                
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
                                placeholderColor: Style.colorLightText
                                imagePath: Icons.eyeOn
                                toggledImagePath: Icons.eyeOff
                                
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
                                onKeyDown: registerButton.giveFocus();
                            }
                            
                            MButton 
                            {
                                id: registerButton
                                Layout.fillWidth: true
                                Layout.preferredHeight: 40
                                Layout.topMargin: 44
                                borderWidth: 0
                                backgroundColor: Style.colorBasePurple
                                fontSize: 12                            
                                fontColor: Style.colorBrightText
                                fontWeight: Font.Bold
                                text: "Let's start"
                                
                                onClicked:
                                {
                                    page.saveEntries();
                                    loadPage(loginPage);
                                }
                                
                                Keys.onPressed: 
                                    (event) =>
                                    {
                                        if(event.key === Qt.Key_Return) buttonTriggeredAction();
                                        else if(event.key === Qt.Key_Up) acceptPolicy.giveFocus();
                                    }
                                
                                
                                function buttonTriggeredAction()
                                {
                                    page.saveEntries();
                                    loadPage(loginPage);
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
                color: Style.colorBasePurple
                
                
                MouseArea
                {
                    anchors.fill: parent
                    onClicked: loadPage(loginPage);
                }
            }
        }
        
        Component.onCompleted: firstNameInput.giveFocus();
        
        
        function saveEntries()
        {
            if(firstNameInput.text.length >= 2)
                Globals.firstName = firstNameInput.text;
            if(lastNameInput.text.length >= 2)
                Globals.lastName = lastNameInput.text;
            if(emailInput.text.length >= 5)
                Globals.email = emailInput.text;
        }
    }
}