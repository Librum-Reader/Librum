import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Librum.style 1.0


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