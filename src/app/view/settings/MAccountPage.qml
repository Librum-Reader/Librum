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
            
            
            ColumnLayout
            {
                id: inDetailsLayout
                spacing: 0
                
                
                Label
                {
                    id: detailsTitle
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
        }
    }
}