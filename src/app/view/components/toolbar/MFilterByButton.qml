import QtQuick
import QtQuick.Layouts
import QtQuick.Controls


FocusScope
{
    implicitWidth: 100
    implicitHeight: 36
    
    Rectangle
    {
        id: container
        anchors.fill: parent
        color: properties.colorBackground
        border.width: 1
        border.color: properties.colorLightBorder
        radius: 5
        
        RowLayout
        {
            anchors.centerIn: parent
            spacing: 5
            
            Image
            {
                id: filterByArrowIcon
                sourceSize.height: 14
                source: "/resources/images/filter-icon.svg"
                fillMode: Image.PreserveAspectFit
                antialiasing: false
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
