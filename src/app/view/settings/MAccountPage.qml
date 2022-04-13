import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import CustomComponents


Page
{
    id: root
    background: Rectangle
    {
        anchors.fill: parent
        color: properties.pagesBackground
    }
    
    
    ColumnLayout
    {
        id: layout
        property int outsideMargin: 48
        property int inRectMargin : 40
        
        width: root.width
        spacing: 0
        
        
        RowLayout
        {
            id: titleRow
            Layout.fillWidth: true
            Layout.leftMargin: layout.outsideMargin
            Layout.rightMargin: layout.outsideMargin
            spacing: 0
            
            
            MTitle
            {
                id: title
                Layout.topMargin: 64
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
                textContent: "Save"
                fontSize: 12
                fontBold: true
                fontColor: properties.colorBackground
                imagePath: properties.iconCheckWhite
                imageSize: 14
            }
        }
        
        Rectangle
        {
            id: profile
            Layout.fillWidth: true
            Layout.preferredHeight: 334
            Layout.topMargin: 32
            Layout.leftMargin: layout.outsideMargin
            Layout.rightMargin: layout.outsideMargin
            color: properties.colorBackground
            border.color: properties.colorLightBorder
            radius: 4
            antialiasing: true
            
            
            RowLayout
            {
                id: profileLayout
                width: parent.width
                spacing: 0
                
                
                ColumnLayout
                {
                    id: profileInputColumn
                    Layout.fillWidth: true
                    spacing: 0
                    
                    
                    Label
                    {
                        id: profileTitle
                        Layout.leftMargin: layout.inRectMargin
                        Layout.topMargin: 24
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
                        boxHeight: 40
                        Layout.topMargin: 26
                        Layout.leftMargin: layout.inRectMargin
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
                        boxHeight: 40
                        Layout.topMargin: 18
                        Layout.leftMargin: layout.inRectMargin
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
                        boxHeight: 40
                        Layout.topMargin: 18
                        Layout.leftMargin: layout.inRectMargin
                        headerText: "Email"
                        text: "Kaidoe@gmail.com"
                        headerToBoxSpacing: 3
                        inputFontSize: 13
                        borderWidth: 1
                        borderRadius: 4
                    }
                }
                
                Rectangle
                {
                    id: dropArea
                    Layout.fillWidth: true
                    Layout.maximumWidth: 312
                    Layout.alignment: Qt.AlignLeft
                    Layout.preferredHeight: 190
                    Layout.rightMargin: 40
                    Layout.topMargin: firstNameInput.y + 24
                    Layout.leftMargin: 32
                    color: properties.colorLightGray
                    border.color: properties.colorLightBorder
                    radius: 4
                    
                    
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
                            source: properties.iconAddImage
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
        
        Rectangle
        {
            id: changePassword
            Layout.fillWidth: true
            Layout.preferredHeight: 187
            Layout.topMargin: 26
            Layout.leftMargin: layout.outsideMargin
            Layout.rightMargin: layout.outsideMargin
            color: properties.colorBackground
            border.color: properties.colorLightBorder
            radius: 4
            antialiasing: true
            
            
            ColumnLayout
            {
                id: passwordContentColumn
                width: parent.width
                spacing: 0
                
                Label
                {
                    id: passwordTitle
                    Layout.leftMargin: layout.inRectMargin
                    Layout.topMargin: 24
                    text: "Change password"
                    color: properties.colorBaseText
                    font.pointSize: 16.5
                    font.family: properties.defaultFontFamily
                    font.weight: Font.DemiBold
                }
                
                RowLayout
                {
                    id: changePasswordInputLayout
                    width: parent.width - layout.leftMargin*2
                    spacing: 32
                    Layout.topMargin: 30
                    
                    MLabeledInputBox
                    {
                        id: passwordInput
                        Layout.fillWidth: true
                        boxHeight: 40
                        Layout.leftMargin: layout.inRectMargin
                        headerText: "Password"
                        headerToBoxSpacing: 3
                        inputFontSize: 13
                        borderWidth: 1
                        borderRadius: 4
                        addImageToRight: true
                        imagePath: properties.iconEyeOn
                        toggledImagePath: properties.iconEyeOff
                    }
                    
                    MLabeledInputBox
                    {
                        id: passwordConfirmationInput
                        Layout.fillWidth: true
                        Layout.rightMargin: layout.inRectMargin
                        boxHeight: 40
                        headerText: "Password confirmation"
                        headerToBoxSpacing: 3
                        inputFontSize: 13
                        borderWidth: 1
                        borderRadius: 4
                        addImageToRight: true
                        imagePath: properties.iconEyeOn
                        toggledImagePath: properties.iconEyeOff
                    }
                }
            }
        }
        
        Rectangle
        {
            id: yourData
            Layout.fillWidth: true
            Layout.preferredHeight: yourDataContentColumn.height + 38
            Layout.topMargin: 26
            Layout.leftMargin: layout.outsideMargin
            Layout.rightMargin: layout.outsideMargin
            color: properties.colorBackground
            border.color: properties.colorLightBorder
            radius: 4
            antialiasing: true
            
            
            ColumnLayout
            {
                id: yourDataContentColumn
                width: parent.width
                spacing: 0
                
                Label
                {
                    id: yourDataTitle
                    Layout.leftMargin: layout.inRectMargin
                    Layout.topMargin: 24
                    text: "Your data"
                    color: properties.colorBaseText
                    font.pointSize: 16.5
                    font.family: properties.defaultFontFamily
                    font.weight: Font.DemiBold
                }
                
                RowLayout
                {
                    Layout.topMargin: 30
                    Layout.leftMargin: layout.inRectMargin
                    spacing: 12
                    
                    MCheckBox
                    {
                        id: analysingCheckbox
                        Layout.preferredWidth: 21
                        Layout.preferredHeight: 21
                        checked: true
                    }
                    
                    Label
                    {
                        Layout.fillWidth: true
                        text: "Analyse your reading to make better recommendations"
                        wrapMode: Text.WordWrap
                        font.family: properties.defaultFontFamily
                        font.pointSize: 13
                        color: properties.colorBaseText
                    }
                }
                
                RowLayout
                {
                    Layout.fillWidth: true
                    Layout.topMargin: 16
                    Layout.leftMargin: layout.inRectMargin
                    spacing: 12
                    
                    
                    MCheckBox
                    {
                        id: shareBookTypesCheckbox
                        Layout.preferredWidth: 21
                        Layout.preferredHeight: 21
                        checked: true
                    }
                    
                    Label
                    {
                        
                        Layout.fillWidth: true
                        text: "Share the book types (e.g. ePub, Pdf) to help us improve Librum"
                        wrapMode: Text.WordWrap
                        font.family: properties.defaultFontFamily
                        font.pointSize: 13
                        color: properties.colorBaseText
                    }
                }
                
                RowLayout
                {
                    Layout.topMargin: 16
                    Layout.leftMargin: layout.inRectMargin
                    spacing: 12
                    
                    MCheckBox
                    {
                        id: otherCheckbox1
                        Layout.preferredWidth: 21
                        Layout.preferredHeight: 21
                    }
                    
                    Label
                    {
                        Layout.fillWidth: true
                        text: "Something else the user can agree on allowing"
                        wrapMode: Text.WordWrap
                        font.family: properties.defaultFontFamily
                        font.pointSize: 13
                        color: properties.colorBaseText
                    }
                }
                
                RowLayout
                {
                    Layout.topMargin: 16
                    Layout.leftMargin: layout.inRectMargin
                    spacing: 12
                    
                    MCheckBox
                    {
                        id: otherCheckbox2
                        Layout.preferredWidth: 21
                        Layout.preferredHeight: 21
                    }
                    
                    Label
                    {
                        Layout.fillWidth: true
                        text: "Something else the user can agree on allowing"
                        wrapMode: Text.WordWrap
                        font.family: properties.defaultFontFamily
                        font.pointSize: 13
                        color: properties.colorBaseText
                    }
                }
            }
        }
    }
}