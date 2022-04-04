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
        property int marginToLeft: 48
        property int marginToRight: 48
        property int inRectLeftMargin : 40
        
        width: root.width - marginToLeft - marginToRight
        spacing: 0
        
        
        RowLayout
        {
            id: titleRow
            spacing: 0
            
            
            MTitle
            {
                id: title
                Layout.topMargin: 64
                Layout.leftMargin: layout.marginToLeft
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
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: 334
            Layout.topMargin: 32
            Layout.leftMargin: layout.marginToLeft
            color: properties.colorBackground
            border.color: properties.colorLightBorder
            radius: 4
            antialiasing: true
            
            
            RowLayout
            {
                id: profileLayout
                spacing: 0
                
                
                ColumnLayout
                {
                    id: profileInputColumn
                    spacing: 0
                    
                    
                    Label
                    {
                        id: profileTitle
                        Layout.leftMargin: layout.inRectLeftMargin
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
                        Layout.preferredWidth: 699
                        boxHeight: 40
                        Layout.topMargin: 26
                        Layout.leftMargin: layout.inRectLeftMargin
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
                        Layout.preferredWidth: 699
                        boxHeight: 40
                        Layout.topMargin: 18
                        Layout.leftMargin: layout.inRectLeftMargin
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
                        Layout.preferredWidth: 699
                        boxHeight: 40
                        Layout.topMargin: 18
                        Layout.leftMargin: layout.inRectLeftMargin
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
                    Layout.preferredWidth: 312
                    Layout.preferredHeight: 190
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
                            Layout.preferredWidth: 237
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
            }
        }
        
        Rectangle
        {
            id: changePassword
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: 187
            Layout.topMargin: 26
            Layout.leftMargin: layout.marginToLeft
            color: properties.colorBackground
            border.color: properties.colorLightBorder
            radius: 4
            antialiasing: true
            
            
            ColumnLayout
            {
                id: passwordContentColumn
                spacing: 0
                
                Label
                {
                    id: passwordTitle
                    Layout.leftMargin: layout.inRectLeftMargin
                    Layout.topMargin: 24
                    text: "Change password"
                    color: properties.colorBaseText
                    font.pointSize: 16.5
                    font.family: properties.defaultFontFamily
                    font.weight: Font.DemiBold
                }
                
                RowLayout
                {
                    spacing: 32
                    Layout.topMargin: 30
                    
                    MLabeledInputBox
                    {
                        id: passwordInput
                        Layout.preferredWidth: 699
                        boxHeight: 40
                        Layout.leftMargin: layout.inRectLeftMargin
                        headerText: "Password"
                        text: ""
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
                        Layout.preferredWidth: 699
                        boxHeight: 40
                        headerText: "Password confirmation"
                        text: ""
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
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: 252
            Layout.topMargin: 26
            Layout.leftMargin: layout.marginToLeft
            color: properties.colorBackground
            border.color: properties.colorLightBorder
            radius: 4
            antialiasing: true
            
            
            ColumnLayout
            {
                id: yourDataContentColumn
                spacing: 0
                
                Label
                {
                    id: yourDataTitle
                    Layout.leftMargin: layout.inRectLeftMargin
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
                    Layout.leftMargin: layout.inRectLeftMargin
                    spacing: 12
                    
                    MCheckBox
                    {
                        id: analysingCheckbox
                        Layout.preferredWidth: 21
                        Layout.preferredHeight: 21
                    }
                    
                    Label
                    {
                        text: "Analyse your reading to make better recommendations"
                        font.family: properties.defaultFontFamily
                        font.pointSize: 13
                        color: properties.colorBaseText
                    }
                }
                
                RowLayout
                {
                    Layout.topMargin: 16
                    Layout.leftMargin: layout.inRectLeftMargin
                    spacing: 12
                    
                    MCheckBox
                    {
                        id: shareBookTypesCheckbox
                        Layout.preferredWidth: 21
                        Layout.preferredHeight: 21
                    }
                    
                    Label
                    {
                        text: "Share the book types (e.g. ePub, Pdf) to help us improve Librum"
                        font.family: properties.defaultFontFamily
                        font.pointSize: 13
                        color: properties.colorBaseText
                    }
                }
            }
        }
    }
}