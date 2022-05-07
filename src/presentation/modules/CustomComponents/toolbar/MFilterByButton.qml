import QtQuick
import QtQuick.Layouts
import QtQuick.Controls


FocusScope
{
    id: root
    implicitWidth: 100
    implicitHeight: 36
    
    Pane
    {
        id: container
        anchors.fill: parent
        padding: 0
        background: Rectangle
        {
            color: properties.colorBackground
            border.width: 1
            border.color: properties.colorLightBorder
            radius: 5
        }
        
        
        RowLayout
        {
            anchors.centerIn: parent
            spacing: 5
            
            Image
            {
                id: filterByArrowIcon
                sourceSize.height: 14
                source: properties.iconFilter
                fillMode: Image.PreserveAspectFit
            }
            
            Label
            {
                id: filterByLabel
                color: properties.colorBaseText
                text: "Filters"
                font.pointSize: 12
                font.family: properties.defaultFontFamily
                font.weight: Font.Bold
            }
        }
    }
}
