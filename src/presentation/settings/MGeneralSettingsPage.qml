import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Librum.style


Page
{
    id: root
    background: Rectangle
    {
        anchors.fill: parent
        color: Style.pagesBackground
    }
    
    
    Label
    {
        anchors.centerIn: parent
        text: "General Settings"
        font.pointSize: 36
        font.family: Style.defaultFontFamily
    }
}