import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Librum.style


Item
{
    id: root
    implicitWidth: 250
    implicitHeight: 600
    
    
    Rectangle
    {
        anchors.fill: parent
        color: Style.colorBackground
        
        
        Label
        {
            anchors.centerIn: parent
            text: "Bookmarks"
            font.pointSize: 14
            font.weight: Font.Bold
        }
    }
}