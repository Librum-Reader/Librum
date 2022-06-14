import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Librum.style
import CustomComponents


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
        width: parent.width
        spacing: 0
        
        
        MTitle
        {
            id: title
            Layout.topMargin: 64
            titleText: "Updates"
            descriptionText: "Anything new update?"
            titleSize: 25
            descriptionSize: 13.25
        }
        
        Pane
        {
            id: container
            Layout.fillWidth: true
            Layout.topMargin: 32
            horizontalPadding: 40
            topPadding: 32
            bottomPadding: 32
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
                width: parent.width
                spacing: 0
                
                Label
                {
                    id: newUpdateTitle
                    Layout.fillWidth: true
                    text: "A new update is available!"
                    wrapMode: Text.WordWrap
                    color: Style.colorBaseText
                    font.pointSize: 23
                    font.family: Style.defaultFontFamily
                    font.weight: Font.Bold
                }
                
                Label
                {
                    Layout.fillWidth: true
                    Layout.topMargin: 7
                    text: "Download it and get the great new improvements"
                    wrapMode: Text.WordWrap
                    color: Style.colorLightText3
                    font.pointSize: 15
                    font.family: Style.defaultFontFamily
                }
                
                Label
                {
                    Layout.fillWidth: true
                    Layout.topMargin: 26
                    text: "The newest version is"
                    wrapMode: Text.WordWrap
                    color: Style.colorLightText3
                    font.pointSize: 14.5
                    font.family: Style.defaultFontFamily
                }
                
                Label
                {
                    Layout.fillWidth: true
                    text: properties.newestVersion
                    wrapMode: Text.WordWrap
                    color: Style.colorLightText3
                    font.pointSize: 14.2
                    font.bold: true
                    font.family: Style.defaultFontFamily
                }
                
                MButton
                {
                    id: downloadButton
                    Layout.preferredWidth: 120
                    Layout.preferredHeight: 34
                    Layout.topMargin: 14
                    borderColor: Style.colorLightBorder
                    backgroundColor: Style.colorLightGrayBackground
                    imagePath: properties.iconArrowDownBlack
                    imageSize: 14
                    imageSpacing: 6
                    text: "Download"
                    fontSize: 11.7
                    fontWeight: Font.Bold
                    fontColor: Style.colorBaseText
                    opacityOnPressed: 0.8
                }
                
                Label
                {
                    Layout.fillWidth: true
                    Layout.topMargin: 42
                    text: "See the exact changes on our website at:"
                    wrapMode: Text.WordWrap
                    color: Style.colorLightText3
                    font.pointSize: 13.5
                    font.family: Style.defaultFontFamily
                }
                
                Label
                {
                    Layout.fillWidth: true
                    text: "www.etovex.com/new_updates"
                    wrapMode: Text.WordWrap
                    font.underline: true
                    color: Style.colorBasePurple
                    font.pointSize: 13.5
                    font.family: Style.defaultFontFamily
                }
            }
        }
    }
}