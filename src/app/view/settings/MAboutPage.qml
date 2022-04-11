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
        
        width: root.width - outsideMargin*2
        spacing: 0
        
        
        MTitle
        {
            id: title
            Layout.topMargin: 64
            Layout.leftMargin: layout.outsideMargin
            titleText: "About"
            descriptionText: "About this application"
            titleSize: 25
            descriptionSize: 13.25
        }
        
        Rectangle
        {
            id: details
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: 232
            Layout.topMargin: 32
            Layout.leftMargin: layout.outsideMargin
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
                    Layout.leftMargin: layout.inRectMargin
                    Layout.topMargin: 24
                    text: "Details"
                    color: properties.colorBaseText
                    font.pointSize: 16.5
                    font.family: properties.defaultFontFamily
                    font.weight: Font.DemiBold
                }
                
                Label
                {
                    Layout.leftMargin: layout.inRectMargin
                    Layout.topMargin: 15
                    text: "CURRENT VERSION"
                    color: properties.colorLightText3
                    font.pointSize: 10.25
                    font.family: properties.defaultFontFamily
                    font.weight: Font.Bold
                }
                
                Label
                {
                    Layout.leftMargin: layout.inRectMargin
                    Layout.topMargin: 1
                    text: properties.currentVersion
                    color: properties.colorBaseText
                    font.pointSize: 13.5
                    font.family: properties.defaultFontFamily
                }
                
                Label
                {
                    Layout.leftMargin: layout.inRectMargin
                    Layout.topMargin: 12
                    text: "QT VERSION"
                    color: properties.colorLightText3
                    font.pointSize: 10.25
                    font.family: properties.defaultFontFamily
                    font.weight: Font.Bold
                }
                
                Label
                {
                    Layout.leftMargin: layout.inRectMargin
                    Layout.topMargin: 1
                    text: properties.currentQtVersion
                    color: properties.colorBaseText
                    font.pointSize: 13.5
                    font.family: properties.defaultFontFamily
                }
                
                MButton
                {
                    Layout.leftMargin: layout.inRectMargin
                    Layout.topMargin: 21
                    Layout.preferredWidth: 120
                    Layout.preferredHeight: 32
                    borderColor: properties.colorLightGreen2
                    backgroundColor: properties.colorLightGreen
                    imagePath: properties.iconCheckGreen
                    imageSize: 20
                    textContent: "Up-to date"
                    fontSize: 10.5
                    fontColor: properties.colorMediumGreen
                    imageSpacing: 6
                    backgroundOpacityOnPressed: 0.8
                }
            }
        }
        
        Rectangle
        {
            id: creator
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: 242
            Layout.topMargin: 26
            Layout.leftMargin: layout.outsideMargin
            color: properties.colorBackground
            border.color: properties.colorLightBorder
            radius: 4
            antialiasing: true
            
            
            ColumnLayout
            {
                id: inCreatorLayout
                spacing: 0
                
                Label
                {
                    id: creatorTitle
                    Layout.leftMargin: layout.inRectMargin
                    Layout.topMargin: 24
                    text: "Creator"
                    color: properties.colorBaseText
                    font.pointSize: 16.5
                    font.family: properties.defaultFontFamily
                    font.weight: Font.DemiBold
                }
                
                Label
                {
                    Layout.leftMargin: layout.inRectMargin
                    Layout.topMargin: 18
                    text: "COMPANY NAME"
                    color: properties.colorLightText3
                    font.pointSize: 10.25
                    font.family: properties.defaultFontFamily
                    font.weight: Font.Bold
                }
                
                Label
                {
                    Layout.leftMargin: layout.inRectMargin
                    Layout.topMargin: 1
                    text: properties.companyName
                    color: properties.colorBasePurple
                    font.pointSize: 13.5
                    font.family: properties.defaultFontFamily
                }
                
                Label
                {
                    Layout.leftMargin: layout.inRectMargin
                    Layout.topMargin: 12
                    text: "WEBSITE"
                    color: properties.colorLightText3
                    font.pointSize: 10.25
                    font.family: properties.defaultFontFamily
                    font.weight: Font.Bold
                }
                
                Label
                {
                    Layout.leftMargin: layout.inRectMargin
                    text: properties.companyWebsite
                    color: properties.colorBasePurple
                    font.pointSize: 13.5
                    font.family: properties.defaultFontFamily
                }
                
                Label
                {
                    Layout.leftMargin: layout.inRectMargin
                    Layout.topMargin: 12
                    text: "CONTACT"
                    color: properties.colorLightText3
                    font.pointSize: 10.25
                    font.family: properties.defaultFontFamily
                    font.weight: Font.Bold
                }
                
                Label
                {
                    Layout.leftMargin: layout.inRectMargin
                    Layout.topMargin: 1
                    text: properties.companyEmail
                    color: properties.colorBasePurple
                    font.pointSize: 13.5
                    font.family: properties.defaultFontFamily
                }
            }
        }
    
        Rectangle
        {
            id: thisApp
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: 218
            Layout.minimumHeight: inThisAppLayout.height + 15
            Layout.topMargin: 26
            Layout.leftMargin: layout.outsideMargin
            color: properties.colorBackground
            border.color: properties.colorLightBorder
            radius: 4
            antialiasing: true
            
            ColumnLayout
            {
                id: inThisAppLayout
                width: parent.width
                spacing: 0
                
                
                Label
                {
                    id: thisAppTitle
                    Layout.leftMargin: layout.inRectMargin
                    Layout.topMargin: 24
                    text: "This App"
                    color: properties.colorBaseText
                    font.pointSize: 16.5
                    font.family: properties.defaultFontFamily
                    font.weight: Font.DemiBold
                }
                
                RowLayout
                {
                    spacing: 0
                    Layout.preferredWidth: parent.width
                    Layout.topMargin: 40
                    Layout.leftMargin: layout.inRectMargin
                    
                    Image
                    {
                        id: heartImage
                        source: properties.iconHeartGray
                        fillMode: Image.PreserveAspectFit
                        sourceSize.width: 80
                    }
                    
                    Label
                    {
                        id: thisAppText
                        Layout.fillWidth: parent.width
                        Layout.rightMargin: layout.inRectMargin
                        Layout.leftMargin: 22
                        text: "This app is completely free for everyone who just wants to enjoy a book.\n" +
                              "We hope you enjoy using it! If you do, feel free to leave us a rating and some feedback."
                        wrapMode: Text.WordWrap
                        color: properties.colorBaseText
                        font.pointSize: 14.6
                        font.family: properties.defaultFontFamily
                        lineHeight: 1.05
                    }
                }
            }
        }
    }
}