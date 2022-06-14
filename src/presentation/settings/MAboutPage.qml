import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import CustomComponents
import Librum.style
import Librum.icons


Page
{
    id: root
    horizontalPadding: 48
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
                color: Style.colorBackground
                border.color: Style.colorLightBorder
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
                    color: Style.colorBaseText
                    font.pointSize: 16.5
                    font.family: Style.defaultFontFamily
                    font.weight: Font.DemiBold
                }
                
                Label
                {
                    Layout.topMargin: 15
                    text: "CURRENT VERSION"
                    color: Style.colorLightText3
                    font.pointSize: 10.25
                    font.family: Style.defaultFontFamily
                    font.weight: Font.Bold
                }
                
                Label
                {
                    Layout.topMargin: 1
                    text: properties.currentVersion
                    color: Style.colorBaseText
                    font.pointSize: 13.5
                    font.family: Style.defaultFontFamily
                }
                
                Label
                {
                    Layout.topMargin: 12
                    text: "QT VERSION"
                    color: Style.colorLightText3
                    font.pointSize: 10.25
                    font.family: Style.defaultFontFamily
                    font.weight: Font.Bold
                }
                
                Label
                {
                    Layout.topMargin: 1
                    text: properties.currentQtVersion
                    color: Style.colorBaseText
                    font.pointSize: 13.5
                    font.family: Style.defaultFontFamily
                }
                
                MButton
                {
                    Layout.topMargin: 21
                    Layout.preferredWidth: 120
                    Layout.preferredHeight: 32
                    borderColor: Style.colorLightGreen2
                    backgroundColor: Style.colorLightGreen
                    imagePath: Icons.checkGreen
                    imageSize: 20
                    text: "Up-to date"
                    fontSize: 10.5
                    fontColor: Style.colorMediumGreen
                    imageSpacing: 6
                    opacityOnPressed: 0.8
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
                color: Style.colorBackground
                border.color: Style.colorLightBorder
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
                    color: Style.colorBaseText
                    font.pointSize: 16.5
                    font.family: Style.defaultFontFamily
                    font.weight: Font.DemiBold
                }
                
                Label
                {
                    Layout.topMargin: 18
                    text: "COMPANY NAME"
                    color: Style.colorLightText3
                    font.pointSize: 10.25
                    font.family: Style.defaultFontFamily
                    font.weight: Font.Bold
                }
                
                Label
                {
                    Layout.topMargin: 1
                    text: properties.companyName
                    color: Style.colorBasePurple
                    font.pointSize: 13.5
                    font.family: Style.defaultFontFamily
                }
                
                Label
                {
                    Layout.topMargin: 12
                    text: "WEBSITE"
                    color: Style.colorLightText3
                    font.pointSize: 10.25
                    font.family: Style.defaultFontFamily
                    font.weight: Font.Bold
                }
                
                Label
                {
                    text: properties.companyWebsite
                    color: Style.colorBasePurple
                    font.pointSize: 13.5
                    font.family: Style.defaultFontFamily
                }
                
                Label
                {
                    Layout.topMargin: 12
                    text: "CONTACT"
                    color: Style.colorLightText3
                    font.pointSize: 10.25
                    font.family: Style.defaultFontFamily
                    font.weight: Font.Bold
                }
                
                Label
                {
                    Layout.topMargin: 1
                    text: properties.companyEmail
                    color: Style.colorBasePurple
                    font.pointSize: 13.5
                    font.family: Style.defaultFontFamily
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
                color: Style.colorBackground
                border.color: Style.colorLightBorder
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
                    color: Style.colorBaseText
                    font.pointSize: 16.5
                    font.family: Style.defaultFontFamily
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
                        source: Icons.heartGray
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
                        color: Style.colorBaseText
                        font.pointSize: 14.6
                        font.family: Style.defaultFontFamily
                        lineHeight: 1.05
                    }
                }
            }
        }
    }
}