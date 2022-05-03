import QtQuick
import QtQuick.Layouts
import QtQuick.Controls


FocusScope
{
    id: root
    signal sortQueryEmitted()
    
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
            spacing: 6
            
            Label
            {
                id: sortByLabel
                color: properties.colorBaseText
                text: "Sort by"
                font.pointSize: 12
                font.family: properties.defaultFontFamily
                font.weight: Font.Bold
            }
            
            Image
            {
                id: sortByArrowIcon
                sourceSize.height: 6
                source: properties.iconArrowDownFilled
                fillMode: Image.PreserveAspectFit
            }
        }
    }
}
