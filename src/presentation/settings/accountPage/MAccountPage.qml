import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import CustomComponents 1.0
import Librum.style 1.0
import Librum.icons 1.0
import Librum.globals 1.0
import Librum.controllers 1.0


MFlickWrapper
{
    id: root
    property alias pageCleanup: pageCleanup
    
    contentHeight: page.implicitHeight
    
    
    Page
    {
        id: page
        anchors.fill: parent
        horizontalPadding: 48
        topPadding: 64
        bottomPadding: 22
        background: Rectangle { anchors.fill: parent; color: Style.pagesBackground }
        
        
        Shortcut
        {
            id: saveSettings
            sequence: StandardKey.Save
            onActivated: root.saveAccountSettings()
        }
        
        
        ColumnLayout
        {
            id: layout
            width: parent.width
            spacing: 0
            
            
            RowLayout
            {
                id: titleRow
                Layout.fillWidth: true
                spacing: 0
                
                
                MTitle
                {
                    id: pageTitle
                    titleText: "Account"
                    descriptionText: UserController.firstName + " " + UserController.lastName
                    titleSize: 25
                    descriptionSize: 13.25
                }
                
                Item { Layout.fillWidth: true }
                
                MButton
                {
                    id: saveButton
                    Layout.preferredWidth: 90
                    Layout.preferredHeight: 38
                    Layout.alignment: Qt.AlignBottom
                    borderWidth: 0
                    backgroundColor: Style.colorBasePurple
                    text: "Save"
                    fontSize: 12
                    fontWeight: Font.Bold
                    textColor: Style.colorFocusedButtonText
                    imagePath: Icons.checkWhite
                    imageSize: 14
                    
                    onClicked: root.saveAccountSettings();
                }
            }
            
            Pane
            {
                id: profileBox
                Layout.fillWidth: true
                Layout.topMargin: 32
                topPadding: 25
                horizontalPadding: internal.boxPadding
                bottomPadding: 40
                background: Rectangle
                {
                    color: Style.colorBackground
                    border.color: Style.colorContainerBorder
                    radius: 4
                    antialiasing: true
                }
                
                
                RowLayout
                {
                    id: profileLayout
                    property int horizontalSpacing: 32
                    
                    anchors.fill: parent
                    spacing: 0
                    
                    
                    ColumnLayout
                    {
                        id: profileInputLayout
                        Layout.maximumWidth: Math.round(profileLayout.width - profileLayout.horizontalSpacing) / 2
                        Layout.fillWidth: true
                        spacing: 0
                        
                        
                        Label
                        {
                            id: profileTitle
                            text: "Profile"
                            color: Style.colorText
                            font.pointSize: 16.5
                            font.weight: Font.DemiBold
                        }
                        
                        MLabeledInputBox
                        {
                            id: firstNameInput
                            Layout.fillWidth: true
                            Layout.topMargin: 26
                            boxHeight: 40
                            headerText: "First name"
                            text: UserController.firstName
                            headerToBoxSpacing: 3
                            inputFontSize: 13
                            borderWidth: 1
                            borderRadius: 4
                            
                            onEdited: internal.unsavedChanges = true
                        }
                        
                        MLabeledInputBox
                        {
                            id: lastNameInput
                            Layout.fillWidth: true
                            Layout.topMargin: 18
                            boxHeight: 40
                            headerText: "Last name"
                            text: UserController.lastName
                            headerToBoxSpacing: 3
                            inputFontSize: 13
                            borderWidth: 1
                            borderRadius: 4
                            
                            onEdited: internal.unsavedChanges = true
                        }
                        
                        MLabeledInputBox
                        {
                            id: emailInput
                            Layout.fillWidth: true
                            Layout.topMargin: 18
                            boxHeight: 40
                            headerText: "Email"
                            text: UserController.email
                            headerToBoxSpacing: 3
                            inputFontSize: 13
                            borderWidth: 1
                            borderRadius: 4
                            
                            onEdited: internal.unsavedChanges = true
                        }
                    }
                    
                    MSelectProfilePictureArea
                    {
                        id: profilePictureArea
                        Layout.fillWidth: true
                        Layout.maximumWidth: 312
                        Layout.preferredHeight: 190
                        Layout.topMargin: 76
                        Layout.rightMargin: 40
                        Layout.leftMargin: 32
                        
                        onImageSelected: internal.unsavedChanges = true
                    }
                    
                    Item { Layout.fillWidth: true }
                }
            }
            
            Pane
            {
                id: changePasswordBox
                Layout.fillWidth: true
                Layout.topMargin: 26
                topPadding: 24
                horizontalPadding: internal.boxPadding
                bottomPadding: 50
                background: Rectangle
                {
                    color: Style.colorBackground
                    border.color: Style.colorContainerBorder
                    radius: 4
                    antialiasing: true
                }
                
                
                ColumnLayout
                {
                    id: passwordLayout
                    anchors.fill: parent
                    spacing: 0
                    
                    
                    Label
                    {
                        id: passwordTitle
                        text: "Change password"
                        color: Style.colorText
                        font.pointSize: 16.5
                        font.weight: Font.DemiBold
                    }
                    
                    RowLayout
                    {
                        id: changePasswordInputLayout
                        Layout.fillWidth: true
                        spacing: profileLayout.horizontalSpacing
                        Layout.topMargin: 30
                        
                        
                        MLabeledInputBox
                        {
                            id: passwordInput
                            Layout.fillWidth: true
                            boxHeight: 40
                            headerText: "Password"
                            headerToBoxSpacing: 3
                            inputFontSize: 13
                            borderWidth: 1
                            borderRadius: 4
                            image: Icons.eyeOn
                            toggledImage: Icons.eyeOff
                            
                            onEdited: internal.unsavedChanges = true
                        }
                        
                        MLabeledInputBox
                        {
                            id: passwordConfirmationInput
                            Layout.fillWidth: true
                            boxHeight: 40
                            headerText: "Password confirmation"
                            headerToBoxSpacing: 3
                            inputFontSize: 13
                            borderWidth: 1
                            borderRadius: 4
                            image: Icons.eyeOn
                            toggledImage: Icons.eyeOff
                            
                            onEdited: internal.unsavedChanges = true
                        }
                    }
                }
            }
            
            Pane
            {
                id: yourDataBox
                Layout.fillWidth: true
                Layout.topMargin: 26
                topPadding: 24
                horizontalPadding: internal.boxPadding
                bottomPadding: 38
                background: Rectangle
                {
                    color: Style.colorBackground
                    border.color: Style.colorContainerBorder
                    radius: 4
                    antialiasing: true
                }
                
                
                ColumnLayout
                {
                    id: yourDataContentColumn
                    anchors.fill: parent
                    spacing: 0
                    
                    
                    Label
                    {
                        id: yourDataTitle
                        text: "Your data"
                        color: Style.colorText
                        font.pointSize: 16.5
                        font.weight: Font.DemiBold
                    }
                    
                    MLabeledCheckBox
                    {
                        Layout.fillWidth: true
                        Layout.topMargin: 30
                        boxWidth: 21
                        boxHeight: 21
                        text: "Analyse your reading to make better recommendations"
                        fontSize: 13
                        fontColor: Style.colorText
                        spacing: 12
                        checked: true
                        
                        onClicked: internal.unsavedChanges = true
                    }
                    
                    MLabeledCheckBox
                    {
                        Layout.fillWidth: true
                        Layout.topMargin: 16
                        boxWidth: 21
                        boxHeight: 21
                        text: "Anonymously share the fromats of books you read (e.g. Pdf), to help us improve Librum"
                        fontSize: 13
                        fontColor: Style.colorText
                        spacing: 12
                        checked: true
                        
                        onClicked: internal.unsavedChanges = true
                    }
                    
                    MLabeledCheckBox
                    {
                        Layout.fillWidth: true
                        Layout.topMargin: 16
                        boxWidth: 21
                        boxHeight: 21
                        text: "Collect data on application crashes, so we can prevent it from happening again"
                        fontSize: 13
                        fontColor: Style.colorText
                        spacing: 12
                        
                        onClicked: internal.unsavedChanges = true
                    }
                    
                    MLabeledCheckBox
                    {
                        Layout.fillWidth: true
                        Layout.topMargin: 16
                        boxWidth: 21
                        boxHeight: 21
                        text: "Collect data to display in your statistics"
                        fontSize: 13
                        fontColor: Style.colorText
                        spacing: 12
                        
                        onClicked: internal.unsavedChanges = true
                    }
                }
            }
        }
    }
    
    MForgotToSaveChangesPopup
    {
        id: forgotToSaveChangesDialog
        x: Math.round(root.width / 2 - implicitWidth / 2 - settingsSidebar.width / 2 - sidebar.width / 2)
        y: Math.round(root.height / 2 - implicitHeight / 2 - (root.height > implicitHeight + 80 ? 80 : 0))
        
        saveFunction: root.saveAccountSettings
        discardMethod: () => { internal.unsavedChanges = false; }
        
        onOpenedChanged: if(opened) forgotToSaveChangesDialog.giveFocus()
    }
    
    /*
      Since MAccountPage can have unsaved change, it needs MPageCleanup to
      ensure correct saving of data on page switching.
      */
    MPageCleanup
    {
        id: pageCleanup
        savePageAction: () =>
                        {
                            if(internal.unsavedChanges)
                            {
                                forgotToSaveChangesDialog.open();
                                return true;
                            }
                            return false;
                        }
        
        savingPageFinishedSignal: forgotToSaveChangesDialog.decisionMade
    }
    
    QtObject
    {
        id: internal
        property bool unsavedChanges: false
        property int boxPadding: 40
    }
    
    
    function saveAccountSettings()
    {
        UserController.firstName = firstNameInput.text;
        UserController.lastName = lastNameInput.text;
        
        if(profilePictureArea.image !== Globals.profilePicture)
            Globals.profilePicture = profilePictureArea.image;
        
        internal.unsavedChanges = false;
    }
}