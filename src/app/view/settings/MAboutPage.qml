import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import CustomComponents


Page
{
    id: root
    horizontalPadding: 48
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
        spacing: 26
        
        
        MTitle
        {
            id: title
            Layout.topMargin: 64
            titleText: "About"
            descriptionText: "About this application"
            titleSize: 25
            descriptionSize: 13.25
        }
        
        Pane
        {
            id: details
            Layout.fillWidth: true
            Layout.topMargin: 6
            topPadding: 24
            horizontalPadding: layout.insideMargin
            bottomPadding: 21
            background: Rectangle
            {
                color: properties.colorBackground
                border.color: properties.colorLightBorder
                radius: 4
                antialiasing: true
            }
            
            
            ColumnLayout
            {
                id: inDetailsLayout
                anchors.fill: parent
                spacing: 0
                
                
                Label
                {
                    id: detailsTitle
                    text: "Details"
                    color: properties.colorBaseText
                    font.pointSize: 16.5
                    font.family: properties.defaultFontFamily
                    font.weight: Font.DemiBold
                }
                
                Label
                {
                    Layout.topMargin: 15
                    text: "CURRENT VERSION"
                    color: properties.colorLightText3
                    font.pointSize: 10.25
                    font.family: properties.defaultFontFamily
                    font.weight: Font.Bold
                }
                
                Label
                {
                    Layout.topMargin: 1
                    text: properties.currentVersion
                    color: properties.colorBaseText
                    font.pointSize: 13.5
                    font.family: properties.defaultFontFamily
                }
                
                Label
                {
                    Layout.topMargin: 12
                    text: "QT VERSION"
                    color: properties.colorLightText3
                    font.pointSize: 10.25
                    font.family: properties.defaultFontFamily
                    font.weight: Font.Bold
                }
                
                Label
                {
                    Layout.topMargin: 1
                    text: properties.currentQtVersion
                    color: properties.colorBaseText
                    font.pointSize: 13.5
                    font.family: properties.defaultFontFamily
                }
                
                MButton
                {
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
        
        Pane
        {
            id: creator
            Layout.fillWidth: true
            topPadding: 24
            horizontalPadding: layout.insideMargin
            bottomPadding: 30
            background: Rectangle
            {
                color: properties.colorBackground
                border.color: properties.colorLightBorder
                radius: 4
                antialiasing: true
            }
            
            
            ColumnLayout
            {
                id: inCreatorLayout
                anchors.fill: parent
                spacing: 0
                
                
                Label
                {
                    id: creatorTitle
                    text: "Creator"
                    color: properties.colorBaseText
                    font.pointSize: 16.5
                    font.family: properties.defaultFontFamily
                    font.weight: Font.DemiBold
                }
                
                Label
                {
                    Layout.topMargin: 18
                    text: "COMPANY NAME"
                    color: properties.colorLightText3
                    font.pointSize: 10.25
                    font.family: properties.defaultFontFamily
                    font.weight: Font.Bold
                }
                
                Label
                {
                    Layout.topMargin: 1
                    text: properties.companyName
                    color: properties.colorBasePurple
                    font.pointSize: 13.5
                    font.family: properties.defaultFontFamily
                }
                
                Label
                {
                    Layout.topMargin: 12
                    text: "WEBSITE"
                    color: properties.colorLightText3
                    font.pointSize: 10.25
                    font.family: properties.defaultFontFamily
                    font.weight: Font.Bold
                }
                
                Label
                {
                    text: properties.companyWebsite
                    color: properties.colorBasePurple
                    font.pointSize: 13.5
                    font.family: properties.defaultFontFamily
                }
                
                Label
                {
                    Layout.topMargin: 12
                    text: "CONTACT"
                    color: properties.colorLightText3
                    font.pointSize: 10.25
                    font.family: properties.defaultFontFamily
                    font.weight: Font.Bold
                }
                
                Label
                {
                    Layout.topMargin: 1
                    text: properties.companyEmail
                    color: properties.colorBasePurple
                    font.pointSize: 13.5
                    font.family: properties.defaultFontFamily
                }
            }
        }
        
        Pane
        {
            id: thisApp
            Layout.fillWidth: true
            Layout.minimumHeight: inThisAppLayout.height + 15
            topPadding: 24
            horizontalPadding: layout.insideMargin
            bottomPadding: 56
            background: Rectangle
            {
                color: properties.colorBackground
                border.color: properties.colorLightBorder
                radius: 4
                antialiasing: true
            }
            
            
            ColumnLayout
            {
                id: inThisAppLayout
                anchors.fill: parent
                spacing: 0
                
                
                Label
                {
                    id: thisAppTitle
                    text: "This App"
                    color: properties.colorBaseText
                    font.pointSize: 16.5
                    font.family: properties.defaultFontFamily
                    font.weight: Font.DemiBold
                }
                
                RowLayout
                {
                    id: heartRow
                    Layout.fillWidth: true
                    Layout.topMargin: 40
                    spacing: 0
                    
                    
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
                        Layout.fillWidth: true
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