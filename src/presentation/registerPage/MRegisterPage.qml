import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15
import CustomComponents 1.0
import Librum.style 1.0
import Librum.icons 1.0
import Librum.globals 1.0
import Librum.controllers 1.0


MFlickWrapper
{
    id: root
    contentHeight: Window.height < layout.implicitHeight ? layout.implicitHeight : Window.height
    
    // Passing the focus to firstNameInput on Component.onCompleted() causes it
    // to pass controll back to root for some reason, this fixes the focus problem
    onActiveFocusChanged: if(activeFocus) firstNameInput.giveFocus()
    
    Page
    {
        id: page
        anchors.fill: parent
        bottomPadding: 16
        background: Rectangle { anchors.fill: parent; color: Style.colorPageBackground }
        
        
        Shortcut
        {
            id: registerShortcut
            sequence: "Ctrl+Return"
            onActivated: internal.registerUser()
        }
        
        Connections
        {
            id: registrationFinished
            target: AuthController
            function onRegistrationFinished(success, reason) 
            {
                internal.proccessRegistrationResult(success, reason);
            }
        }
        
        
        ColumnLayout
        {
            id: layout
            width: 543
            anchors.centerIn: parent
            
            
            Pane
            {
                id: background
                Layout.fillWidth: true
                topPadding: 48
                bottomPadding: 36
                horizontalPadding: 52
                background: Rectangle { color: Style.colorContainerBackground; radius: 5 }
                
                
                ColumnLayout
                {
                    id: contentLayout
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
                        color: Style.colorText
                        text: "Welcome!"
                        font.bold: true
                        font.pointSize: 26
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
                        color: Style.colorSubtitle
                        wrapMode: Text.WordWrap
                    }
                    
                    Pane
                    {
                        id: credentialInputContainer
                        Layout.fillWidth: true
                        Layout.topMargin: 38
                        Layout.alignment: Qt.AlignHCenter
                        verticalPadding: 0
                        horizontalPadding: 20
                        background: Rectangle { color: "transparent" }
                        
                        
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
                                    placeholderColor: Style.colorPlaceholderText
                                    
                                    Keys.onPressed: (event) => internal.moveFocusToNextInput(event, 
                                                                                             null, 
                                                                                             lastNameInput)
                                }
                                
                                MLabeledInputBox
                                {
                                    id: lastNameInput
                                    Layout.fillWidth: true
                                    headerText: "Last name"
                                    placeholderContent: "Doe"
                                    placeholderColor: Style.colorPlaceholderText
                                    
                                    Keys.onPressed: (event) => internal.moveFocusToNextInput(event, 
                                                                                             firstNameInput,
                                                                                             emailInput)
                                }
                            }
                            
                            MLabeledInputBox 
                            {
                                id: emailInput
                                Layout.fillWidth: true
                                Layout.topMargin: 19
                                headerText: 'Email'
                                placeholderContent: "kaidoe@gmail.com"
                                placeholderColor: Style.colorPlaceholderText
                                Keys.onPressed: (event) => internal.moveFocusToNextInput(event,
                                                                                         lastNameInput,
                                                                                         passwordInput)
                            }
                            
                            MLabeledInputBox 
                            {
                                id: passwordInput
                                Layout.fillWidth: true
                                Layout.topMargin: 16
                                headerText: 'Password'
                                placeholderColor: Style.colorPlaceholderText
                                image: Icons.eyeOn
                                toggledImage: Icons.eyeOff
                                
                                Keys.onPressed: (event) => internal.moveFocusToNextInput(event, 
                                                                                         emailInput,
                                                                                         passwordConfirmationInput)
                                
                                onEdited: passwordConfirmationInput.clearError()
                            }
                            
                            MLabeledInputBox 
                            {
                                id: passwordConfirmationInput
                                Layout.fillWidth: true
                                Layout.topMargin: 16
                                headerText: 'Confirmation password'
                                placeholderColor: Style.colorPlaceholderText
                                image: Icons.eyeOn
                                toggledImage: Icons.eyeOff
                                
                                Keys.onPressed: (event) => internal.moveFocusToNextInput(event, 
                                                                                         passwordInput,
                                                                                         keepMeUpdated)
                                
                                onEdited: passwordInput.clearError()
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
                                textColor: Style.colorFocusedButtonText
                                fontWeight: Font.Bold
                                text: "Let's start"
                                
                                onClicked: internal.registerUser()
                                
                                Keys.onReturnPressed: internal.registerUser()
                                Keys.onUpPressed: acceptPolicy.giveFocus()
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
                font.pointSize: 10.5
                color: Style.colorBasePurple
                
                
                MouseArea
                {
                    anchors.fill: parent
                    
                    onClicked: loadPage(loginPage);
                }
            }
        }
        
        Component.onCompleted: firstNameInput.giveFocus();
    }
    
    QtObject
    {
        id: internal
        
        
        function registerUser()
        {
            if(!passwordIsValid() || !policyIsAccepted())
                return;
            
            AuthController.registerUser(firstNameInput.text, lastNameInput.text,
                                        emailInput.text, passwordInput.text,
                                        keepMeUpdated.checked);
        }
        
        function passwordIsValid()
        {
            if(passwordInput.text == passwordConfirmationInput.text)
                return true;
            
            passwordConfirmationInput.errorText = "Passwords don't match."
            passwordConfirmationInput.setError();
            return false;
        }
        
        function policyIsAccepted()
        {
            if(acceptPolicy.activated)
                return true;
            
            acceptPolicy.setError();
            return false;
        }
        
        function proccessRegistrationResult(success, reason)
        {
            if(success)
                loadPage(loginPage);
            else
                internal.setErrors(reason);
        }
        
        
        // Parse keywords from error message and set the corresponding error
        function setErrors(reason)
        {
            if(reason.toLowerCase().includes("email"))
            {
                emailInput.errorText = reason;
                emailInput.setError();
            }
            else if(reason.toLowerCase().includes("password"))
            {
                passwordInput.errorText = reason;
                passwordInput.setError();
            }
            else if(reason.toLowerCase().includes("first"))
            {
                firstNameInput.errorText = reason;
                firstNameInput.setError();
            }
            else if(reason.toLowerCase().includes("last"))
            {
                lastNameInput.errorText = reason;
                lastNameInput.setError();
            }
        }
        
        
        // Navigates to the next or previous item, depending on user input
        function moveFocusToNextInput(event, previous, next)
        {
            if(event.key === Qt.Key_Return || event.key === Qt.Key_Down)
            {
                if(next)
                    next.giveFocus();
            }
            else if(event.key === Qt.Key_Up)
            {
                if(previous)
                    previous.giveFocus();
            }
        }
    }
}