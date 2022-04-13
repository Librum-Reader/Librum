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
        
        
        MTitle
        {
            id: title
            Layout.topMargin: 64
            Layout.leftMargin: layout.outsideMargin
            titleText: "Updates"
            descriptionText: "Anything new update?"
            titleSize: 25
            descriptionSize: 13.25
        }
        
        Rectangle
        {
            id: container
            Layout.fillWidth: true
            Layout.preferredHeight: inDetailsLayout.height + 32
            Layout.topMargin: 32
            Layout.leftMargin: layout.outsideMargin
            Layout.rightMargin: layout.outsideMargin
            color: properties.colorBackground
            border.color: properties.colorLightBorder
            radius: 4
            antialiasing: true
            
            ColumnLayout
            {
                id: inDetailsLayout
                width: parent.width
                spacing: 0
                
                Label
                {
                    id: newUpdateTitle
                    Layout.fillWidth: true
                    Layout.leftMargin: layout.inRectMargin
                    Layout.topMargin: 32
                    text: "A new update is available!"
                    wrapMode: Text.WordWrap
                    color: properties.colorBaseText
                    font.pointSize: 23
                    font.family: properties.defaultFontFamily
                    font.weight: Font.Bold
                }
                
                Label
                {
                    Layout.fillWidth: true
                    Layout.leftMargin: layout.inRectMargin
                    Layout.topMargin: 7
                    text: "Download it and get the great new improvements"
                    wrapMode: Text.WordWrap
                    color: properties.colorLightText3
                    font.pointSize: 15
                    font.family: properties.defaultFontFamily
                }
                
                Label
                {
                    Layout.fillWidth: true
                    Layout.leftMargin: layout.inRectMargin
                    Layout.topMargin: 26
                    text: "The newest version is"
                    wrapMode: Text.WordWrap
                    color: properties.colorLightText3
                    font.pointSize: 14.5
                    font.family: properties.defaultFontFamily
                }
                
                Label
                {
                    Layout.fillWidth: true
                    Layout.leftMargin: layout.inRectMargin
                    text: properties.newestVersion
                    wrapMode: Text.WordWrap
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
                    Layout.leftMargin: layout.inRectMargin
                    Layout.topMargin: 14
                    backgroundColor: properties.colorLightGrayBackground
                    imagePath: properties.iconArrowDownHallow
                    imageSize: 14
                    imageSpacing: 6
                    textContent: "Download"
                    fontSize: 11.7
                    fontBold: true
                    fontColor: properties.colorBaseText
                    borderColor: properties.colorLightBorder
                    backgroundOpacityOnPressed: 0.8
                }
                
                Label
                {
                    Layout.fillWidth: true
                    Layout.leftMargin: layout.inRectMargin
                    Layout.topMargin: 42
                    text: "See the exact changes on our website at:"
                    wrapMode: Text.WordWrap
                    color: properties.colorLightText3
                    font.pointSize: 13.5
                    font.family: properties.defaultFontFamily
                }
                
                Label
                {
                    Layout.fillWidth: true
                    Layout.leftMargin: layout.inRectMargin
                    text: "www.etovex.com/new_updates"
                    wrapMode: Text.WordWrap
                    font.underline: true
                    color: properties.colorBasePurple
                    font.pointSize: 13.5
                    font.family: properties.defaultFontFamily
                }
            }
        }
    }
}