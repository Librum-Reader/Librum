import QtQuick
import QtQuick.Layouts
import QtQuick.Controls


FocusScope
{
    id: root
    implicitWidth: 58
    implicitHeight: 32
    
    
    Rectangle
    {
        id: container
        anchors.fill: parent
        color: properties.colorBackground
        border.color: properties.colorLightBorder
        radius: 4
        antialiasing: true
        
        
        RowLayout
        {
            id: layout
            anchors.centerIn: parent
            spacing: 5
            
            
            Label
            {
                id: bookCountLabel
                text: bookCount.toString()
                color: properties.colorBaseText
                font.family: properties.defaultFontFamily
                font.pointSize: 11
                font.bold: true
            }
            
            Image
            {
                id: dropDownArrowImage
                source: properties.iconArrowDownFilled
                sourceSize.width: 8
                fillMode: Image.PreserveAspectFit
            }
        }
    }
}
