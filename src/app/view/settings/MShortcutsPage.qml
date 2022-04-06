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
            titleText: "Shortcuts"
            descriptionText: "Make your own experience"
            titleSize: 25
            descriptionSize: 13.25
        }
        
        Rectangle
        {
            id: details
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: 809
            Layout.topMargin: 32
            Layout.leftMargin: layout.marginToLeft
            color: properties.colorBackground
            border.color: properties.colorLightBorder
            radius: 4
            antialiasing: true
            
            
            ColumnLayout
            {
                id: inDetailsLayout
                width: parent.width
                spacing: 0
                
                
                RowLayout
                {
                    id: headerLayout
                    Layout.preferredWidth: parent.width
                    Layout.topMargin: 62
                    spacing: 0
                    
                    
                    Label
                    {
                        id: actionsLabel
                        Layout.leftMargin: 60
                        text: "ACTION"
                        color: properties.colorLightText3
                        font.pointSize: 10.25
                        font.family: properties.defaultFontFamily
                        font.bold: true
                    }
                    
                    Label
                    {
                        id: shortcutsLabel
                        Layout.leftMargin: 280
                        text: "SHORTCUTS"
                        color: properties.colorLightText3
                        font.pointSize: 10.25
                        font.family: properties.defaultFontFamily
                        font.bold: true
                    }
                    
                    Item { Layout.fillWidth: true }
                    
                    MSearchButton
                    {
                        id: searchButton
                        implicitWidth: 34
                        implicitHeight: 32
//                        Layout.preferredWidth: 34
//                        Layout.preferredHeight: 32
                        Layout.rightMargin: layout.marginToRight
                        imageSize: 14
                    }
                }
            }
        }
    }
}