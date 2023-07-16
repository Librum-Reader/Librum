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
        id: background
        anchors.fill: parent
        color: Style.colorContainerBackground
        
        
        Label
        {
            anchors.centerIn: parent
            text: "In Development"
            color: Style.colorTitle
            font.pointSize: 20
            font.weight: Font.Bold
        }
    }
}