import QtQuick 2.15
import QtQuick.Layouts 1.15
import Librum.style 1.0

Item
{
    id: root
    implicitWidth: 72
    implicitHeight: 72
    
    Rectangle
    {
        id: outerRect
        anchors.fill: parent
        color: Style.colorBasePurple
        radius: 8
        antialiasing: true
        
        Rectangle
        {
            id: innerRect
            width: parent.width / 2
            height: parent.height / 2
            anchors.centerIn: parent
            color: Style.loginContentBackground
            radius: width / 4
            antialiasing: true
        }
    }
}