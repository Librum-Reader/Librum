import QtQuick
import QtQuick.Controls
import QtQuick.Layouts


Page
{
    id: root
    
    Rectangle
    {
        anchors.fill: parent
        color: "red"
        opacity: 0.7
        border.width: 2
        border.color: "blue"
        
        Label
        {
            anchors.centerIn: parent
            text: "Home"
            font.pointSize: 22
            font.bold: true
        }
    }
}