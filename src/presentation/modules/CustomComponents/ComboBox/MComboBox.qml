import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

FocusScope
{
    id: root
    property int radius: 4
    property string headerText : "Header here"
    property int headerFontWeight : Font.Medium
    property double headerFontSize : 10.5
    property color headerFontColor : properties.colorBaseTitle
    property int headerToBoxSpacing : 2    
    property string imagePath: "noPath"
    property int imageSpacing: 4
    property int imageSize: 6
    
    implicitWidth: 100
    implicitHeight: mainLayout.implicitHeight
    
    ColumnLayout
    {
        id: mainLayout
        width: parent.width
        spacing: root.headerToBoxSpacing
        
        Label
        {
            id: header
            Layout.fillWidth: true
            text: root.headerText
            font.family: properties.defaultFontFamily
            font.pointSize: root.headerFontSize
            font.weight: root.headerFontWeight
            color: root.headerFontColor
        }
        
        Pane
        {
            id: container
            Layout.fillWidth: true
            verticalPadding: 6
            horizontalPadding: 8
            background: Rectangle
            {
                color: properties.colorBackground
                border.width: 1
                border.color: properties.colorLightBorder
                radius: root.radius
                antialiasing: true
            }
            
            
            RowLayout
            {
                id: inContainerLayout
                width: parent.width
                spacing: root.imageSpacing
                
                
                Label
                {
                    id: sortByLabel
                    color: properties.colorBaseText
                    text: "English"
                    font.pointSize: 11
                    font.family: properties.defaultFontFamily
                    font.weight: Font.Normal
                }
                
                Item { Layout.fillWidth: true }
                
                Image
                {
                    id: icon
                    sourceSize.height: root.imageSize
                    source: root.imagePath
                    fillMode: Image.PreserveAspectFit
                }
            }
        }
    }
}