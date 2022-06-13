import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import CustomComponents


Page
{
    id: root
    horizontalPadding: 48
    topPadding: 64
    background: Rectangle
    {
        anchors.fill: parent
        color: properties.pagesBackground
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
                backgroundColor: properties.colorBasePurple
                text: "Save"
                fontSize: 12
                fontWeight: Font.Bold
                fontColor: properties.colorBackground
                imagePath: properties.iconCheckWhite
                imageSize: 14
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
                color: properties.colorBackground
                border.color: properties.colorLightBorder
                radius: 4
                antialiasing: true
            }
            
            
            RowLayout
            {
                id: profileLayout
                anchors.fill: parent
                spacing: 0
                
                
                ColumnLayout
                {
                    id: profileInputColumn
                    Layout.fillWidth: true
                    spacing: 0
                    
                    
                    Label
                    {
                        id: profileTitle
                        text: "Profile"
                        color: properties.colorBaseText
                        font.pointSize: 16.5
                        font.family: properties.defaultFontFamily
                        font.weight: Font.DemiBold
                    }
                    
                    MLabeledInputBox
                    {
                        id: firstNameInput
                        Layout.preferredWidth: passwordInput.width
                        Layout.topMargin: 26
                        boxHeight: 40
                        headerText: "First name"
                        text: "Kai"
                        headerToBoxSpacing: 3
                        inputFontSize: 13
                        borderWidth: 1
                        borderRadius: 4
                    }
                    
                    MLabeledInputBox
                    {
                        id: lastNameInput
                        Layout.preferredWidth: passwordInput.width
                        Layout.topMargin: 18
                        boxHeight: 40
                        headerText: "Last name"
                        text: "Doe"
                        headerToBoxSpacing: 3
                        inputFontSize: 13
                        borderWidth: 1
                        borderRadius: 4
                    }
                    
                    MLabeledInputBox
                    {
                        id: emailInput
                        Layout.preferredWidth: passwordInput.width
                        Layout.topMargin: 18
                        boxHeight: 40
                        headerText: "Email"
                        text: "Kaidoe@gmail.com"
                        headerToBoxSpacing: 3
                        inputFontSize: 13
                        borderWidth: 1
                        borderRadius: 4
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
                        color: properties.colorLightGray
                        border.color: properties.colorLightBorder
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
                            source: properties.illustrationAddImage
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
                            color: properties.colorLightText2
                            font.pointSize: 12
                            font.family: properties.defaultFontFamily
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
                color: properties.colorBackground
                border.color: properties.colorLightBorder
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
                    color: properties.colorBaseText
                    font.pointSize: 16.5
                    font.family: properties.defaultFontFamily
                    font.weight: Font.DemiBold
                }
                
                RowLayout
                {
                    id: changePasswordInputLayout
                    Layout.fillWidth: true
                    spacing: 32
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
                        imagePath: properties.iconEyeOn
                        toggledImagePath: properties.iconEyeOff
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
                        imagePath: properties.iconEyeOn
                        toggledImagePath: properties.iconEyeOff
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
                color: properties.colorBackground
                border.color: properties.colorLightBorder
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
                    color: properties.colorBaseText
                    font.pointSize: 16.5
                    font.family: properties.defaultFontFamily
                    font.weight: Font.DemiBold
                }
                
                MLabeledCheckBox
                {
                    Layout.fillWidth: true
                    Layout.topMargin: 30
                    boxWidth: 21
                    boxHeight: 21
                    textContent: "Analyse your reading to make better recommendations"
                    fontPointSize: 13
                    fontColor: properties.colorBaseText
                    spacing: 12
                    checked: true
                }
                
                MLabeledCheckBox
                {
                    Layout.fillWidth: true
                    Layout.topMargin: 16
                    boxWidth: 21
                    boxHeight: 21
                    textContent: "Share the book types (e.g. ePub, Pdf) to help us improve Librum"
                    fontPointSize: 13
                    fontColor: properties.colorBaseText
                    spacing: 12
                    checked: true
                }
                
                MLabeledCheckBox
                {
                    Layout.fillWidth: true
                    Layout.topMargin: 16
                    boxWidth: 21
                    boxHeight: 21
                    textContent: "Something else the user can agree on allowing"
                    fontPointSize: 13
                    fontColor: properties.colorBaseText
                    spacing: 12
                }
                
                MLabeledCheckBox
                {
                    Layout.fillWidth: true
                    Layout.topMargin: 16
                    boxWidth: 21
                    boxHeight: 21
                    textContent: "Something else the user can agree on allowing"
                    fontPointSize: 13
                    fontColor: properties.colorBaseText
                    spacing: 12
                }
            }
        }
    }
}