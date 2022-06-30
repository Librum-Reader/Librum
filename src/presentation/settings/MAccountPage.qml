import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import CustomComponents
import Librum.style
import Librum.icons


Page
{
    id: root
    property alias pageCleanup: pageCleanup
    property alias forgotToSaveChangesDialog: forgotToSaveChangesDialog
    readonly property bool hasCleanup: true
    property bool unsavedChanges: false
    
    horizontalPadding: 48
    topPadding: 64
    background: Rectangle
    {
        anchors.fill: parent
        color: Style.pagesBackground
    }
    
    
    ColumnLayout
    {
        id: layout
        property int insideMargin : 40
        
        width: parent.width
        spacing: 0
        
        
        RowLayout
        {
            id: titleRow
            Layout.fillWidth: true
            spacing: 0
            
            
            MTitle
            {
                id: title
                titleText: "Account"
                descriptionText: "Kai Doe"
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
                fontColor: Style.colorBackground
                imagePath: Icons.checkWhite
                imageSize: 14
                
                onClicked:
                {
                    root.saveAccountSettings();
                }
            }
        }
        
        Pane
        {
            id: profile
            Layout.fillWidth: true
            Layout.topMargin: 32
            topPadding: 25
            horizontalPadding: layout.insideMargin
            bottomPadding: 40
            background: Rectangle
            {
                color: Style.colorBackground
                border.color: Style.colorLightBorder
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
                    id: profileInputColumn
                    Layout.maximumWidth: Math.round(profileLayout.width - profileLayout.horizontalSpacing) / 2
                    Layout.fillWidth: true
                    spacing: 0
                    
                    
                    Label
                    {
                        id: profileTitle
                        text: "Profile"
                        color: Style.colorBaseText
                        font.pointSize: 16.5
                        font.family: Style.defaultFontFamily
                        font.weight: Font.DemiBold
                    }
                    
                    MLabeledInputBox
                    {
                        id: firstNameInput
                        Layout.fillWidth: true
                        Layout.topMargin: 26
                        boxHeight: 40
                        headerText: "First name"
                        text: "Kai"
                        headerToBoxSpacing: 3
                        inputFontSize: 13
                        borderWidth: 1
                        borderRadius: 4
                        
                        onEdited: root.unsavedChanges = true
                    }
                    
                    MLabeledInputBox
                    {
                        id: lastNameInput
                        Layout.fillWidth: true
                        Layout.topMargin: 18
                        boxHeight: 40
                        headerText: "Last name"
                        text: "Doe"
                        headerToBoxSpacing: 3
                        inputFontSize: 13
                        borderWidth: 1
                        borderRadius: 4
                        
                        onEdited: root.unsavedChanges = true
                    }
                    
                    MLabeledInputBox
                    {
                        id: emailInput
                        Layout.fillWidth: true
                        Layout.topMargin: 18
                        boxHeight: 40
                        headerText: "Email"
                        text: "Kaidoe@gmail.com"
                        headerToBoxSpacing: 3
                        inputFontSize: 13
                        borderWidth: 1
                        borderRadius: 4
                        
                        onEdited: root.unsavedChanges = true
                    }
                }
                
                Pane
                {
                    id: dropArea
                    Layout.fillWidth: true
                    Layout.maximumWidth: 312
                    Layout.preferredHeight: 190
                    Layout.topMargin: 76
                    Layout.rightMargin: 40
                    Layout.leftMargin: 32
                    clip: true
                    background: Rectangle
                    {
                        color: Style.colorLightGray
                        border.color: Style.colorLightBorder
                        radius: 4
                    }
                    
                    
                    ColumnLayout
                    {
                        id: dropAreaLayout
                        width: parent.width
                        spacing: 10
                        
                        
                        Image
                        {
                            id: dropAreaIcon
                            Layout.alignment: Qt.AlignCenter
                            Layout.topMargin: 50
                            source: Icons.addImage
                            sourceSize.width: 40
                            fillMode: Image.PreserveAspectFit
                        }
                        
                        Label
                        {
                            id: dropAreaText
                            Layout.fillWidth: true
                            Layout.maximumWidth: 237
                            Layout.alignment: Qt.AlignCenter
                            text: "Click to select an image or drag and drop in this area"
                            horizontalAlignment: Qt.AlignHCenter
                            wrapMode: Text.WordWrap
                            color: Style.colorLightText2
                            font.pointSize: 12
                            font.family: Style.defaultFontFamily
                            font.weight: Font.DemiBold
                            lineHeight: 0.9
                        }
                    }
                }
                
                Item { Layout.fillWidth: true }
            }
        }
        
        Pane
        {
            id: changePassword
            Layout.fillWidth: true
            Layout.topMargin: 26
            topPadding: 24
            horizontalPadding: layout.insideMargin
            bottomPadding: 50
            background: Rectangle
            {
                color: Style.colorBackground
                border.color: Style.colorLightBorder
                radius: 4
                antialiasing: true
            }
            
            
            ColumnLayout
            {
                id: passwordColumn
                anchors.fill: parent
                spacing: 0
                
                Label
                {
                    id: passwordTitle
                    text: "Change password"
                    color: Style.colorBaseText
                    font.pointSize: 16.5
                    font.family: Style.defaultFontFamily
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
                        imagePath: Icons.eyeOn
                        toggledImagePath: Icons.eyeOff
                        
                        onEdited: root.unsavedChanges = true
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
                        imagePath: Icons.eyeOn
                        toggledImagePath: Icons.eyeOff
                        
                        onEdited: root.unsavedChanges = true
                    }
                }
            }
        }
        
        Pane
        {
            id: yourData
            Layout.fillWidth: true
            Layout.topMargin: 26
            topPadding: 24
            horizontalPadding: layout.insideMargin
            bottomPadding: 38
            background: Rectangle
            {
                color: Style.colorBackground
                border.color: Style.colorLightBorder
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
                    color: Style.colorBaseText
                    font.pointSize: 16.5
                    font.family: Style.defaultFontFamily
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
                    fontColor: Style.colorBaseText
                    spacing: 12
                    checked: true
                    
                    onClicked: root.unsavedChanges = true
                }
                
                MLabeledCheckBox
                {
                    Layout.fillWidth: true
                    Layout.topMargin: 16
                    boxWidth: 21
                    boxHeight: 21
                    text: "Share the book types (e.g. ePub, Pdf) to help us improve Librum"
                    fontSize: 13
                    fontColor: Style.colorBaseText
                    spacing: 12
                    checked: true
                    
                    onClicked: root.unsavedChanges = true
                }
                
                MLabeledCheckBox
                {
                    Layout.fillWidth: true
                    Layout.topMargin: 16
                    boxWidth: 21
                    boxHeight: 21
                    text: "Something else the user can agree on allowing"
                    fontSize: 13
                    fontColor: Style.colorBaseText
                    spacing: 12
                    
                    onClicked: root.unsavedChanges = true
                }
                
                MLabeledCheckBox
                {
                    Layout.fillWidth: true
                    Layout.topMargin: 16
                    boxWidth: 21
                    boxHeight: 21
                    text: "Something else the user can agree on allowing"
                    fontSize: 13
                    fontColor: Style.colorBaseText
                    spacing: 12
                    
                    onClicked: root.unsavedChanges = true
                }
            }
        }
    }
    
    
    MForgotToSaveChangesPopup
    {
        id: forgotToSaveChangesDialog
        x: root.width / 2 - implicitWidth / 2 - settingsSidebar.width / 2 - sidebar.width / 2 - root.horizontalPadding
        y: root.height / 2 - implicitHeight / 2 - root.topPadding - 50
        
        saveMethod: saveAccountSettings
        dontSaveMethod: () => { root.unsavedChanges = false; }
        
        onOpenedChanged: if(opened) forgotToSaveChangesDialog.giveFocus()
    }
    
    MPageCleanup
    {
        id: pageCleanup
        action: () =>
                {
                    if(root.unsavedChanges)
                    {
                        forgotToSaveChangesDialog.open();
                        return true;
                    }
                    return false;
                }
        
        signalToBindTo: forgotToSaveChangesDialog.decisionMade
    }
    
    
    function saveAccountSettings()
    {
        root.unsavedChanges = false;
    }
}