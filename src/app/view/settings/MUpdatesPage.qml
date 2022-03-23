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
        
        
        MTitle
        {
            id: title
            Layout.topMargin: 64
            Layout.leftMargin: layout.marginToLeft
            titleText: "Updates"
            descriptionText: "Anything new update?"
            titleSize: 25
            descriptionSize: 13.25
        }
        
        Rectangle
        {
            id: container
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: 342
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
                    id: newUpdateTitle
                    Layout.leftMargin: layout.inRectLeftMargin
                    Layout.topMargin: 32
                    text: "There is a new update available!"
                    color: properties.colorBaseText
                    font.pointSize: 23
                    font.family: properties.defaultFontFamily
                    font.weight: Font.Bold
                }
                
                Label
                {
                    Layout.leftMargin: layout.inRectLeftMargin
                    Layout.topMargin: 7
                    text: "Download it and get the great new improvements"
                    color: properties.colorLightText3
                    font.pointSize: 15
                    font.family: properties.defaultFontFamily
                }
                
                Label
                {
                    Layout.leftMargin: layout.inRectLeftMargin
                    Layout.topMargin: 20
                    text: "The newest version is"
                    color: properties.colorLightText3
                    font.pointSize: 14.5
                    font.family: properties.defaultFontFamily
                }
                
                Label
                {
                    Layout.leftMargin: layout.inRectLeftMargin
                    text: properties.newestVersion
                    color: properties.colorLightText3
                    font.pointSize: 14.2
                    font.bold: true
                    font.family: properties.defaultFontFamily
                }
                
                MButton
                {
                    id: downloadButton
                    Layout.preferredWidth: 120
                    Layout.preferredHeight: 34
                    Layout.leftMargin: layout.inRectLeftMargin
                    Layout.topMargin: 18
                    backgroundColor: properties.colorLightGrayBackground
                    imagePath: properties.iconArrowDownHallow
                    imageSize: 14
                    imageSpacing: 6
                    textContent: "Download"
                    fontSize: 11.7
                    fontBold: true
                    fontColor: properties.colorBaseText
                    borderColor: properties.colorLightBorder
                }
                
                Label
                {
                    Layout.leftMargin: layout.inRectLeftMargin
                    Layout.topMargin: 25
                    text: "See the exact changes on our website at:"
                    color: properties.colorLightText3
                    font.pointSize: 13.5
                    font.family: properties.defaultFontFamily
                }
                
                Label
                {
                    Layout.leftMargin: layout.inRectLeftMargin
                    text: "www.etovex.com/new_updates"
                    font.underline: true
                    color: properties.colorBasePurple
                    font.pointSize: 13.5
                    font.family: properties.defaultFontFamily
                }
            }
        }
    }
}