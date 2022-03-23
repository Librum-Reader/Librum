import QtQuick
import QtQuick.Layouts
import QtQuick.Controls


Page
{
    id: root
    background: Rectangle
    {
        anchors.fill: parent
        color: properties.pagesBackground
    }
    
    
    Label
    {
        anchors.centerIn: parent
        text: "Advanced Settings"
        font.pointSize: 36
        font.family: properties.defaultFontFamily
    }
}