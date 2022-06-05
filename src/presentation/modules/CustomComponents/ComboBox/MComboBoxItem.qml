import QtQuick
import QtQuick.Controls

Component
{    
    Rectangle
    {
        width: parent.width
        implicitHeight: label.implicitHeight + 8
        radius: root.radius
        color: "transparent"
        
        Label
        {
            id: label
            anchors.fill: parent
            anchors.margins: 4
            anchors.verticalCenter: parent.verticalCenter
            color: properties.colorBaseText
            text: languageName
            font.pointSize: 11
            font.family: properties.defaultFontFamily
            font.weight: Font.Normal
        }
    }
}