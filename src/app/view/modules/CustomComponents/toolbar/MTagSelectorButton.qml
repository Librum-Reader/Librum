import QtQuick
import QtQuick.Layouts
import QtQuick.Controls


FocusScope
{
    id: root
    signal tagChoosen()
    
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
            spacing: 4
            
            Image
            {
                id: tagIcon
                sourceSize.height: 18
                source: properties.iconTag
                fillMode: Image.PreserveAspectFit
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
