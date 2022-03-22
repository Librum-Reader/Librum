import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import CustomComponents


Page
{
    id: root
    
    background: Rectangle
    {
        anchors.fill: parent
        color: properties.pagesBackground
    }
    
    
    MSettingsSidebar
    {
        height: parent.height
        anchors.left: parent.left
        anchors.top: parent.top
    }
}
