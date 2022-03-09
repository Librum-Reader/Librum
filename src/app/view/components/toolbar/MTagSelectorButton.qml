import QtQuick
import QtQuick.Layouts
import QtQuick.Controls


FocusScope
{
    id: root
    implicitWidth: 100
    implicitHeight: 36
    
    signal tagChoosen()
    
    
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
            spacing: 4
            
            Image
            {
                id: tagIcon
                sourceSize.height: 18
                source: "/resources/images/tag-icon.svg"
                fillMode: Image.PreserveAspectFit
                antialiasing: false
            }
            
            Label
            {
                id: tagLabel
                color: properties.colorBaseText
                text: "Tags"
                font.pointSize: 12
                font.family: properties.defaultFontFamily
                font.weight: Font.Bold
            }
        }
    }
}
